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
    float minVelocityX, maxVelocityX, minVelocityY, maxVelocityY;
    float minAccelerationX, maxAccelerationX, minAccelerationY, maxAccelerationY;
    float minLifetime, maxLifetime;
    psInternalEmissionArea emissionArea;
    Particle particles[];
} psInternalParticleSystem;
