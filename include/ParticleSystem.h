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
    PS_SHAPE_POINT,
    PS_SHAPE_ELLIPSE,
    PS_SHAPE_RECT,
} psEmissionShape;

typedef struct psInternalParticleSystem ParticleSystem;

typedef void (*ParticleFn)(const Particle *p, void *args);
typedef void (*SnapshotInfluenceFn)(Particle *p, const Particle *snapshot, int count);
typedef void (*InfluenceFn)(Particle *p, void *context);

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

// Lifecycle

ParticleSystem *psCreate(int maxParticles, float x, float y);
int psDestroy(ParticleSystem *ps);
int psReset(ParticleSystem *ps);
int psEmit(ParticleSystem *ps, int count);
int psUpdate(ParticleSystem *ps, float dt);
int psForEach(ParticleSystem *ps, ParticleFn fn, void *args);

// Getters

// -- Position

float psGetX(ParticleSystem *ps);
float psGetY(ParticleSystem *ps);

// -- Count

int psGetActive(ParticleSystem *ps);
int psGetIdle(ParticleSystem *ps);

// Setters

// -- Position

int psSetOrigin(ParticleSystem *ps, float x, float y);
int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY);

// -- Movement

int psSetVelocity(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);
int psSetAcceleration(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);

// -- Lifetime

int psSetLifetime(ParticleSystem *ps, float min, float max);
int psSetEvent(ParticleSystem *ps, float threshold, ParticleFn fn, void *context);

// -- Shape

int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape);

// -- Influence

int psSetInfluence(ParticleSystem *ps, void *context, InfluenceFn fn);
int psSetSnapshotInfluence(ParticleSystem *ps, SnapshotInfluenceFn fn);
