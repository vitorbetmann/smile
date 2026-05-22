// Includes ————————————————————————————————————————————————————————————————————————————————————————

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

// Functions - Public ——————————————————————————————————————————————————————————————————————————————

ParticleSystem *psCreate(const int particles, const float x, const float y)
{
    if (particles <= 0)
    {
        lgInternalLogWithArg(WARN, ORI, CSE_INVALID_ARG, "particles", __func__, CSQ_ABORT);
        return nullptr;
    }

    ParticleSystem *ps = tsCalloc(
        1,
        sizeof(psInternalParticleSystem) + particles * sizeof(Particle)
        );

    if (!ps)
    {
        lgInternalLog(ERROR, ORI, CSE_MEM_ALLOC_FAIL, __func__, CSQ_ABORT);
        return nullptr;
    }

    ps->maxParticles = particles;
    ps->originX = x;
    ps->originY = y;

    return ps;
}

int psDestroy(ParticleSystem *ps)
{
    free(ps);
    return RES_OK;
}

// Functions - Internal ————————————————————————————————————————————————————————————————————————————

// Functions - Private —————————————————————————————————————————————————————————————————————————————
