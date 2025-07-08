// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "ParticleSystem.h"
#include "raylib.h"
#include "stdio.h"
#include <raymath.h>
#include <stdlib.h>
// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef struct {
  Vector2 pos;
  float dx, dy;
  Vector2 size;
  float lifeTime, initialLifeTime;
  float linearAccelerationX, linearAccelerationY;
  float spawnDistanceX, spawnDistanceY;
  Texture2D *texture;
  Color initialColor, currColor, finalColor;
  Color colorDelta;
} Particle;

struct ParticleSystem {
  Vector2 pos;
  Vector2 particleSize;
  Texture2D *texture;
  int particleCount;
  Particle *particles;
  float minLifetime, maxLifetime, elapsedTime;
  int minLinearAccelerationX, maxLinearAccelerationX;
  int minLinearAccelerationY, maxLinearAccelerationY;
  unsigned int maxSpawnDistanceX, maxSpawnDistanceY;
  Distribution distribution;
  int uniformCols;
  bool canEmit, shouldDestroy;
  Color initialColor, finalColor, colorDelta;
};

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void ParticleUpdate(Particle *p, float dt);
void PS_Draw(ParticleSystem *ps);
void ParticleDraw(Particle *p);

// --------------------------------------------------
// Functions
// --------------------------------------------------
ParticleSystem *newParticleSystem(Texture2D *texture, int particleCount,
                                  Vector2 pos) {

  ParticleSystem *ps = calloc(1, sizeof(ParticleSystem));
  if (!ps) {
    return NULL;
  }

  ps->texture = texture;

  ps->particleCount = particleCount;
  ps->particles = calloc(ps->particleCount, sizeof(Particle));
  if (!ps->particles) {
    return NULL;
  }

  ps->pos = pos;

  ps->minLifetime = ps->maxLifetime = 1.0f;

  ps->distribution = UNIFORM;

  ps->initialColor = (Color){255, 0, 0, 255};
  ps->finalColor = (Color){0, 255, 0, 255};

  return ps;
}

void PS_SetParticleLifetime(ParticleSystem *ps, int min, int max) {

  ps->minLifetime = min;
  ps->maxLifetime = max;
}

void PS_SetLinearAcceleration(ParticleSystem *ps, float xMin, float yMin,
                              float xMax, float yMax) {

  ps->minLinearAccelerationX = xMin;
  ps->minLinearAccelerationY = yMin;
  ps->maxLinearAccelerationX = xMax;
  ps->maxLinearAccelerationY = yMax;
}

void PS_SetEmissionArea(ParticleSystem *ps, Distribution dist, float dx,
                        float dy) {

  ps->distribution = dist;
  ps->maxSpawnDistanceX = dx;
  ps->maxSpawnDistanceY = dy;
}

void PS_SetUniformDist(ParticleSystem *ps, Vector2 particleSize,
                       int colsCount) {
  ps->distribution = UNIFORM;
  ps->particleSize = particleSize;
  ps->uniformCols = colsCount;
}

void PS_SetColors(ParticleSystem *ps, Color color1, Color color2) {

  ps->initialColor = color1;
  ps->finalColor = color2;

  ps->colorDelta = (Color){ps->finalColor.r - ps->initialColor.r,
                           ps->finalColor.g - ps->initialColor.g,
                           ps->finalColor.b - ps->initialColor.b,
                           ps->finalColor.a - ps->initialColor.a};
}

void PS_Emit(ParticleSystem *ps) {
  int uniformRows = 0, uniformCols = 0;

  for (int i = 0; i < ps->particleCount; i++) {
    Particle *temp = &ps->particles[i];

    // Texture
    temp->texture = ps->texture;

    // Size
    temp->size = ps->particleSize;

    // Lifetime
    temp->lifeTime = GetRandomValue(ps->minLifetime, ps->maxLifetime) / 1000.0f;
    temp->initialLifeTime = temp->lifeTime;

    switch (ps->distribution) {
    case UNIFORM:
      temp->pos.x = ps->pos.x + (uniformCols * temp->size.x);
      uniformCols++;
      if (uniformCols == ps->uniformCols) {
        uniformCols = 0;
        uniformRows++;
      }
      temp->pos.y = ps->pos.y + (uniformRows * temp->size.y);
      break;
    case NORMAL:
      // Position
      temp->pos.x = GetRandomValue(0, ps->maxSpawnDistanceX);
      temp->pos.x *= GetRandomValue(0, 1) == 0 ? 1 : -1;
      temp->pos.x += ps->pos.x;

      temp->pos.y = GetRandomValue(0, ps->maxSpawnDistanceY);
      temp->pos.y *= GetRandomValue(0, 1) == 0 ? 1 : -1;
      temp->pos.y += ps->pos.y;
      break;
    }

    // Velocity
    temp->dx = temp->dy = 0.0f;

    // Acceleration
    temp->linearAccelerationX =
        GetRandomValue(ps->minLinearAccelerationX, ps->maxLinearAccelerationX);
    temp->linearAccelerationY =
        GetRandomValue(ps->minLinearAccelerationY, ps->maxLinearAccelerationY);

    // Color
    temp->initialColor = ps->initialColor;
    temp->currColor = ps->initialColor;
    temp->finalColor = ps->finalColor;
    temp->colorDelta = ps->colorDelta;
  }

  ps->canEmit = true;
}

void PS_Update(ParticleSystem *ps, float dt) {

  if (!ps || !ps->canEmit) {
    return;
  }

  ps->elapsedTime += dt * 1000;
  if (ps->elapsedTime > ps->maxLifetime) {
    ps->canEmit = false;
    ps->shouldDestroy = true;
    return;
  }

  for (int i = 0; i < ps->particleCount; i++) {
    ParticleUpdate(&ps->particles[i], dt);
  }
}

void ParticleUpdate(Particle *p, float dt) {

  // Update speed and pos
  p->dx = p->linearAccelerationX;
  p->dy = p->linearAccelerationY;
  p->pos.x += p->dx * dt;
  p->pos.y += p->dy * dt;

  // Update color
  p->lifeTime -= dt;
  if (p->lifeTime < 0) {
    p->lifeTime = 0;
  }

  float ratio = p->initialLifeTime > 0 ? p->lifeTime / p->initialLifeTime : 0;

  // LERP the hell out of it
  p->currColor.r = Lerp(p->finalColor.r, p->initialColor.r, ratio);
  p->currColor.g = Lerp(p->finalColor.g, p->initialColor.g, ratio);
  p->currColor.b = Lerp(p->finalColor.b, p->initialColor.b, ratio);
  p->currColor.a = Lerp(p->finalColor.a, p->initialColor.a, ratio);
}

void PS_Draw(ParticleSystem *ps) {

  if (!ps->canEmit) {
    return;
  }

  for (int i = 0; i < ps->particleCount; i++) {
    ParticleDraw(&ps->particles[i]);
  }
}

void ParticleDraw(Particle *p) {

  DrawTexture(*p->texture, p->pos.x, p->pos.y, p->currColor);
}

bool PS_ShouldDestroy(ParticleSystem *ps) { return ps->shouldDestroy; }

void PS_Unload(ParticleSystem *ps) {
  if (!ps) {
    return;
  }

  if (ps->particles) {
    free(ps->particles);
  }

  free(ps);
}