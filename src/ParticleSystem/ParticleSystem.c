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

int psReset(ParticleSystem *ps)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    ps->activeParticles = 0;
    return RES_OK;
}

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
    ps->activeParticles += amount > idleParticles ? idleParticles : amount;

    for (int i = 0; i < amount; i++)
    {
        Particle *p = &ps->particles[i];
        // Velocity
        const float velocityDiffX = ps->maxVelocityX - ps->minVelocityX;
        const float velocityDiffY = ps->maxVelocityY - ps->minVelocityY;
        p->velocityX = ps->minVelocityX + (float)rand() / (float)RAND_MAX * velocityDiffX;
        p->velocityY = ps->minVelocityY + (float)rand() / (float)RAND_MAX * velocityDiffY;
        // Acceleration
        const float accelerationDiffX = ps->maxVelocityX - ps->minVelocityX;
        const float accelerationDiffY = ps->maxVelocityY - ps->minVelocityY;
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

int psUpdate(ParticleSystem *ps, const float dt)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    if (dt <= 0.0f)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_NULL_ARG, "dt", __func__, CSQ_ABORT);
        return RES_INVALID_ARG;
    }

    int deadParticles[ps->activeParticles];
    int deadCount = 0;
    for (int i = 0, active = ps->activeParticles; i < active; i++)
    {
        Particle *p = &ps->particles[i];
        p->age += dt;
        if (p->age >= p->lifetime)
        {
            deadParticles[deadCount] = i;
            deadCount++;
        }
        p->velocityX += p->accelerationX * dt;
        p->velocityY += p->accelerationY * dt;
        p->x += p->velocityX * dt;
        p->y += p->velocityY * dt;
    }

    int i = 0;
    while (i < deadCount)
    {
        const int deadIndex = deadParticles[i];
        const Particle temp = ps->particles[deadIndex];

        ps->activeParticles--;
        ps->particles[deadIndex] = ps->particles[ps->activeParticles];
        ps->particles[ps->activeParticles] = temp;

        i++;
    }

    // Stream particles
    ps->streamAccumulator += ps->streamRate * dt;
    const int newParticles = (int)ps->streamAccumulator;
    psBurst(ps, newParticles);
    ps->streamAccumulator -= fminf((float)newParticles, (float)psGetIdle(ps));

    return RES_OK;
}

int psSetVelocity(ParticleSystem *ps, const float minX, const float minY, const float maxX,
                  const float maxY)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
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

    ps->minAccelerationX = minX;
    ps->maxAccelerationX = maxX;
    ps->minAccelerationY = minY;
    ps->maxAccelerationY = maxY;

    return RES_OK;
}

int psSetLifetime(ParticleSystem *ps, const float min, const float max)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    ps->minLifetime = min;
    ps->maxLifetime = max;

    return RES_OK;
}

int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape)
{
    if (psPrivateIsPsNull(ps, __func__))
    {
        return RES_NULL_ARG;
    }

    ps->emissionArea.shape = shape;
    return RES_OK;
}

int psSetOrigin(ParticleSystem *ps, float x, float y)
{
    return RES_OK;
}

int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY)
{
    return RES_OK;
}

// Functions - Internal ————————————————————————————————————————————————————————————————————————————

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
