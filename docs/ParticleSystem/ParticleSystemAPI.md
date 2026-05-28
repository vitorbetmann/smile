# ParticleSystem — API ✨

The `ParticleSystem` module provides a configurable, instance-based particle
simulation for 2D game effects such as explosions, fire, rain, and trails.

For workflow examples see: [ParticleSystem – Getting Started](README.md)

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#-module-header)
- [Data Types](#-data-types)
    - [Structs](#-structs)
    - [Enums](#-enums)
    - [Function Pointers](#-function-pointers)
- [Functions](#-functions)
    - [Lifecycle](#-lifecycle)
    - [Emission](#-emission)
    - [Update / Query](#-update--query)
    - [Getters](#-getters)
    - [Setters](#-setters)

---

## 😊 Module Header

The module's header is `ParticleSystem.h`. Its full Smile path is:
`smile/include/ParticleSystem.h`

✅ Example

```c
#include <ParticleSystem.h>
```

---

## 📦 Data Types

### — Structs

| `Particle` |
|------------|

Represents a single active particle in the system.

- Fields:
    - `float x, y` — Current position.
    - `float velocityX, velocityY` — Current velocity components.
    - `float accelerationX, accelerationY` — Current acceleration components.
    - `float age` — Time elapsed since emission, in seconds.
    - `float lifetime` — Total lifespan of the particle, in seconds.

- Notes:
    - Passed as `const Particle *` to `ParticleFn` and as the snapshot array
      to `SnapshotInfluenceFn`. Passed as `Particle *` to `InfluenceFn` and
      `SnapshotInfluenceFn` so influence callbacks may modify the live particle.
    - The `Particle` struct is public so callers can read fields inside
      callbacks; do not store or cache pointers to individual particles across
      frames.

✅ Example

```c
static void printParticle(const Particle *p, void *context)
{
    (void)context;
    printf("pos=(%.1f, %.1f) age=%.2fs\n", p->x, p->y, p->age);
}
```

<br>

### — Enums

| `psEmissionShape` |
|-------------------|

Geometric shape used to sample particle spawn positions within the emission
area.

- Values:
    - `PS_SHAPE_ELLIPSE` — Particles emitted within an elliptical area.
    - `PS_SHAPE_RECT` — Particles emitted within a rectangular area.
    - `SHAPE_COUNT` — Sentinel; number of valid emission shapes.

✅ Example

```c
psSetEmissionShape(ps, PS_SHAPE_ELLIPSE);
```

<br>

### — Function Pointers

| `void (*ParticleFn)(const Particle *p, void *context)` |
|--------------------------------------------------------|

Callback invoked for each active particle during `psForEach`.

- Parameters:
    - `p` — Read-only pointer to the active particle.
    - `context` — Optional caller-supplied pointer forwarded from `psForEach`.

- Notes:
    - Intended for read-only operations such as rendering or accumulating
      statistics. Use `InfluenceFn` or `SnapshotInfluenceFn` to modify
      particles.

✅ Example

```c
#include <raylib.h>

static void drawParticle(const Particle *p, void *context)
{
    (void)context;
    DrawCircleV((Vector2){ p->x, p->y }, 3.0f, WHITE);
}
```

<br>

| `void (*InfluenceFn)(Particle *p, void *context)` |
|---------------------------------------------------|

Callback that modifies a particle once per update step.

- Parameters:
    - `p` — Pointer to the particle being modified.
    - `context` — Optional caller-supplied pointer forwarded from
      `psSetInfluence`.

- Notes:
    - Called by `psUpdate` for every active particle before position
      integration. Changes to `p->velocityX`, `p->velocityY`,
      `p->accelerationX`, or `p->accelerationY` take effect in the same frame.

✅ Example

```c
static void applyGravity(Particle *p, void *context)
{
    float *gravity = context;
    p->velocityY += *gravity * p->age;
}

float gravity = 200.0f;
psSetInfluence(ps, &gravity, applyGravity);
```

<br>

| `void (*SnapshotInfluenceFn)(Particle *p, const Particle *snapshot, int count, void *context)` |
|------------------------------------------------------------------------------------------------|

Callback that modifies a particle using a pre-update snapshot of all active
particles.

- Parameters:
    - `p` — Pointer to the particle being modified.
    - `snapshot` — Read-only array of all active particles captured before
      this update step began.
    - `count` — Number of particles in `snapshot`.
    - `context` — Optional caller-supplied pointer forwarded from
      `psSetSnapshotInfluence`.

- Notes:
    - The snapshot is taken once at the start of `psUpdate` and is consistent
      across all particles in that frame; modifying `p` does not affect
      `snapshot`.
    - Use this callback when particle behavior depends on the positions or
      states of other particles (e.g., repulsion, flocking, neighbor
      averaging).
    - Only one snapshot influence can be registered per instance at a time;
      passing `nullptr` as `fn` removes the current one.

✅ Example

```c
static void repel(Particle *p, const Particle *snapshot, int count, void *context)
{
    (void)context;
    for (int i = 0; i < count; i++)
    {
        float dx = p->x - snapshot[i].x;
        float dy = p->y - snapshot[i].y;
        float dist = dx * dx + dy * dy;
        if (dist > 0.0f && dist < 400.0f)
        {
            p->velocityX += dx * 0.5f;
            p->velocityY += dy * 0.5f;
        }
    }
}

psSetSnapshotInfluence(ps, nullptr, repel);
```

<br>

---

## 🛠️ Functions

### — Lifecycle

| `ParticleSystem *psCreate(int maxParticles, float originX, float originY)` |
|----------------------------------------------------------------------------|

Allocates and initializes a new ParticleSystem with the given capacity and
origin.

- Parameters:
    - `maxParticles` — Maximum number of particles the system can hold.
    - `originX` — X coordinate of the emission origin.
    - `originY` — Y coordinate of the emission origin.

- Returns: Pointer to the new ParticleSystem (caller-owned), or `nullptr` on
  failure.

- Notes:
    - Fails if `maxParticles` is less than or equal to zero, or if memory
      allocation fails.
    - The returned instance must be freed with `psDestroy()`.

✅ Example

```c
ParticleSystem *ps = psCreate(500, 400.0f, 300.0f);
if (!ps)
{
    // Handle allocation failure.
}
```

<br>

| `int psDestroy(ParticleSystem *ps)` |
|-------------------------------------|

Frees all memory associated with the ParticleSystem.

- Parameters:
    - `ps` — ParticleSystem to destroy.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`.
    - After this call the pointer is invalid; set it to `nullptr` to avoid
      use-after-free.

✅ Example

```c
psDestroy(ps);
ps = nullptr;
```

<br>

| `int psReset(ParticleSystem *ps)` |
|-----------------------------------|

Deactivates all particles and resets streaming state without freeing the
system.

- Parameters:
    - `ps` — ParticleSystem to reset.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`.
    - The capacity, origin, and configuration (velocity, lifetime, shape,
      etc.) are preserved; only the live particle pool and streaming
      accumulator are cleared.
    - Use this to restart an effect without reallocating the instance.

✅ Example

```c
// Restart the effect from scratch without allocating a new instance.
psReset(ps);
psBurst(ps, 50);
```

<br>

### — Emission

| `int psBurst(ParticleSystem *ps, int amount)` |
|-----------------------------------------------|

Emits a fixed number of particles immediately.

- Parameters:
    - `ps` — ParticleSystem to emit from.
    - `amount` — Number of particles to emit.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr` or `amount` is less than or equal to zero.
    - If fewer idle slots are available than `amount`, only as many particles
      as there are idle slots are emitted; this is not treated as a failure.

✅ Example

```c
// Emit 30 particles on impact.
psBurst(ps, 30);
```

<br>

| `int psStream(ParticleSystem *ps, float rate)` |
|------------------------------------------------|

Sets the continuous emission rate in particles per second; pass `0` to stop
streaming.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `rate` — Emission rate in particles per second. Pass `0.0f` to stop.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr` or `rate` is negative.
    - Fractional particles accumulate across frames; the system emits a whole
      particle only once the accumulator reaches `1.0`.
    - Calling `psReset()` clears the accumulator and stops streaming.

✅ Example

```c
// Begin streaming at 60 particles per second.
psStream(ps, 60.0f);

// Stop streaming.
psStream(ps, 0.0f);
```

<br>

### — Update / Query

| `int psUpdate(ParticleSystem *ps, float dt)` |
|----------------------------------------------|

Advances the simulation by `dt` seconds, ageing particles, culling expired
ones, and emitting streamed ones.

- Parameters:
    - `ps` — ParticleSystem to update.
    - `dt` — Delta time in seconds since the last update.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr` or `dt` is negative.
    - Influence callbacks (registered via `psSetInfluence` and
      `psSetSnapshotInfluence`) are applied before position integration.
    - Call once per frame, before `psForEach`.

✅ Example

```c
while (!WindowShouldClose())
{
    float dt = GetFrameTime();
    psUpdate(ps, dt);

    BeginDrawing();
    psForEach(ps, drawParticle, nullptr);
    EndDrawing();
}
```

<br>

| `int psForEach(const ParticleSystem *ps, ParticleFn fn, void *context)` |
|-------------------------------------------------------------------------|

Iterates over all active particles and invokes `fn` for each one.

- Parameters:
    - `ps` — ParticleSystem to iterate.
    - `fn` — Callback invoked for each active particle.
    - `context` — Optional caller-supplied pointer passed to `fn`.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` or `fn` is `nullptr`.
    - The iteration order is unspecified.
    - The callback must not add or remove particles (i.e., must not call
      `psBurst`, `psStream`, or `psReset` on the same instance) during
      iteration.

✅ Example

```c
static void drawParticle(const Particle *p, void *context)
{
    (void)context;
    DrawCircleV((Vector2){ p->x, p->y }, 4.0f, WHITE);
}

psForEach(ps, drawParticle, nullptr);
```

<br>

### — Getters

| `float psGetX(const ParticleSystem *ps)` |
|------------------------------------------|

Returns the X coordinate of the emission origin.

- Parameters:
    - `ps` — ParticleSystem to query.

- Returns: X coordinate of the emission origin.

✅ Example

```c
float x = psGetX(ps);
```

<br>

| `float psGetY(const ParticleSystem *ps)` |
|------------------------------------------|

Returns the Y coordinate of the emission origin.

- Parameters:
    - `ps` — ParticleSystem to query.

- Returns: Y coordinate of the emission origin.

✅ Example

```c
float y = psGetY(ps);
```

<br>

| `int psGetActive(const ParticleSystem *ps)` |
|---------------------------------------------|

Returns the number of currently active particles.

- Parameters:
    - `ps` — ParticleSystem to query.

- Returns: Number of active particles, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`.

✅ Example

```c
int active = psGetActive(ps);
```

<br>

| `int psGetIdle(const ParticleSystem *ps)` |
|-------------------------------------------|

Returns the number of currently idle particle slots.

- Parameters:
    - `ps` — ParticleSystem to query.

- Returns: Number of idle particle slots, or a negative result code on
  failure.

- Notes:
    - Fails if `ps` is `nullptr`.
    - `psGetActive(ps) + psGetIdle(ps)` always equals `maxParticles`.

✅ Example

```c
int idle = psGetIdle(ps);
```

<br>

### — Setters

| `int psSetOrigin(ParticleSystem *ps, float x, float y)` |
|----------------------------------------------------------|

Sets the emission origin to the given coordinates.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `x` — New X coordinate of the origin.
    - `y` — New Y coordinate of the origin.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`.
    - Already-active particles are not affected; only newly emitted particles
      use the updated origin.

✅ Example

```c
// Follow the mouse cursor.
psSetOrigin(ps, GetMouseX(), GetMouseY());
```

<br>

| `int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY)` |
|-----------------------------------------------------------------------------------------------|

Configures the inner and outer emission spread around the origin.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `innerX` — Inner spread half-extent on X; particles are not emitted
      within this boundary.
    - `innerY` — Inner spread half-extent on Y; particles are not emitted
      within this boundary.
    - `outerX` — Outer spread half-extent on X; particles are not emitted
      beyond this boundary.
    - `outerY` — Outer spread half-extent on Y; particles are not emitted
      beyond this boundary.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`, any extent is negative, or an inner extent
      exceeds its corresponding outer extent.
    - Setting inner extents to `0` emits particles anywhere within the outer
      boundary, including at the origin.
    - The shape of the boundary is controlled by `psSetEmissionShape`.

✅ Example

```c
// Emit in a ring: avoid the centre 10 px, spread up to 40 px.
psSetSpread(ps, 10.0f, 10.0f, 40.0f, 40.0f);
```

<br>

| `int psSetVelocity(ParticleSystem *ps, float minX, float minY, float maxX, float maxY)` |
|-----------------------------------------------------------------------------------------|

Sets the random velocity range assigned to each newly emitted particle.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `minX` — Minimum X velocity.
    - `minY` — Minimum Y velocity.
    - `maxX` — Maximum X velocity.
    - `maxY` — Maximum Y velocity.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`, or if `minX > maxX` or `minY > maxY`.
    - Each emitted particle receives a velocity sampled uniformly at random
      from `[minX, maxX]` × `[minY, maxY]`.

✅ Example

```c
// Shoot particles upward with a slight horizontal spread.
psSetVelocity(ps, -20.0f, -150.0f, 20.0f, -80.0f);
```

<br>

| `int psSetAcceleration(ParticleSystem *ps, float minX, float minY, float maxX, float maxY)` |
|---------------------------------------------------------------------------------------------|

Sets the random acceleration range assigned to each newly emitted particle.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `minX` — Minimum X acceleration.
    - `minY` — Minimum Y acceleration.
    - `maxX` — Maximum X acceleration.
    - `maxY` — Maximum Y acceleration.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`, or if `minX > maxX` or `minY > maxY`.
    - Each emitted particle receives an acceleration sampled uniformly at
      random from `[minX, maxX]` × `[minY, maxY]`.

✅ Example

```c
// Apply a downward gravitational pull.
psSetAcceleration(ps, 0.0f, 200.0f, 0.0f, 200.0f);
```

<br>

| `int psSetLifetime(ParticleSystem *ps, float min, float max)` |
|---------------------------------------------------------------|

Sets the random lifetime range assigned to each newly emitted particle.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `min` — Minimum lifetime in seconds.
    - `max` — Maximum lifetime in seconds.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`, `min` is negative, or `min > max`.
    - Each emitted particle receives a lifetime sampled uniformly at random
      from `[min, max]`.

✅ Example

```c
// Particles live between half a second and two seconds.
psSetLifetime(ps, 0.5f, 2.0f);
```

<br>

| `int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape)` |
|---------------------------------------------------------------------|

Sets the geometric shape used to sample particle spawn positions.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `shape` — Emission shape to use (`PS_SHAPE_ELLIPSE` or `PS_SHAPE_RECT`).

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr` or `shape` is not a valid `psEmissionShape`
      value.
    - The shape applies to the spread region defined by `psSetSpread`.
    - Defaults to `PS_SHAPE_ELLIPSE`.

✅ Example

```c
psSetEmissionShape(ps, PS_SHAPE_RECT);
```

<br>

| `int psSetInfluence(ParticleSystem *ps, void *context, InfluenceFn fn)` |
|-------------------------------------------------------------------------|

Registers a per-particle callback applied to every active particle each
update.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `context` — Optional caller-supplied pointer passed to `fn` on each
      call.
    - `fn` — Influence callback; pass `nullptr` to remove the current
      influence.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`.
    - Only one influence callback can be registered per instance at a time;
      calling this function again replaces the previous one.
    - The callback is applied during `psUpdate`, before position integration.

✅ Example

```c
static void applyGravity(Particle *p, void *context)
{
    float *gravity = context;
    p->velocityY += *gravity * p->age;
}

float gravity = 200.0f;
psSetInfluence(ps, &gravity, applyGravity);

// Remove the influence.
psSetInfluence(ps, nullptr, nullptr);
```

<br>

| `int psSetSnapshotInfluence(ParticleSystem *ps, void *context, SnapshotInfluenceFn fn)` |
|-----------------------------------------------------------------------------------------|

Registers a snapshot-based influence callback applied to every active particle
each update.

- Parameters:
    - `ps` — ParticleSystem to configure.
    - `context` — Optional caller-supplied pointer passed to `fn` on each
      call.
    - `fn` — Snapshot influence callback; pass `nullptr` to remove the
      current influence.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Fails if `ps` is `nullptr`.
    - Only one snapshot influence callback can be registered per instance at
      a time; calling this function again replaces the previous one.
    - A snapshot of all active particles is taken once at the start of
      `psUpdate`; the callback receives the same snapshot for every particle
      in that frame.
    - The snapshot influence is applied after the regular influence (if both
      are registered).

✅ Example

```c
static void repel(Particle *p, const Particle *snapshot, int count, void *context)
{
    (void)context;
    for (int i = 0; i < count; i++)
    {
        float dx = p->x - snapshot[i].x;
        float dy = p->y - snapshot[i].y;
        float dist = dx * dx + dy * dy;
        if (dist > 0.0f && dist < 400.0f)
        {
            p->velocityX += dx * 0.5f;
            p->velocityY += dy * 0.5f;
        }
    }
}

psSetSnapshotInfluence(ps, nullptr, repel);

// Remove the snapshot influence.
psSetSnapshotInfluence(ps, nullptr, nullptr);
```