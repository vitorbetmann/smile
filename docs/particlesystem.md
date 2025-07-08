# Particle System Module 💨

Welcome to the Particle System module, your go-to solution for adding dynamic and captivating particle effects to your SMILE projects. Whether you're aiming to simulate swirling smoke, flickering flames, or explosive bursts, this module provides a powerful yet easy-to-use framework to bring your visuals to life.

## Features

Integrating seamlessly with SMILE states, this module offers comprehensive support for a variety of particle attributes, including position, velocity, color, and lifetime. By leveraging raylib's robust types such as `Vector2`, `Color`, and `Texture2D`, it ensures smooth rendering and efficient vector operations, making your particle effects both visually stunning and performant.

## Example Usage

Here's a simple example that walks you through initializing, updating, and drawing a particle system within a SMILE state using C:

```c
#include "ParticleSystem.h"
#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "Particle System Example");

    Texture2D particleTexture = LoadTexture("particle.png");

    ParticleSystem *ps = newParticleSystem(&particleTexture, 100, (Vector2){400, 300});
    PS_SetParticleLifetime(ps, 500, 1500); // Lifetime in milliseconds
    PS_SetLinearAcceleration(ps, -50, -50, 50, 50);
    PS_SetEmissionArea(ps, UNIFORM, 100, 100); // Emission can be UNIFORM or NORMAL
    PS_SetUniformDist(ps, (Vector2){16, 16}, 10);
    PS_SetColors(ps, RED, GREEN);
    PS_Emit(ps);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        PS_Update(ps, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        PS_Draw(ps);
        EndDrawing();

        if (PS_ShouldDestroy(ps)) {
            break;
        }
    }

    UnloadParticleSystem(ps);
    UnloadTexture(particleTexture);

    CloseWindow();

    return 0;
}
```

## API Overview

- `ParticleSystem* ParticleSystem_New(Texture2D texture, int maxParticles)`: Initializes a new particle system ready to generate effects.
- `void ParticleSystem_Emit(ParticleSystem* ps, Vector2 position, Vector2 velocity, Color color)`: Adds new particles to the system with specified attributes.
- `void ParticleSystem_Update(ParticleSystem* ps, float dt)`: Advances the state of all particles based on elapsed time.
- `void ParticleSystem_Draw(ParticleSystem* ps)`: Renders the current particles onto the screen.

## Integration Notes

This particle system is built upon raylib's foundational types like `Vector2`, `Color`, and `Texture2D`, so ensure raylib is properly linked in your project to enjoy full functionality. Textures used with the system should be loaded and managed in accordance with raylib's resource handling guidelines. Designed with SMILE states in mind, the system fits naturally into the lifecycle methods such as `init`, `update`, and `draw`, allowing for smooth and organized integration.

# Particle System Module

## Features

This module empowers you to craft particle systems tailored to your needs, allowing you to specify the number of particles and their texture with ease. You can define how long particles live and control their acceleration, giving you fine-grained control over their motion. The emission area can be customized using either uniform or normal distribution, letting you shape how particles spread out. Particle size and uniform distribution columns are adjustable to fit your visual style. Additionally, you can set both initial and final colors, with smooth automatic color interpolation over each particle's lifetime. The module handles emission, updates, and rendering, and even provides a way to check when the system has completed its lifecycle, ensuring efficient memory management through its unloading function.

## Example Usage

Take a look at this example that demonstrates how to set up and run a particle system:
