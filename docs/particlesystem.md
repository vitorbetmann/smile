# Welcome to the ParticleSystem Module 💨

The ParticleSystem module lets you easily spawn animated, colored particles with lifetimes, positions, and movement. It's perfect for adding visual effects like sparks, trails, bursts, or explosions to your game or application.

---

# 🎮 How to Use the Particle System

To use the ParticleSystem:

1. **Configure your system**: Use the `PS_Set...` functions to set particle properties, such as color, lifetime, speed, and spread.
2. **Emit particles**: Call the appropriate function to emit or spawn particles when you need effects (e.g., on an explosion or player action).
3. **Update and draw**: In your game loop, call the update and draw functions to animate and render the particles.

The module uses raylib types like `Vector2` and `Color`, so make sure your project is linked with raylib.

For function documentation, see:

```c
// Example Usage

#include <raylib.h>
#include "ParticleSystem.h"

int main(void) {
    // Init window and all...

    ParticleSystem *ps = newParticleSystem();
    PS_SetColors(ps, (Color){255, 0, 0, 255}, (Color){0, 255, 0, 0});
    PS_SetParticleLifetime(ps, 1000, 2000); // time in miliseconds
    PS_SetLinearAcceleration(ps, -50, -50, 50, 50);
    PS_SetEmissionArea(ps, NORMAL, 10, 10);

    while (!WindowShouldClose()) {
        PS_Emit(ps);
        PS_Update(ps);
        // Begin Drawing...
        PS_Draw(ps);
        // EndDrawing...

        if (PS_ShouldDestroy(ps)){
            break;
        }
    }

    PS_Unload(ps);
    // Unload more stuff, close window, return 0...
}
```
