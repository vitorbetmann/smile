// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ParticleSystem.h"
#include "ParticleSystemInternal.h"
#include "ParticleSystemMessages.h"

#include "LogInternal.h"
#include "internal/Common/Common.h"
#include "internal/Common/CommonMessages.h"
#include "internal/Test/Test.h"

// Variables ———————————————————————————————————————————————————————————————————————————————————————

static bool isSeedSet;

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

static bool psPrivateIsPsNull(const ParticleSystem *ps, const char *caller);

// Functions - Public ——————————————————————————————————————————————————————————————————————————————

// Lifecycle

ParticleSystem *psCreate(const int maxParticles, const float originX, const float originY)
{
    if (maxParticles <= 0)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_INVALID_ARG, "maxParticles", __func__, CSQ_ABORT);
        return nullptr;
    }

    ParticleSystem *ps = tsCalloc(
        1,
        sizeof(psInternalParticleSystem) + maxParticles * sizeof(Particle)
        );

    if (!ps)
    {
        lgInternalLog(ERROR, ORI, CSE_MEM_ALLOC_FAIL, __func__, CSQ_ABORT);
        return nullptr;
    }

    ps->maxParticles = maxParticles;
    ps->originX = originX;
    ps->originY = originY;

    if (!isSeedSet)
    {
        srand(time(nullptr));
        isSeedSet = true;
    }

    return ps;
}

int psDestroy(ParticleSystem *ps)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    free(ps);
    return RES_OK;
}

int psReset(ParticleSystem *ps)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    ps->activeParticles = 0;
    return RES_OK;
}

// Emission

int psBurst(ParticleSystem *ps, const int amount)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (amount <= 0)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_INVALID_ARG, "amount", __func__, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    const int idleParticles = ps->maxParticles - ps->activeParticles;
    const int oldActive = ps->activeParticles;
    ps->activeParticles += amount > idleParticles ? idleParticles : amount;

    for (int i = oldActive; i < ps->activeParticles; i++)
    {
        Particle *p = &ps->particles[i];
        // Position
        psInternalSamplePosition(ps->emissionArea, ps->originX, ps->originY, &p->x, &p->y);
        // Velocity
        const float velocityDiffX = ps->maxVelocityX - ps->minVelocityX;
        const float velocityDiffY = ps->maxVelocityY - ps->minVelocityY;
        p->velocityX = ps->minVelocityX + (float)rand() / (float)RAND_MAX * velocityDiffX;
        p->velocityY = ps->minVelocityY + (float)rand() / (float)RAND_MAX * velocityDiffY;
        // Acceleration
        const float accelerationDiffX = ps->maxAccelerationX - ps->minAccelerationX;
        const float accelerationDiffY = ps->maxAccelerationY - ps->minAccelerationY;
        p->accelerationX = ps->minAccelerationX + (float)rand() / (float)RAND_MAX *
                           accelerationDiffX;
        p->accelerationY = ps->minAccelerationY + (float)rand() / (float)RAND_MAX *
                           accelerationDiffY;
        // Lifetime
        const float lifetimeDiff = ps->maxLifetime - ps->minLifetime;
        p->lifetime = ps->minLifetime + (float)rand() / (float)RAND_MAX * lifetimeDiff;
    }

    return RES_OK;
}

int psStream(ParticleSystem *ps, const float rate)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (rate < 0)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_INVALID_ARG, "rate", __func__, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    ps->streamRate = rate < (float)ps->maxParticles ? rate : (float)ps->maxParticles;

    return RES_OK;
}

// Update / Query

int psUpdate(ParticleSystem *ps, const float dt)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (dt <= 0.0f)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_INVALID_ARG, "dt", __func__, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    int i = 0;
    while (i < ps->activeParticles)
    {
        Particle *p = &ps->particles[i];
        p->age += dt;
        p->velocityX += p->accelerationX * dt;
        p->velocityY += p->accelerationY * dt;
        p->x += p->velocityX * dt;
        p->y += p->velocityY * dt;

        if (p->age >= p->lifetime)
        {
            ps->activeParticles--;
            ps->particles[i] = ps->particles[ps->activeParticles];
            continue;
        }

        i++;
    }

    // Stream particles
    ps->streamAccumulator += ps->streamRate * dt;
    const int newParticles = (int)ps->streamAccumulator;
    const int toSpawn = newParticles < psGetIdle(ps) ? newParticles : psGetIdle(ps);
    if (toSpawn > 0)
        psBurst(ps, toSpawn);
    ps->streamAccumulator -= (float)toSpawn;

    return RES_OK;
}

int psForEach(const ParticleSystem *ps, const ParticleFn fn, void *context)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (!fn)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_NULL_ARG, "fn", __func__, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    for (int i = 0, active = ps->activeParticles; i < active; i++)
    {
        fn(&ps->particles[i], context);
    }

    return RES_OK;
}

// Getters

// -- Position

float psGetX(const ParticleSystem *ps)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return nanf("");
    }

    return ps->originX;
}

float psGetY(const ParticleSystem *ps)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return nanf("");
    }

    return ps->originY;
}

// -- Count

int psGetActive(const ParticleSystem *ps)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    return ps->activeParticles;
}

int psGetIdle(const ParticleSystem *ps)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    return ps->maxParticles - ps->activeParticles;
}

// Setters

// -- Position

int psSetOrigin(ParticleSystem *ps, const float x, const float y)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    ps->originX = x;
    ps->originY = y;

    return RES_OK;
}

int psSetSpread(ParticleSystem *ps, const float innerX, const float innerY, const float outerX,
                const float outerY)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (innerX > outerX || innerY > outerY)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_NULL_ARG, MSG_INVALID_SPREAD_RANGE, __func__,
                             CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    ps->emissionArea.innerSpreadX = innerX;
    ps->emissionArea.innerSpreadY = innerY;
    ps->emissionArea.outerSpreadX = outerX;
    ps->emissionArea.outerSpreadY = outerY;

    return RES_OK;
}

// -- Movement

int psSetVelocity(ParticleSystem *ps, const float minX, const float minY, const float maxX,
                  const float maxY)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (minX > maxX || minY > maxY)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_NULL_ARG, MSG_INVALID_VELOCITY_RANGE, __func__,
                             CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    ps->minVelocityX = minX;
    ps->maxVelocityX = maxX;
    ps->minVelocityY = minY;
    ps->maxVelocityY = maxY;

    return RES_OK;
}

int psSetAcceleration(ParticleSystem *ps, const float minX, const float minY, const float maxX,
                      const float maxY)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (minX > maxX || minY > maxY)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_NULL_ARG, MSG_INVALID_ACCELERATION_RANGE, __func__,
                             CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    ps->minAccelerationX = minX;
    ps->maxAccelerationX = maxX;
    ps->minAccelerationY = minY;
    ps->maxAccelerationY = maxY;

    return RES_OK;
}

// -- Lifetime

int psSetLifetime(ParticleSystem *ps, const float min, const float max)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (min > max)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_NULL_ARG, MSG_INVALID_LIFETIME_RANGE, __func__,
                             CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    ps->minLifetime = min;
    ps->maxLifetime = max;

    return RES_OK;
}

// -- Shape

int psSetEmissionShape(ParticleSystem *ps, const psEmissionShape shape)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if ((unsigned)shape >= (unsigned)SHAPE_COUNT)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_INVALID_ARG, "shape", __func__, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    ps->emissionArea.shape = shape;
    return RES_OK;
}

// -- Influence

int psSetInfluence(ParticleSystem *ps, void *context, InfluenceFn fn)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    return RES_OK;
}

int psSetSnapshotInfluence(ParticleSystem *ps, void *context, SnapshotInfluenceFn fn)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    return RES_OK;
}


// Functions - Internal ————————————————————————————————————————————————————————————————————————————

void psInternalSamplePosition(const psInternalEmissionArea area, const float originX,
                              float const originY, float *outX, float *outY)
{
    if (area.outerSpreadX == 0.0f && area.outerSpreadY == 0.0f)
    {
        *outX = originX;
        *outY = originY;
        return;
    }

    float dx, dy;
    switch (area.shape)
    {
    case PS_SHAPE_ELLIPSE:
        if (area.innerSpreadX == area.outerSpreadX && area.innerSpreadY == area.outerSpreadY)
        {
            const float theta = (float)rand() / (float)RAND_MAX * 6.28318530718f;
            *outX = originX + area.outerSpreadX * cosf(theta);
            *outY = originY + area.outerSpreadY * sinf(theta);
            break;
        }

        do
        {
            dx = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * area.outerSpreadX;
            dy = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * area.outerSpreadY;
        }
        while (
            dx / area.outerSpreadX * (dx / area.outerSpreadX) +
            dy / area.outerSpreadY * (dy / area.outerSpreadY) > 1.0f ||
            (area.innerSpreadX > 0.0f &&
             dx / area.innerSpreadX * (dx / area.innerSpreadX) +
             dy / area.innerSpreadY * (dy / area.innerSpreadY) < 1.0f)
        );
        *outX = originX + dx;
        *outY = originY + dy;

        break;
    case PS_SHAPE_RECT:
        if (area.innerSpreadX == area.outerSpreadX && area.innerSpreadY == area.outerSpreadY)
        {
            const float halfPerimeter = area.outerSpreadX + area.outerSpreadY;
            const float t = (float)rand() / (float)RAND_MAX * halfPerimeter;
            if (t < area.outerSpreadX)
            {
                *outX = originX + t * 2.0f - area.outerSpreadX;
                *outY = originY + (rand() % 2 ? area.outerSpreadY : -area.outerSpreadY);
            }
            else
            {
                *outX = originX + (rand() % 2 ? area.outerSpreadX : -area.outerSpreadX);
                *outY = originY + (t - area.outerSpreadX) * 2.0f - area.outerSpreadY;
            }
            break;
        }

        do
        {
            dx = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * area.outerSpreadX;
            dy = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * area.outerSpreadY;
        }
        while (fabsf(dx) < area.innerSpreadX && fabsf(dy) < area.innerSpreadY);

        *outX = originX + dx;
        *outY = originY + dy;

        break;
    case SHAPE_COUNT:
    default:
        *outX = originX;
        *outY = originY;
    }
}

// Functions - Private —————————————————————————————————————————————————————————————————————————————

static bool psPrivateIsPsNull(const ParticleSystem *ps, const char *caller)
{
    if (!ps)
    {
        lgInternalLogWithArg(ERROR, ORI, CSE_NULL_ARG, "ps", caller, CSQ_ABORT);
        return true;
    }

    return false;
}
