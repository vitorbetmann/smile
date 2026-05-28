#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

typedef struct
{
    float x, y;
    float velocityX, velocityY;
    float accelerationX, accelerationY;
    float age;
    float lifetime;
} Particle;

typedef enum
{
    PS_SHAPE_ELLIPSE,
    PS_SHAPE_RECT,
    SHAPE_COUNT
} psEmissionShape;

typedef struct psInternalParticleSystem ParticleSystem;

typedef void (*ParticleFn)(const Particle *p, void *context);

typedef void (*InfluenceFn)(Particle *p, void *context);

typedef void (*SnapshotInfluenceFn)(Particle *p, const Particle *snapshot, int count,
                                    void *context);

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

// Lifecycle

ParticleSystem *psCreate(int maxParticles, float originX, float originY);

int psDestroy(ParticleSystem *ps);

int psReset(ParticleSystem *ps);

// Emission

int psBurst(ParticleSystem *ps, int amount);

int psStream(ParticleSystem *ps, float rate);

// Update / Query

int psUpdate(ParticleSystem *ps, float dt);

int psForEach(const ParticleSystem *ps, ParticleFn fn, void *context);

// Getters

// -- Position

float psGetX(const ParticleSystem *ps);

float psGetY(const ParticleSystem *ps);

// -- Count

int psGetActive(const ParticleSystem *ps);

int psGetIdle(const ParticleSystem *ps);

// Setters

// -- Position

int psSetOrigin(ParticleSystem *ps, float x, float y);

int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY);

// -- Movement

int psSetVelocity(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);

int psSetAcceleration(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);

// -- Lifetime

int psSetLifetime(ParticleSystem *ps, float min, float max);

// -- Shape

int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape);

// -- Influence

int psSetInfluence(ParticleSystem *ps, void *context, InfluenceFn fn);

int psSetSnapshotInfluence(ParticleSystem *ps, void *context, SnapshotInfluenceFn fn);
