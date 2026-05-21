# ParticleSystem Module — Design Summary

## Context

Building a `ps`-prefixed ParticleSystem module for **Smile**, a dependency-free C23 static library for 2D game development targeting game jams and rapid prototyping. Smile doesn't handle rendering — it integrates with libraries like raylib.

### Lifecycle Conventions

All modules follow a `Start → Use → Stop` lifecycle with two variants:

| Pattern | Functions | Used for |
|---|---|---|
| Module-level singleton | `xStart()` / `xStop()` | Modules with global state (e.g. scene manager) |
| Object-level instance | `xCreate()` / `xDestroy()` | Owned handles (e.g. particle system) |

### What "Dependency-Free" Means

Dependency-free is from the **user's perspective**: the user only includes Smile's header and links Smile's library. Smile may use platform APIs or bundled header-only libraries internally (e.g. `uthash`) as long as the user doesn't have to install or link anything themselves.

---

## Decided: Core Architecture

### Memory Layout

- **Memory pool** — one upfront `malloc` for all particles instead of per-particle allocation. Eliminates allocation cost and cache thrashing.
- **Swap-and-pop** — when a particle at index `i` dies, swap it with the particle at `count-1` and decrement `count`. Active particles always live in `0..count-1`; the update loop is simply `for (int i = 0; i < count; i++)`.
- **Array of Structures (AoS)** — SoA (separate arrays per field) was rejected: swap-and-pop would require synchronizing every array simultaneously, which is easy to corrupt and not worth the cache benefit at game-jam scale.

### Flyweight Pattern

The `ParticleSystem` stores the **spawn recipe** — ranges used to generate particles (lifetime min/max, acceleration min/max, emission area, etc.). Individual `Particle` structs hold only per-particle runtime state.

**Why ranges must persist on the system:** `psEmit` can be called multiple times over the system's lifetime (e.g. a brick gets hit, particles die, brick gets hit again). The system needs the ranges every time it emits.

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

**`age` instead of `lifeLeft`:** `age` counts *up* from `0` to `lifetime`, which keeps the direction consistent with the lifetime event threshold (0.0 = just born, 1.0 = about to die). A `lifeLeft` field counting down would require a directional inversion inside `psUpdate`'s threshold-crossing check — a permanent source of confusion and off-by-one bugs. The trade-off is that a fade-out ratio becomes `1.0f - age / lifetime` instead of `lifeLeft / lifetime`; that's one extra operation in the user's `psForEach` callback.

**Why no hot/cold split:** `velocityX`, `velocityY`, `accelerationX`, `accelerationY` are all read every frame to update position — there are no cold fields worth splitting out.

### Particle Recycling

When a dead particle is recycled, all values are re-rolled fresh from the system's ranges. Preserving values was rejected: re-randomizing is cheap (a few RNG calls), produces natural visual variety, and is simpler. Recycled particles following identical trajectories looks unnatural in continuous emitters like fire.

**Consequence:** `Particle` does not need a separate "initial state" copy.

---

## Decided: Data Model and Callbacks

### Callback Signature

```c
typedef void (*ParticleFn)(const Particle *p, void *args);
```

- `const Particle *p` — read-only access to one particle per invocation. Clients cannot modify particles directly through this callback.
- `void *args` — arbitrary client context (e.g. a color, a texture) without Smile needing to know about it.
- Iteration is controlled entirely by `psForEach` internally. Clients define what to do with a particle, not which particle to act on, and never hold an index or access the array directly.

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

// Use
int psEmit(ParticleSystem *ps, int count);
int psUpdate(ParticleSystem *ps, float dt);
int psForEach(ParticleSystem *ps, ParticleFn fn, void *args);

// Query
float psGetX(ParticleSystem *ps);
float psGetY(ParticleSystem *ps);
int psGetActive(ParticleSystem *ps);  // number of live particles
int psGetIdle(ParticleSystem *ps);    // available slots (maxParticles - active)
```

**`psReset`:** kills all active particles without freeing the system. Useful for pooled systems — allocate once, reuse across waves without the cost of destroy/create.

**`psGetX` / `psGetY`:** return the system's origin. Smile has no tuple or vector type, so the origin is exposed as two separate float getters rather than a compound return value.

**`psGetActive` / `psGetIdle`:** convenience pair for the staged-emission pattern (e.g. pool of 100, emit 20 at a time, emit more as slots free up). `active + idle == maxParticles` always, so the maximum capacity is recoverable without a separate getter.

### Emission Shape and Spread

The original single `EmissionType` enum was replaced with a shape enum and inner/outer spread values. This eliminates a separate mode enum — the relationship between inner and outer spread encodes fill, border, and thickness naturally.

```c
typedef enum {
    PS_SHAPE_POINT,    // convenience alias for zero-spread ellipse; spread ignored in psEmit
    PS_SHAPE_ELLIPSE,  // ellipse of semi-axes outerX × outerY (circle = equal semi-axes)
    PS_SHAPE_RECT,     // rectangle of dimensions outerX × outerY
} psEmissionShape;
```

**Why ellipse instead of circle:** a circle is a degenerate ellipse (`outerX == outerY`). A single `PS_SHAPE_ELLIPSE` covers both without adding an enumerator.

**`PS_SHAPE_POINT` as a convenience alias:** mathematically, point emission is a zero-spread ellipse. `PS_SHAPE_POINT` is kept as an explicit enumerator so users don't need two calls (`psSetEmissionShape` + `psSetSpread(0,0,0,0)`) to express the most common case. In `psEmit`, it is handled as an early-out before any spread math. The spread fields are stored but ignored.

**Why inner/outer spread replaces a mode enum:** three spawn patterns fall out of the inner/outer relationship with no extra concept:

| Pattern | Condition |
|---------|-----------|
| Filled area | `innerX = innerY = 0` |
| Zero-thickness border | `innerX == outerX && innerY == outerY` |
| Thick ring / frame | `0 < inner < outer` |

A separate `psEmissionMode` enum (`FILLED` / `BORDER`) was rejected — it couldn't express thickness and required an extra setter.

**Two setters replace `psSetEmissionArea`:**

```c
int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape);
int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY);
```

Each setter is a single responsibility. `psSetEmissionArea(ps, type, spreadX, spreadY)` was rejected — five parameters, shape and spread conflated, forced an unused spread argument for `PS_SHAPE_POINT`.

**Defaults at `psCreate`:** `PS_SHAPE_POINT`, all spreads `0.0f`. A freshly created system emits at the origin without any configuration.

**Zero-spread behavior:** if a non-point shape is set but `psSetSpread` is never called, all spreads remain `0.0f` and particles spawn at the origin — effectively point emission. This is user error; Smile may log a warning in `psEmit` but does not guard against it.

**Spread with point shape:** `psSetSpread` has no effect when shape is `PS_SHAPE_POINT` — spread values are stored but ignored in `psEmit`.

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
// No context parameter — if external state is needed, close over it in the function.
int psSetSnapshotInfluence(ParticleSystem *ps, SnapshotInfluenceFn fn);
```

**Naming rationale:** "external vs. internal" was rejected as the distinguishing axis. The real distinction is the update model:
- `psSetInfluence` — stateless, each particle updated independently.
- `psSetSnapshotInfluence` — snapshot semantics: all particles read from time `t`, write to time `t+1`. Requires a double-buffered pool.

**Why no context on `psSetSnapshotInfluence`:** the callback only operates on the system's own particles. The snapshot buffer and count are passed directly by `psUpdate`. If external state is genuinely needed alongside snapshot access, close over it in the function pointer.

**`count` in snapshot callback:** the pool has `maxParticles` slots but only `count` are active at any given frame. Without `count`, the callback cannot safely iterate the snapshot buffer.

**Update behavior:** `psUpdate` applies whichever influence functions are set, in sequence — stateless first, then snapshot. No branching on a mode; just calling function pointers that may or may not be `NULL`. Both can be active simultaneously.

**Lazy second buffer:** starts as `NULL` at create time. `psSetSnapshotInfluence` triggers allocation and copies the current buffer in. Only systems that use inter-particle awareness pay the `2 × maxParticles × sizeof(Particle)` memory cost.

**Error handling:** `psSetSnapshotInfluence` returns `int` — `0` on success, negative on allocation failure. Consistent with Smile's existing error convention (e.g. SceneManager). The negative value signals that something went wrong; the specific cause is not exposed to the client.

**Dynamic behavior:** influence functions can be swapped at any point during the system's lifetime — no need to destroy and recreate. This enables user-driven behavioral state machines (e.g. particles move left for 2s, then switch to moving right).

---

## Decided: Cross-System Interaction

**Scenario:** a water surface system reacts to fish positions/velocity; a fish system uses boids (inter-particle awareness); the two systems influence each other.

Covered by `psSetInfluence`. Pass the other `ParticleSystem *` as `context`, then use `psForEach` inside the influence function to read its particles. Since `psForEach` exposes particles as `const Particle *`, the influencing system's state is read-only — one system cannot corrupt another's particles.

```c
void waterInfluence(Particle *p, void *context) {
    ParticleSystem *fish = (ParticleSystem *)context;
    psForEach(fish, readFishState, p);
}

psSetInfluence(water, fish, waterInfluence);
```

**Note:** iterating one system's particles while calling `psForEach` on another is O(n×m) per frame. Acceptable at game-jam scale.

No additional design needed.

---

## Decided: Lifetime Events

Trigger a callback at a specific point in a particle's lifetime. Smile fires the callback when the threshold is crossed, passing the triggering particle as read-only. The user owns everything that comes out of the callback — including any new systems created inside it.

```c
// Returns 0 on success, < 0 on error — consistent with Smile's error convention
int psSetEvent(ParticleSystem *ps, float threshold, ParticleFn fn, void *context);
```

### Threshold semantics

`threshold` is an **elapsed fraction**: `0.0` = just born, `1.0` = about to die. This matches the direction of `age` on the `Particle` struct and requires no mental inversion when reading the code.

The crossing check inside `psUpdate` (for any threshold `t`):

```c
float ageNew = p->age + dt;
float thresholdAge = p->lifetime * t;
if (p->age < thresholdAge && ageNew >= thresholdAge)
    event.fn(p, event.context);
```

### Fire before removal

Lifetime events at threshold `1.0` fire **before** swap-and-pop. The callback receives the particle at its final valid state — position, velocity, and all other fields are readable. This is the useful behaviour: the main use case for a death event is spawning a child system at `p->x, p->y`.

Firing after removal would require either a per-particle copy (overhead for every death, even without listeners) or a dangling pointer. Both are worse.

### Multiple thresholds

A system supports any number of lifetime events via a **small dynamic array** (stretchy buffer). Starts with a default capacity and grows via `realloc` when exceeded. No artificial cap; the user only pays for what they register.

Linked list was rejected — per-node heap allocation and pointer indirection on every traversal undermine the memory pool's cache benefits, and the actual memory savings are negligible at game-jam scale (3 unused structs ≈ 60 bytes per system). The dynamic array gives the same "pay for what you use" property with contiguous memory and no pointer chasing.

Typical use — spawning a child system at a particle's position:

```c
void onDeath(const Particle *p, void *context) {
    Game *game = (Game *)context;
    ParticleSystem *sparks = psCreate(50, p->x, p->y);
    psEmit(sparks, 50);
    game->activeSystems[game->systemCount++] = sparks;
}

psSetEvent(ps, 1.0f, onDeath, game);
```

`psCreate` takes an origin (`x`, `y`) so the spawned system can be positioned at the triggering particle's location. `psSetOrigin` covers the case where the origin needs to change after creation (e.g. a continuous emitter following a moving object).

Smile never holds a reference to spawned systems — ownership and lifecycle management are entirely the user's responsibility.

---

## Showcase Demo — Fireworks → Fish → Boids

A planned interactive demo that exercises the full module in sequence. **To be written concurrently with the module** so API awkwardness surfaces during development rather than after.

1. **Rocket** — single particle emitter travelling upward; lifetime event at `0.0` spawns a trail effect; lifetime event at `1.0` spawns an explosion system at its position
2. **Explosion** — burst emitter; lifetime event on each particle transforms it into a fish (destroys explosion system, spawns boids system at same position)
3. **Fish** — boids system using `psSetSnapshotInfluence` for separation, alignment, and cohesion; `psSetInfluence` with mouse cursor position as context — fish flee the cursor (hook/predator interaction)
4. **Water** — separate system using `psSetInfluence` with the fish system as context; reads fish positions via `psForEach` to ripple the surface

**Interactive element:** the mouse cursor acts as a hook. Fish flee it via `psSetInfluence`, demonstrating real-time external influence with a user-controlled context. Swapping the influence function at runtime (repel → attract) demonstrates behavioral state machines.

Capabilities exercised:
- `psCreate` with origin positioning
- `psSetEvent` for chained system spawning and transformation (uses multiple thresholds on rocket)
- `psSetSnapshotInfluence` with double-buffered pool
- `psSetInfluence` for cross-system interaction and cursor-driven behavior
- `psForEach` for read-only inter-system state access

**Ownership chain:** each spawned system is owned by the user-level game context, passed through as `context` in each lifetime event callback. Smile holds no references across the chain.

---

## Pending

- Finalize `psEmissionShape` sampling math (uniform distribution for ellipse fills and borders)