// TODO document this

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
// --------------------------------------------------
// Includes
// --------------------------------------------------
#include <raylib.h>

// --------------------------------------------------
// Data types
// --------------------------------------------------
typedef enum {
  UNIFORM,
  NORMAL,
} Distribution;

typedef struct ParticleSystem ParticleSystem;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 *
 **/
ParticleSystem *newParticleSystem(Texture2D *texture, int particles,
                                  Vector2 pos);

/**
 *
 **/
void PS_SetParticleLifetime(ParticleSystem *ps, int min, int max);

/**
 *
 **/
void PS_SetLinearAcceleration(ParticleSystem *ps, float xMin, float yMin,
                              float xMax, float yMax);

/**
 *
 **/
void PS_SetEmissionArea(ParticleSystem *ps, Distribution dist, float dx,
                        float dy);

/**
 *
 **/
void PS_SetUniformDist(ParticleSystem *ps, Vector2 particleSize, int colsCount);

/**
 *
 **/
void PS_SetColors(ParticleSystem *ps, Color color1, Color color2);

/**
 *
 **/
void PS_Emit(ParticleSystem *ps);

/**
 *
 **/
void PS_Update(ParticleSystem *ps, float dt);

/**
 *
 **/
void PS_Draw(ParticleSystem *ps);

/**
 *
 **/
bool PS_ShouldDestroy(ParticleSystem *ps);

/**
 *
 **/
void PS_Unload(ParticleSystem *ps);

#endif