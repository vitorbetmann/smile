#pragma once

// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include "ParticleSystem.h"

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

typedef struct
{
    psEmissionShape shape;

    float innerSpreadX, innerSpreadY;
    float outerSpreadX, outerSpreadY;
} psInternalEmissionArea;

typedef struct psInternalParticleSystem
{
    int maxParticles, activeParticles;

    float originX, originY;

    float minVelocityX, minVelocityY;
    float maxVelocityX, maxVelocityY;

    float minAccelerationX, minAccelerationY;
    float maxAccelerationX, maxAccelerationY;

    float minLifetime, maxLifetime;

    float streamRate;
    float streamAccumulator;

    psInternalEmissionArea emissionArea;

    Particle particles[];
} psInternalParticleSystem;
