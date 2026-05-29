# ParticleSystem — Getting Started ✨

The `ParticleSystem` module provides a configurable, instance-based particle
simulation for 2D game effects such as explosions, fire, rain, and trails.
Each instance manages its own pool of particles and exposes callbacks for
custom per-particle behavior.

Module contract:

- `psCreate()` must succeed before any other ParticleSystem API is called on
  that instance.
- Always call `psDestroy()` when an instance is no longer needed to prevent
  memory leaks.

For detailed documentation see: [ParticleSystem – API](ParticleSystemAPI.md)

### 🚨 Warning! This module is not thread-safe!
---

## 📋 Table of Contents

- [Visual Example](#-visual-example)
- [Module Header](#-module-header)
- [ParticleSystem Lifecycle](#-particlesystem-lifecycle)
- [Quick Reference Table](#-quick-reference-table)
- [Workflow Example](#-workflow-example)

---

## 👀 Visual Example

<p align="center">
  <img src="https://raw.githubusercontent.com/vitorbetmann/smile-assets/refs/heads/main/ParticleSystem/ParticleSystemDemo.gif" width="90%" alt="GIF of ParticleSystem module working"/>
</p>

---

## 😊 Module Header

The module's header is `ParticleSystem.h`. Its full Smile path is:
`smile/include/ParticleSystem.h`

✅ Example

```c
#include <ParticleSystem.h>
```

---

## 🔄 ParticleSystem Lifecycle

1️⃣ Create an instance with `psCreate()`, passing the maximum number of
particles and the emission origin. The returned pointer is caller-owned —
multiple independent instances can coexist.

2️⃣ Configure the instance using the `psSet*` family of functions. You can
control the emission spread, velocity and acceleration ranges, particle
lifetime, emission shape, and per-particle influence callbacks.

3️⃣ Emit particles with `psBurst()` for an immediate one-shot burst, or
`psStream()` for a continuous rate that feeds particles every frame.

4️⃣ Call `psUpdate()` once per frame (typically inside your game loop) with
the elapsed delta time. This ages particles, culls expired ones, and emits
streamed ones.

5️⃣ Use `psForEach()` to iterate over all active particles and render them.
You supply a `ParticleFn` callback and an optional context pointer; the
callback receives each `Particle` in turn.

6️⃣ Always call `psDestroy()` when the instance is no longer needed. This
frees all memory and invalidates the pointer.

---

## 🔍 Quick Reference Table

### Data Types

— Structs

| Signature  | Description                                                                                                            |
|------------|------------------------------------------------------------------------------------------------------------------------|
| `Particle` | Represents a single active particle with position, velocity, acceleration, age, lifetime, angle, and angular velocity. |

— Enums

| Signature         | Description                                                                                    |
|-------------------|------------------------------------------------------------------------------------------------|
| `psEmissionShape` | Geometric shape used to sample particle spawn positions (`PS_SHAPE_ELLIPSE`, `PS_SHAPE_RECT`). |

— Function Pointers

| Signature                                                                                      | Description                                                                            |
|------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------|
| `void (*ParticleFn)(const Particle *p, void *context)`                                         | Callback invoked for each active particle during `psForEach`.                          |
| `void (*InfluenceFn)(Particle *p, void *context)`                                              | Callback that modifies a particle once per update step.                                |
| `void (*SnapshotInfluenceFn)(Particle *p, const Particle *snapshot, int count, void *context)` | Callback that modifies a particle using a pre-update snapshot of all active particles. |

<br>

### Functions

| Signature                                                                                     | Description                                                                                                                         |
|-----------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| `ParticleSystem *psCreate(int maxParticles, float originX, float originY)`                    | Allocates and initializes a new ParticleSystem.                                                                                     |
| `int psDestroy(ParticleSystem *ps)`                                                           | Frees all memory associated with the ParticleSystem.                                                                                |
| `int psReset(ParticleSystem *ps)`                                                             | Deactivates all particles without freeing the instance or clearing configuration.                                                   |
| `int psBurst(ParticleSystem *ps, int amount)`                                                 | Emits a fixed number of particles immediately.                                                                                      |
| `int psStream(ParticleSystem *ps, float rate)`                                                | Sets the continuous emission rate in particles per second.                                                                          |
| `int psUpdate(ParticleSystem *ps, float dt)`                                                  | Advances the simulation by dt seconds.                                                                                              |
| `int psForEach(const ParticleSystem *ps, ParticleFn fn, void *context)`                       | Iterates over all active particles and invokes fn for each one.                                                                     |
| `float psGetX(const ParticleSystem *ps)`                                                      | Returns the X coordinate of the emission origin.                                                                                    |
| `float psGetY(const ParticleSystem *ps)`                                                      | Returns the Y coordinate of the emission origin.                                                                                    |
| `int psGetActive(const ParticleSystem *ps)`                                                   | Returns the number of currently active particles.                                                                                   |
| `int psGetIdle(const ParticleSystem *ps)`                                                     | Returns the number of currently idle particle slots.                                                                                |
| `int psSetOrigin(ParticleSystem *ps, float x, float y)`                                       | Sets the emission origin to the given coordinates.                                                                                  |
| `int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY)` | Configures the inner and outer emission spread around the origin.                                                                   |
| `int psSetVelocity(ParticleSystem *ps, float minX, float minY, float maxX, float maxY)`       | Sets the random velocity range assigned to each newly emitted particle.                                                             |
| `int psSetAcceleration(ParticleSystem *ps, float minX, float minY, float maxX, float maxY)`   | Sets the random acceleration range assigned to each newly emitted particle.                                                         |
| `int psSetAngularVelocity(ParticleSystem *ps, float min, float max)`                          | Sets the random angular velocity range (radians/sec) assigned to each newly emitted particle. Defaults to `0.0f`.                   |
| `int psSetLifetime(ParticleSystem *ps, float min, float max)`                                 | Sets the random lifetime range assigned to each newly emitted particle. Pass `INFINITY` for immortal particles. Defaults to `1.0f`. |
| `int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape)`                           | Sets the geometric shape used to sample particle spawn positions.                                                                   |
| `int psSetInfluence(ParticleSystem *ps, void *context, InfluenceFn fn)`                       | Registers a per-particle callback applied each update.                                                                              |
| `int psSetSnapshotInfluence(ParticleSystem *ps, void *context, SnapshotInfluenceFn fn)`       | Registers a snapshot-based influence callback applied each update.                                                                  |

---

## 🧪️ Workflow Example

The example below shows a simple fire effect: particles stream upward from a
fixed point, rendered each frame using raylib.

```c
#include <ParticleSystem.h>
#include <raylib.h>

static void drawParticle(const Particle *p, void *context)
{
    (void)context;

    // Fade out as the particle ages.
    float alpha = 1.0f - (p->age / p->lifetime);
    Color color = { 255, (unsigned char)(100 * alpha), 0, (unsigned char)(255 * alpha) };

    DrawCircleV((Vector2){ p->x, p->y }, 4.0f, color);
}

int main(void)
{
    InitWindow(800, 600, "Fire Effect");

    // Create a system with up to 200 particles at the centre-bottom of the screen.
    ParticleSystem *fire = psCreate(200, 400.0f, 550.0f);

    // Spread slightly around the origin, launch upward, short-lived.
    psSetSpread(fire, 0.0f, 0.0f, 20.0f, 5.0f);
    psSetVelocity(fire, -15.0f, -120.0f, 15.0f, -60.0f);
    psSetLifetime(fire, 0.5f, 1.2f);

    // Stream 80 particles per second continuously.
    psStream(fire, 80.0f);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        psUpdate(fire, dt);

        BeginDrawing();
        ClearBackground(BLACK);
        psForEach(fire, drawParticle, nullptr);
        EndDrawing();
    }

    psDestroy(fire);
    CloseWindow();
    return 0;
}
```

To move the emitter each frame — for example, following the mouse pointer —
update the origin inside the game loop:

```c
while (!WindowShouldClose())
{
    Vector2 mouse = GetMousePosition();
    psSetOrigin(fire, mouse.x, mouse.y);

    float dt = GetFrameTime();
    psUpdate(fire, dt);

    BeginDrawing();
    ClearBackground(BLACK);
    psForEach(fire, drawParticle, nullptr);
    EndDrawing();
}
```

To apply custom physics — for example, gravity — register an influence
callback before the loop:

```c
static void applyGravity(Particle *p, void *context)
{
    float *gravity = context;
    p->velocityY += *gravity * p->age;
}

float gravity = 200.0f;
psSetInfluence(fire, &gravity, applyGravity);
```

To react to other particles (e.g., repulsion), use a snapshot influence:

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

psSetSnapshotInfluence(fire, nullptr, repel);
```

Call `psDestroy()` when the effect is done to free the instance:

```c
psDestroy(fire);
fire = nullptr;
```