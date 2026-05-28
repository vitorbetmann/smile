# ParticleSystem Module — Design Summary

## Context

Building a `ps`-prefixed ParticleSystem module for **Smile**, which doesn't handle rendering — it
integrates with libraries like raylib.

---

## Decided: Core Architecture

### Memory Layout

- **Memory pool** — one upfront `malloc` for all particles instead of per-particle allocation.
  Eliminates allocation cost and cache thrashing.
- **Swap-and-pop** — when a particle at index `i` dies, swap it with the particle at `count-1` and
  decrement `count`. Active particles always live in `0..count-1`; the update loop is simply
  `for (int i = 0; i < count; i++)`.
- **Array of Structures (AoS)** — SoA (separate arrays per field) was rejected: swap-and-pop would
  require synchronizing every array simultaneously, which is easy to corrupt and not worth the cache
  benefit.

### Flyweight Pattern

The `ParticleSystem` stores the **spawn recipe** — ranges used to generate particles (lifetime
min/max, acceleration min/max, emission area, etc.). Individual `Particle` structs hold only
per-particle runtime state.

**Why ranges must persist on the system:** the system needs the ranges every time it emits, since
`psBurst` can be called multiple times over the system's lifetime (e.g. a brick gets hit, particles
die, brick gets hit again).

### Particle Struct

```c
typedef struct {
    float x, y;                       // position — updated every frame
    float velocityX, velocityY;       // velocity — read every frame
    float accelerationX, accelerationY; // acceleration — read every frame
    float age;                        // counts up from 0 every frame
    float lifetime;                   // upper bound — stored for age/lifetime ratio (e.g. fade alpha)
} Particle;
```

**`age` instead of `lifeLeft`:** `age` counts *up* from `0` to `lifetime`. The trade-off is that a
fade-out ratio becomes `1.0f - age / lifetime` instead of `lifeLeft / lifetime` — one extra
operation in the user's `psForEach` callback. The gain is that game-loop crossing checks read
naturally: `p->age + dt >= p->lifetime` means "this particle dies next update," with no directional
inversion required.

**Why no hot/cold split:** `velocityX`, `velocityY`, `accelerationX`, `accelerationY` are all read
every frame to update position — there are no cold fields worth splitting out.

### Particle Recycling

When a dead particle is recycled, all values are re-rolled fresh from the system's ranges.
Preserving values was rejected: re-randomizing is cheap (a few RNG calls), produces natural visual
variety, and is simpler. Recycled particles following identical trajectories looks unnatural in
continuous emitters like fire.

**Consequence:** `Particle` does not need a separate "initial state" copy.

---

## Decided: Data Model and Callbacks

### Callback Signatures

Three distinct callback types cover the three distinct contracts:

```c
typedef void (*ParticleFn)(const Particle *p, void *args);
typedef void (*InfluenceFn)(Particle *p, void *context);
typedef void (*SnapshotInfluenceFn)(Particle *p, const Particle *snapshot, int count, void *context);
```

These are not unified. Each type is honest about what its caller can do:

- `ParticleFn` — read-only. Used by `psForEach` for observation (rendering, condition detection).
  `const` is a compile-time contract: a draw callback that accidentally writes to `p->x` is a
  compiler error, not a silent bug.
- `InfluenceFn` — mutable. Used by `psSetInfluence` for stateless per-frame mutation driven by
  external state. Every argument is used; nothing is ignored.
- `SnapshotInfluenceFn` — mutable, with read-only access to all particles at frame start. Used by
  `psSetSnapshotInfluence`. `snapshot` and `count` are the entire point of this type; they cannot
  be dropped into a unified signature without introducing ignored arguments.

Unifying `ParticleFn` and `InfluenceFn` by dropping `const` was rejected: the `const` on
`ParticleFn` is not paternalism — it is a guarantee that `psForEach` is an observation tool.
Removing it would silently allow rendering callbacks to mutate particle state.

**`psForEach` is read-only by design.** Mutations belong in influence functions, which run inside
`psUpdate`. This mirrors the SceneManager pattern: callbacks are registered once, the module owns
the when. The user should not need to call `psForEach` every frame in the right order — that
ordering is the library's responsibility. For one-time bulk mutation (e.g. shift all particles by
10 units), set an influence function, let one `psUpdate` run, then unset it.

**`void *args`** — arbitrary client context (e.g. a color, a texture) without Smile needing to
know about it. Iteration is controlled entirely by `psForEach` internally. Clients define what to
do with a particle, not which particle to act on, and never hold an index or access the array
directly.

This keeps Smile fully decoupled from raylib while giving the client everything needed to render.

---

## Decided: Shared API

These functions work identically regardless of which system type is used.

```c
// Lifecycle
ParticleSystem *psCreate(int maxParticles, float x, float y);
int             psDestroy(ParticleSystem *ps);
int             psReset(ParticleSystem *ps);   // kills all active particles; system remains valid

// Configuration — spawn recipe
int psSetLifetime(ParticleSystem *ps, float min, float max);
int psSetVelocity(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);
int psSetAcceleration(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);
int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape);
int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY);
int psSetOrigin(ParticleSystem *ps, float x, float y);

// Emission
int psBurst(ParticleSystem *ps, int n);        // spawns n particles immediately
int psStream(ParticleSystem *ps, float rate);  // particles per second; 0.0f disables

// Use
int psUpdate(ParticleSystem *ps, float dt);
int psForEach(const ParticleSystem *ps, ParticleFn fn, void *args);

// Query
float psGetX(const ParticleSystem *ps);
float psGetY(const ParticleSystem *ps);
int   psGetActive(const ParticleSystem *ps);  // number of live particles
int   psGetIdle(const ParticleSystem *ps);    // available slots (maxParticles - active)
```

**`psCreate`:** returns `NULL` on invalid input (`maxParticles <= 0`) or allocation failure. Both
paths log before returning. Callers must check the return value before use.

**`psDestroy`:** returns `int` — `RES_OK` on success, `< 0` if `ps` is `NULL`. The non-void return
is for testability: tests that exercise failure paths (e.g. a `NULL` result from a failed
`psCreate`) can assert on the return value rather than relying on side effects alone.

**`psReset`:** kills all active particles without freeing the system. Useful for pooled systems —
allocate once, reuse across waves without the cost of destroy/create. Two specific behaviors:

- **Accumulator is not reset.** `psReset` clears visual state, not emission configuration. Zeroing
  the accumulator would be a surprising side effect; the sub-particle fraction it holds (`< 1.0`)
  has no visible impact on resumption.
- **Stream rate is not reset.** If `psStream` is active when `psReset` is called, emission resumes
  on the next `psUpdate`. Users who want to stop streaming after a reset must call
  `psStream(ps, 0.0f)` explicitly.

**`psGetX` / `psGetY`:** return the system's origin. Both take `const ParticleSystem *` — they are
pure queries with no side effects. Smile has no tuple or vector type, so the origin is exposed as
two separate float getters rather than a compound return value.

**`psGetActive` / `psGetIdle`:** convenience pair for the staged-emission pattern (e.g. pool of 100,
emit 20 at a time, emit more as slots free up). `active + idle == maxParticles` always, so the
maximum capacity is recoverable without a separate getter.

**Repeated bursting:** loop functionality (`psSetLoop`, `psSetLoopInterval`) was considered and
rejected. The user can implement repeated bursting with two lines in their update function —
`psGetActive(ps) == 0` signals that all particles are dead and `psBurst` can be called again.
A built-in loop would require storing a burst count separately from `psBurst`'s `n` parameter,
and "re-fire when all particles are dead" is game logic, not particle system logic.

### Emission Shape and Spread

The original single `EmissionType` enum was replaced with a shape enum and inner/outer spread
values. This eliminates a separate mode enum — the relationship between inner and outer spread
encodes fill, border, and thickness naturally.

```c
typedef enum {
    PS_SHAPE_ELLIPSE,  // ellipse of semi-axes outerX × outerY (circle = equal semi-axes)
    PS_SHAPE_RECT,     // rectangle of dimensions outerX × outerY
} psEmissionShape;
```

**Why ellipse instead of circle:** a circle is a degenerate ellipse (`outerX == outerY`). A single
`PS_SHAPE_ELLIPSE` covers both without adding an enumerator.

**Point emission via zero spread:** point emission is `PS_SHAPE_ELLIPSE` with all spreads `0.0f` —
no dedicated enumerator is needed. This is also the default at `psCreate`, so a freshly created
system emits at the origin without any configuration.

**Why inner/outer spread replaces a mode enum:** three spawn patterns fall out of the inner/outer
relationship with no extra concept:

| Pattern               | Condition                              |
|-----------------------|----------------------------------------|
| Filled area           | `innerX = innerY = 0`                  |
| Zero-thickness border | `innerX == outerX && innerY == outerY` |
| Thick ring / frame    | `0 < inner < outer`                    |

A separate `psEmissionMode` enum (`FILLED` / `BORDER`) was rejected — it couldn't express thickness
and required an extra setter.

**Two setters replace `psSetEmissionArea`:**

```c
int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape);
int psSetInnerSpread(ParticleSystem *ps, float x, float y);
int psSetOuterSpread(ParticleSystem *ps, float x, float y);
```

Each setter is a single responsibility. `psSetEmissionArea(ps, type, spreadX, spreadY)` was
rejected — five parameters, shape and spread conflated. The original `psSetSpread` with four float
parameters was split further: each half is independently settable, which is less error-prone and
saves a call when only one boundary needs changing.

**Validation:** `psSetOuterSpread` and `psSetInnerSpread` enforce `outer >= inner` on both axes. If
`outerX < innerX` or `outerY < innerY`, the call is rejected (logged and returns negative) — the
inner boundary cannot exceed the outer. `outer == inner` is valid and produces zero-thickness border
emission.

**Defaults at `psCreate`:** `PS_SHAPE_ELLIPSE`, all spreads `0.0f`. A freshly created system emits
at the origin (point emission) without any configuration.

**Zero-spread behavior:** if a shape is set but spread is never configured, all spreads remain
`0.0f` and particles spawn at the origin — effectively point emission. This is user error; Smile
may log a warning but does not guard against it.

---

## Decided: Emission Model

Two distinct, independent emission functions replace the previous unified `psEmit`:

```c
int psBurst(ParticleSystem *ps, int n);
int psStream(ParticleSystem *ps, float rate);
```

They are honest about what they do and neither bleeds into the other's concerns.

### psBurst

Attempts to spawn up to `n` particles immediately using the system's current spawn recipe. Capped
at the number of idle slots; excess count is silently dropped. `psBurst` can be called at any time,
including while particles from a previous burst are still alive — the pool cap prevents
over-emission without any additional guard logic.

### psStream

Enables continuous rate-based emission driven by `psUpdate`. Each frame, `psUpdate` accumulates
`rate * dt` fractional particles and spawns whole numbers as the accumulator crosses integers.

`psStream(ps, 0.0f)` disables streaming. The accumulator is not reset — it resumes if the rate is
restored.

**Rate cap:** rate is capped internally at `maxParticles` per second. Requesting a higher rate is
not an error; it simply resolves to the pool maximum.

**Pool exhaustion:** if no idle slots are available when the accumulator would spawn, that spawn is
skipped. The accumulator is not penalized and continues accumulating. Emission resumes as particles
die and slots free up.

**`psStream` takes effect immediately** — there is no deferred-start behavior. Calling `psStream`
with a non-zero rate starts the accumulator on the next `psUpdate`.

**`psStream` is independent of `psBurst`** — both can be active on the same system simultaneously,
drawing from the same pool.

---

## Decided: Unified System Design

A single system type. Influence is configured via setters; `psUpdate` stays simple.

```c
ParticleSystem *psCreate(int maxParticles, float x, float y);
int             psUpdate(ParticleSystem *ps, float dt);

// Stateless influence — each particle updated independently using outside state
// (e.g. player position, wind, cursor). context may be NULL if not needed.
int psSetInfluence(ParticleSystem *ps, void *context, InfluenceFn fn);

// Snapshot influence — inter-particle awareness (e.g. boids)
// Each particle reads all particles' state at time t, writes to t+1.
// Triggers lazy allocation of the second buffer; returns 0 on success, < 0 on failure.
// context is passed through to the callback for any external state needed alongside snapshot access.
int psSetSnapshotInfluence(ParticleSystem *ps, void *context, SnapshotInfluenceFn fn);
```

**Naming rationale:** "external vs. internal" and "external vs. snapshot" were both considered.
"External/internal" describes the *source* of influence (outside the system vs. other particles),
but the functions differ in their *update model*, which is the thing that actually matters to the
caller. A user could pass `psForEach` on the same system as context to `psSetInfluence` to do
sequential inter-particle effects — technically "internal" by source but going through the
"external" function. "Snapshot" names the guarantee directly: every particle reads from a frozen
frame-start state. It is also self-documenting to a reader unfamiliar with the distinction, making
asymmetric names (`psSetInfluence` / `psSetSnapshotInfluence`) acceptable. The real distinction is
the update model:

- `psSetInfluence` — stateless, each particle updated independently.
- `psSetSnapshotInfluence` — snapshot semantics: all particles read from time `t`, write to time
  `t+1`. Requires a double-buffered pool.

**`context` on `psSetSnapshotInfluence`:** consistent with `psSetInfluence`. In C, function pointers
cannot close over state, so a `void *context` parameter is required for any external state the
callback needs alongside snapshot access. May be `NULL` if not needed.

**`count` in snapshot callback:** the pool has `maxParticles` slots but only `count` are active at
any given frame. Without `count`, the callback cannot safely iterate the snapshot buffer.

**Update behavior:** `psUpdate` applies whichever influence functions are set, in sequence —
stateless first, then snapshot. No branching on a mode; just calling function pointers that may or
may not be `NULL`. Both can be active simultaneously.

**Influence ordering:** stateless influence runs before snapshot. The snapshot callback reads from
the pre-update copy of the buffer (time `t`), so it never sees positions modified by stateless
influence in the same frame. This one-frame lag is a consequence of double-buffering and is
intentional.

**Lazy second buffer:** starts as `NULL` at create time. `psSetSnapshotInfluence` triggers
allocation and copies the current buffer in. Only systems that use inter-particle awareness pay the
`2 × maxParticles × sizeof(Particle)` memory cost.

**Second buffer teardown policy:** once allocated, the second buffer is never freed mid-lifetime —
passing `NULL` for `fn` stops snapshot influence from running but leaves the buffer intact. This
avoids repeated alloc/free churn in behavioral state machines that toggle snapshot influence on and
off. To reclaim the memory, destroy and recreate the system.

**Error handling:** `psSetSnapshotInfluence` returns `int` — `0` on success, negative on allocation
failure. Consistent with Smile's existing error convention (e.g. SceneManager). The negative value
signals that something went wrong; the specific cause is not exposed to the client.

**Dynamic behavior:** influence functions can be swapped at any point during the system's lifetime —
no need to destroy and recreate. This enables user-driven behavioral state machines (e.g. particles
move left for 2s, then switch to moving right).

---

## Decided: Cross-System Interaction

**Scenario:** a water surface system reacts to fish positions/velocity; a fish system uses boids (
inter-particle awareness); the two systems influence each other.

Covered by `psSetInfluence`. Pass the other `ParticleSystem *` as `context`, then use `psForEach`
inside the influence function to read its particles. Since `psForEach` exposes particles as
`const Particle *`, the influencing system's state is read-only — one system cannot corrupt
another's particles.

```c
void waterInfluence(Particle *p, void *context) {
    ParticleSystem *fish = (ParticleSystem *)context;
    psForEach(fish, readFishState, p);
}

psSetInfluence(water, fish, waterInfluence);
```

**Note:** iterating one system's particles while calling `psForEach` on another is O(n×m) per frame.
Acceptable at typical game scales for small-to-medium particle counts.

**Re-entrancy:** `psForEach` is safe to call on a system other than the one currently being updated.
Calling `psForEach` on the *same* system that is mid-update (e.g. passing it as its own context) is
undefined behavior; debug builds assert against this.

No additional design needed.

---

## Showcase Demo — Fireworks → Fish → Boids

A planned interactive demo that exercises the full module in sequence. **To be written concurrently
with the module** so API awkwardness surfaces during development rather than after.

1. **Rocket** — single particle emitter traveling upward; game loop detects near-death via
   `p->age + dt >= p->lifetime` in a `psForEach` pass, records position, spawns trail (`psStream`)
   and explosion (`psBurst`) at that position after `psUpdate`
2. **Explosion** — `psBurst` emitter; game loop detects expiring particles each frame and spawns the
   boids system once the last one is gone (`psGetActive == 0`)
3. **Fish** — boids system using `psSetSnapshotInfluence` for separation, alignment, and cohesion;
   `psSetInfluence` with mouse cursor position as context — fish flee the cursor (hook/predator
   interaction)
4. **Water** — separate system using `psStream` for continuous surface ripple; `psSetInfluence` with
   the fish system as context reads fish positions via `psForEach`

**Interactive element:** the mouse cursor acts as a hook. Fish flee it via `psSetInfluence`,
demonstrating real-time external influence with a user-controlled context. Swapping the influence
function at runtime (repel → attract) demonstrates behavioral state machines.

Capabilities exercised:

- `psCreate` with origin positioning
- `psSetSnapshotInfluence` with double-buffered pool
- `psSetInfluence` for cross-system interaction and cursor-driven behavior
- `psForEach` for read-only inter-system state access and game-loop condition detection
- `psBurst` for one-shot effects
- `psStream` for continuous effects (trail, water surface)
- `psGetActive` for transition detection between stages

**Ownership chain:** each spawned system is owned by the user-level game context. Smile holds no
references across the chain. All chaining logic lives in the game loop.

---

## Pending

- Finalize `psEmissionShape` sampling math (uniform distribution for ellipse fills and borders)
