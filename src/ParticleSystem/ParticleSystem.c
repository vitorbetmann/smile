// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <math.h>
#include <stdlib.h>

#include "ParticleSystem.h"
#include "ParticleSystemInternal.h"
#include "ParticleSystemMessages.h"

#include "LogInternal.h"
#include "internal/Common/Common.h"
#include "internal/Common/CommonMessages.h"
#include "internal/Test/Test.h"

// Variables ———————————————————————————————————————————————————————————————————————————————————————

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
    if (psPrivateIsPsNull(ps, __func__) != RES_OK)
    {
        return nanf("");
    }

    return ps->originY;
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
