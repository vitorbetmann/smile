
// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "ParticleSystem.h"
#include "ParticleSystemInternal.h"

#include "internal/Common/Common.h"
#include "internal/Test/Test.h"

// Defines —————————————————————————————————————————————————————————————————————————————————————————

#ifdef NDEBUG
#error "TestAPIParticleSystem must be compiled without NDEBUG (asserts required)."
#endif

// Variables ———————————————————————————————————————————————————————————————————————————————————————

// Constant

static constexpr int MAX_PARTICLES = 100;
static constexpr int NEGATIVE_MAX_PARTICLES = -100;
static constexpr int STRESS_ITERATIONS = 1000;

static constexpr int BURST_AMOUNT = 50;

static constexpr float STREAM_RATE = 10.0f;
static constexpr float NEGATIVE_RATE = -1.0f;
static constexpr float STREAM_ACCUMULATOR = 0.7f;

static constexpr float ORIGIN_X = 3.0f;
static constexpr float ORIGIN_Y = 7.0f;

static constexpr float MOCK_DT = 0.016667f;

static constexpr float MOCK_VELOCITY_X = 10.0f;
static constexpr float MOCK_VELOCITY_Y = 20.0f;

static constexpr float MOCK_ACCELERATION_X = 5.0f;
static constexpr float MOCK_ACCELERATION_Y = 15.0f;

static constexpr float MOCK_LIFETIME = 0.5f;
static constexpr float SHORT_LIFETIME = MOCK_DT;

static constexpr float INNER_SPREAD_X = 1.0f;
static constexpr float INNER_SPREAD_Y = 2.0f;
static constexpr float OUTER_SPREAD_X = 3.0f;
static constexpr float OUTER_SPREAD_Y = 4.0f;


// Mutable

static ParticleSystem *ps;

// Fixtures ————————————————————————————————————————————————————————————————————————————————————————

static void setup(void)
{
    ps = psCreate(MAX_PARTICLES, ORIGIN_X, ORIGIN_Y);
    assert(ps);
}

static void teardown(void)
{
    psDestroy(ps);
    ps = nullptr;
}

// Create ——————————————————————————————————————————————————————————————————————————————————————————

void Test_psCreate_ReturnsNonNullOnValidArgs(void)
{
    setup();
    teardown();
    tsPass(__func__);
}

void Test_psCreate_ReturnsNullWhenMaxParticlesIsZero(void)
{
    ps = psCreate(0, ORIGIN_X, ORIGIN_Y);
    assert(!ps);
    teardown();
    tsPass(__func__);
}

void Test_psCreate_ReturnsNullWhenMaxParticlesIsNegative(void)
{
    ps = psCreate(NEGATIVE_MAX_PARTICLES, ORIGIN_X, ORIGIN_Y);
    assert(!ps);
    tsPass(__func__);
}

void Test_psCreate_ReturnsNullWhenCallocFails(void)
{
    tsDisable(CALLOC, 1);
    ps = psCreate(MAX_PARTICLES, ORIGIN_X, ORIGIN_Y);
    assert(!ps);
    tsPass(__func__);
}

void Test_psCreate_StoresOrigin(void)
{
    setup();
    assert(ps->originX == ORIGIN_X);
    assert(ps->originY == ORIGIN_Y);
    teardown();
    tsPass(__func__);
}

void Test_psCreate_StartsWithZeroActiveParticles(void)
{
    setup();
    assert(ps->activeParticles == 0);
    teardown();
    tsPass(__func__);
}

// Destroy —————————————————————————————————————————————————————————————————————————————————————————

void Test_psDestroy_FreesSystem(void)
{
    setup();
    assert(psDestroy(ps) == RES_OK);
    ps = nullptr;
    tsPass(__func__);
}

void Test_psDestroy_IsNullSafe(void)
{
    assert(psDestroy(nullptr) == RES_NULL_ARG);
    tsPass(__func__);
}

// Reset ———————————————————————————————————————————————————————————————————————————————————————————

void Test_psReset_KillsAllActiveParticles(void)
{
    setup();
    ps->activeParticles = MAX_PARTICLES;
    assert(psReset(ps) == RES_OK);
    assert(ps->activeParticles == 0);
    teardown();
    tsPass(__func__);
}

void Test_psReset_DoesNotResetStreamRate(void)
{
    setup();
    ps->streamRate = STREAM_RATE;
    assert(psReset(ps)==RES_OK);
    assert(ps->streamRate == STREAM_RATE);
    teardown();
    tsPass(__func__);
}

void Test_psReset_DoesNotResetAccumulator(void)
{
    setup();
    ps->streamAccumulator = STREAM_ACCUMULATOR;
    assert(psReset(ps)==RES_OK);
    assert(ps->streamAccumulator == STREAM_ACCUMULATOR);
    teardown();
    tsPass(__func__);
}

void Test_psReset_IsNullSafe(void)
{
    assert(psReset(nullptr) == RES_NULL_ARG);
    tsPass(__func__);
}

// Burst ———————————————————————————————————————————————————————————————————————————————————————————

void Test_psBurst_IncreasesActiveCount(void)
{
    setup();
    assert(psBurst(ps, BURST_AMOUNT) == RES_OK);
    assert(ps->activeParticles == BURST_AMOUNT);
    teardown();
    tsPass(__func__);
}

void Test_psBurst_DoesNotExceedMaxParticles(void)
{
    setup();
    assert(psBurst(ps, MAX_PARTICLES + 1) == RES_OK);
    assert(ps->activeParticles == MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

void Test_psBurst_IsNullSafe(void)
{
    assert(psBurst(nullptr, 1) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psBurst_RejectsNonPositiveAmount(void)
{
    setup();
    assert(psBurst(ps, 0) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psBurst_AddsToExistingActiveParticles(void)
{
    setup();
    assert(psBurst(ps, BURST_AMOUNT) == RES_OK);
    assert(psBurst(ps, BURST_AMOUNT) == RES_OK);
    assert(ps->activeParticles == 2 * BURST_AMOUNT);
    teardown();
    tsPass(__func__);
}

void Test_psBurst_WhenAtCapacityDoesNothing(void)
{
    setup();
    assert(psBurst(ps, MAX_PARTICLES) == RES_OK);
    assert(psBurst(ps, BURST_AMOUNT) == RES_OK);
    assert(ps->activeParticles == MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

void Test_psBurst_SetsParticlePositionToOrigin(void)
{
    setup();
    psBurst(ps, 1);
    assert(ps->particles[0].x == ORIGIN_X);
    assert(ps->particles[0].y == ORIGIN_Y);
    teardown();
    tsPass(__func__);
}

void Test_psBurst_SpawnsParticlesWithinRectBounds(void)
{
    setup();
    psSetEmissionShape(ps, PS_SHAPE_RECT);
    psSetSpread(ps, 0.0f, 0.0f, OUTER_SPREAD_X, OUTER_SPREAD_Y);
    psBurst(ps, MAX_PARTICLES);
    for (int i = 0; i < ps->activeParticles; i++)
    {
        const float dx = ps->particles[i].x - ORIGIN_X;
        const float dy = ps->particles[i].y - ORIGIN_Y;
        assert(fabsf(dx) <= OUTER_SPREAD_X);
        assert(fabsf(dy) <= OUTER_SPREAD_Y);
    }
    teardown();
    tsPass(__func__);
}

void Test_psBurst_SpawnsParticlesWithinEllipseBounds(void)
{
    setup();
    psSetEmissionShape(ps, PS_SHAPE_ELLIPSE);
    psSetSpread(ps, 0.0f, 0.0f, OUTER_SPREAD_X, OUTER_SPREAD_Y);
    psBurst(ps, MAX_PARTICLES);
    for (int i = 0; i < ps->activeParticles; i++)
    {
        const float dx = (ps->particles[i].x - ORIGIN_X) / OUTER_SPREAD_X;
        const float dy = (ps->particles[i].y - ORIGIN_Y) / OUTER_SPREAD_Y;
        assert(dx * dx + dy * dy <= 1.0f);
    }
    teardown();
    tsPass(__func__);
}

void Test_psBurst_DoesNotOverwriteExistingParticlesOnSecondBurst(void)
{
    setup();
    psSetVelocity(ps, MOCK_VELOCITY_X, MOCK_VELOCITY_Y, MOCK_VELOCITY_X, MOCK_VELOCITY_Y);
    psBurst(ps, 1);
    psSetVelocity(ps, MOCK_VELOCITY_X * 2.0f, MOCK_VELOCITY_Y * 2.0f,
                  MOCK_VELOCITY_X * 2.0f, MOCK_VELOCITY_Y * 2.0f);
    psBurst(ps, 1);
    assert(ps->particles[0].velocityX == MOCK_VELOCITY_X);
    assert(ps->particles[0].velocityY == MOCK_VELOCITY_Y);
    teardown();
    tsPass(__func__);
}

void Test_psBurst_SpawnsParticlesOutsideInnerRectBounds(void)
{
    setup();
    psSetEmissionShape(ps, PS_SHAPE_RECT);
    psSetSpread(ps, INNER_SPREAD_X, INNER_SPREAD_Y, OUTER_SPREAD_X, OUTER_SPREAD_Y);
    psBurst(ps, MAX_PARTICLES);
    for (int i = 0; i < ps->activeParticles; i++)
    {
        const float dx = fabsf(ps->particles[i].x - ORIGIN_X);
        const float dy = fabsf(ps->particles[i].y - ORIGIN_Y);
        assert(!(dx < INNER_SPREAD_X && dy < INNER_SPREAD_Y));
    }
    teardown();
    tsPass(__func__);
}

void Test_psBurst_SpawnsParticlesOutsideInnerEllipseBounds(void)
{
    setup();
    psSetEmissionShape(ps, PS_SHAPE_ELLIPSE);
    psSetSpread(ps, INNER_SPREAD_X, INNER_SPREAD_Y, OUTER_SPREAD_X, OUTER_SPREAD_Y);
    psBurst(ps, MAX_PARTICLES);
    for (int i = 0; i < ps->activeParticles; i++)
    {
        const float dx = (ps->particles[i].x - ORIGIN_X) / INNER_SPREAD_X;
        const float dy = (ps->particles[i].y - ORIGIN_Y) / INNER_SPREAD_Y;
        assert(dx * dx + dy * dy >= 1.0f);
    }
    teardown();
    tsPass(__func__);
}

// Stream ——————————————————————————————————————————————————————————————————————————————————————————

void Test_psStream_SetsRate(void)
{
    setup();
    assert(psStream(ps, STREAM_RATE) == RES_OK);
    assert(ps->streamRate == STREAM_RATE);
    teardown();
    tsPass(__func__);
}

void Test_psStream_ZeroDisablesStreaming(void)
{
    setup();
    assert(psStream(ps, STREAM_RATE) == RES_OK);
    assert(psStream(ps, 0.0f) == RES_OK);
    assert(ps->streamRate == 0.0f);
    teardown();
    tsPass(__func__);
}

void Test_psStream_RejectsNegativeRate(void)
{
    setup();
    assert(psStream(ps, NEGATIVE_RATE) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psStream_RateCapIsMaxParticles(void)
{
    setup();
    assert(psStream(ps, (float)MAX_PARTICLES + 1.0f) == RES_OK);
    assert(ps->streamRate == (float)MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

void Test_psStream_IsNullSafe(void)
{
    setup();
    assert(psStream(nullptr, STREAM_RATE) == RES_NULL_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psStream_AccumulatorNotResetOnZero(void)
{
    setup();
    ps->streamAccumulator = STREAM_ACCUMULATOR;
    assert(psStream(ps, 0.0f) == RES_OK);
    assert(ps->streamAccumulator == STREAM_ACCUMULATOR);
    teardown();
    tsPass(__func__);
}

void Test_psStream_IsIndependentOfBurst(void)
{
    setup();
    assert(psStream(ps, STREAM_RATE) == RES_OK);
    assert(psBurst(ps, MAX_PARTICLES) == RES_OK);
    assert(ps->streamRate == STREAM_RATE);
    assert(ps->activeParticles == MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

// Get Active ——————————————————————————————————————————————————————————————————————————————————————

void Test_psGetActive_ReturnsZeroAfterCreate(void)
{
    setup();
    assert(psGetActive(ps) == 0);
    teardown();
    tsPass(__func__);
}

void Test_psGetActive_ReturnsAmountAfterBurst(void)
{
    setup();
    assert(psBurst(ps, BURST_AMOUNT) == RES_OK);
    assert(psGetActive(ps) == BURST_AMOUNT);
    teardown();
    tsPass(__func__);
}

void Test_psGetActive_DecreasesAfterUpdate(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, BURST_AMOUNT);
    psUpdate(ps, MOCK_LIFETIME);
    assert(psGetActive(ps) == 0);
    teardown();
    tsPass(__func__);
}

void Test_psGetActive_IsNullSafe(void)
{
    assert(psGetActive(nullptr) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psGetActive_ReturnsMaxParticlesAtCapacity(void)
{
    setup();
    ps->activeParticles = MAX_PARTICLES;
    assert(psGetActive(ps) == MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

void Test_psGetActive_ReturnsZeroAfterReset(void)
{
    setup();
    ps->activeParticles = MAX_PARTICLES;
    assert(psReset(ps) == RES_OK);
    assert(psGetActive(ps) == 0);
    teardown();
    tsPass(__func__);
}

// Get Idle ————————————————————————————————————————————————————————————————————————————————————————

void Test_psGetIdle_EqualsMaxParticlesAfterCreate(void)
{
    setup();
    assert(psGetIdle(ps) == MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

void Test_psGetIdle_DecreasesAfterBurst(void)
{
    setup();
    assert(psBurst(ps, BURST_AMOUNT) == RES_OK);
    assert(psGetIdle(ps) == MAX_PARTICLES - BURST_AMOUNT);
    teardown();
    tsPass(__func__);
}

void Test_psGetIdle_IncreasesAfterUpdate(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, BURST_AMOUNT);
    psUpdate(ps, MOCK_LIFETIME);
    assert(psGetIdle(ps) == MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

void Test_psGetIdle_IsNullSafe(void)
{
    assert(psGetIdle(nullptr) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psGetIdle_ReturnsZeroAtCapacity(void)
{
    setup();
    ps->activeParticles = MAX_PARTICLES;
    assert(psGetIdle(ps) == 0);
    teardown();
    tsPass(__func__);
}

void Test_psGetIdle_ReturnsMaxParticlesAfterReset(void)
{
    setup();
    ps->activeParticles = MAX_PARTICLES;
    assert(psReset(ps) == RES_OK);
    assert(psGetIdle(ps) == MAX_PARTICLES);
    teardown();
    tsPass(__func__);
}

// Get Position ————————————————————————————————————————————————————————————————————————————————————

void Test_psGetX_ReturnsOriginX(void)
{
    setup();
    assert(psGetX(ps) == ORIGIN_X);
    teardown();
    tsPass(__func__);
}

void Test_psGetX_IsNullSafe(void)
{
    setup();
    assert(isnan(psGetX(nullptr)));
    teardown();
    tsPass(__func__);
}

void Test_psGetY_ReturnsOriginY(void)
{
    setup();
    assert(psGetY(ps) == ORIGIN_Y);
    teardown();
    tsPass(__func__);
}

void Test_psGetY_IsNullSafe(void)
{
    setup();
    assert(isnan(psGetY(nullptr)));
    teardown();
    tsPass(__func__);
}

// Update ——————————————————————————————————————————————————————————————————————————————————————————

void Test_psUpdate_IsNullSafe(void)
{
    assert(psUpdate(nullptr, MOCK_DT) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psUpdate_RejectsZeroDt(void)
{
    setup();
    assert(psUpdate(ps, 0.0f) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_RejectsNegativeDt(void)
{
    setup();
    assert(psUpdate(ps, -MOCK_DT) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_AdvancesParticleAge(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, 1);
    assert(psUpdate(ps, MOCK_DT) == RES_OK);
    assert(ps->particles[0].age == MOCK_DT);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_MovesParticlesWithVelocity(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psSetVelocity(ps, MOCK_VELOCITY_X, MOCK_VELOCITY_Y, MOCK_VELOCITY_X, MOCK_VELOCITY_Y);
    psBurst(ps, 1);

    const float startX = ps->particles[0].x;
    const float startY = ps->particles[0].y;

    psUpdate(ps, MOCK_DT);

    assert(ps->particles[0].x == startX + MOCK_VELOCITY_X * MOCK_DT);
    assert(ps->particles[0].y == startY + MOCK_VELOCITY_Y * MOCK_DT);
    teardown();
    tsPass(__func__);

}

void Test_psUpdate_AppliesAccelerationToVelocity(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psSetVelocity(ps, 0.0f, 0.0f, 0.0f, 0.0f);
    psSetAcceleration(ps, MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y,
                      MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y);
    psBurst(ps, 1);
    psUpdate(ps, MOCK_DT);
    assert(ps->particles[0].velocityX == MOCK_ACCELERATION_X * MOCK_DT);
    assert(ps->particles[0].velocityY == MOCK_ACCELERATION_Y * MOCK_DT);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_KillsParticleWhenLifetimeExpires(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, 1);
    psUpdate(ps, MOCK_LIFETIME);
    assert(ps->activeParticles == 0);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_KillsMultipleExpiredParticlesInSameFrame(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, BURST_AMOUNT);
    psUpdate(ps, MOCK_LIFETIME);
    assert(ps->activeParticles == 0);
    teardown();
    tsPass(__func__);

}

void Test_psUpdate_KillsNonContiguousParticlesCorrectly(void)
{
    setup();
    psSetLifetime(ps, SHORT_LIFETIME, SHORT_LIFETIME);
    psBurst(ps, 1); // index 0: dies on update
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, 2); // indices 1, 2: survive
    psSetLifetime(ps, SHORT_LIFETIME, SHORT_LIFETIME);
    psBurst(ps, 1); // index 3: dies on update
    psUpdate(ps, MOCK_DT);
    assert(ps->activeParticles == 2);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_SpawnsParticlesAfterSufficientTime(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psStream(ps, STREAM_RATE);
    psUpdate(ps, 1.0f / STREAM_RATE);
    assert(ps->activeParticles == 1);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_AccumulatorBelowThresholdDoesNotSpawn(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psStream(ps, STREAM_RATE);
    psUpdate(ps, MOCK_DT); // MOCK_DT needs to be less than STREAM_RATE for test to work
    assert(ps->activeParticles == 0);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_AccumulatorContinuesWhenPoolExhausted(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, MAX_PARTICLES);
    psStream(ps, STREAM_RATE);
    psUpdate(ps, 1.0f / STREAM_RATE);
    assert(ps->activeParticles == MAX_PARTICLES);
    assert(ps->streamAccumulator == 1.0f);
    teardown();
    tsPass(__func__);
}

void Test_psUpdate_AccumulatorDecrementsOnlyByActualSpawns(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, MAX_PARTICLES - 1); // 1 idle slot
    psStream(ps, STREAM_RATE);
    psUpdate(ps, 3.0f / STREAM_RATE); // accumulator = 3.0, only 1 spawned
    assert(ps->activeParticles == MAX_PARTICLES);
    assert(ps->streamAccumulator == 2.0f); // 3.0 - 1 spawned = 2.0
    teardown();
    tsPass(__func__);
}

// ForEach —————————————————————————————————————————————————————————————————————————————————————————

typedef struct
{
    float velocityX, velocityY;
    float age;
    float lifetime;
} psForEachCapturedParticle;

static void psForEachCountCallback(const Particle *p, void *args)
{
    (void)p;
    (*(int *)args)++;
}

static void psForEachCaptureCallback(const Particle *p, void *args)
{
    psForEachCapturedParticle *out = (psForEachCapturedParticle *)args;
    out->velocityX = p->velocityX;
    out->velocityY = p->velocityY;
    out->age = p->age;
    out->lifetime = p->lifetime;
}

static void psForEachArgsCallback(const Particle *p, void *args)
{
    (void)p;
    assert(args != nullptr);
    (*(int *)args)++;
}

static void psForEachNullArgsCallback(const Particle *p, void *args)
{
    (void)p;
    assert(args == nullptr);
}

void Test_psForEach_IsNullSafe(void)
{
    assert(psForEach(nullptr, psForEachCountCallback, nullptr) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psForEach_SkipsNullCallback(void)
{
    setup();
    assert(psForEach(ps, nullptr, nullptr) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psForEach_WithEmptySystemCallsCallbackZeroTimes(void)
{
    setup();
    int count = 0;
    psForEach(ps, psForEachCountCallback, &count);
    assert(count == 0);
    teardown();
    tsPass(__func__);
}

void Test_psForEach_IteratesAllActiveParticles(void)
{
    setup();
    psBurst(ps, BURST_AMOUNT);
    int count = 0;
    psForEach(ps, psForEachCountCallback, &count);
    assert(count == BURST_AMOUNT);
    teardown();
    tsPass(__func__);
}

void Test_psForEach_PassesArgsToCallback(void)
{
    setup();
    psBurst(ps, BURST_AMOUNT);
    int count = 0;
    psForEach(ps, psForEachArgsCallback, &count);
    assert(count == BURST_AMOUNT);
    teardown();
    tsPass(__func__);
}

void Test_psForEach_WithNullArgsCallsCallback(void)
{
    setup();
    psBurst(ps, 1);
    psForEach(ps, psForEachNullArgsCallback, nullptr);
    teardown();
    tsPass(__func__);
}

void Test_psForEach_ReturnsOkOnSuccess(void)
{
    setup();
    psBurst(ps, 1);
    int count = 0;
    assert(psForEach(ps, psForEachCountCallback, &count) == RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_psForEach_ExposesCorrectParticleData(void)
{
    setup();
    psSetVelocity(ps, MOCK_VELOCITY_X, MOCK_VELOCITY_Y, MOCK_VELOCITY_X, MOCK_VELOCITY_Y);
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, 1);

    psForEachCapturedParticle captured = {0};
    psForEach(ps, psForEachCaptureCallback, &captured);

    assert(captured.velocityX == MOCK_VELOCITY_X);
    assert(captured.velocityY == MOCK_VELOCITY_Y);
    assert(captured.age == 0.0f);
    assert(captured.lifetime == MOCK_LIFETIME);
    teardown();
    tsPass(__func__);
}

void Test_psForEach_SkipsDeadParticlesAfterUpdate(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, BURST_AMOUNT);
    psUpdate(ps, MOCK_LIFETIME);
    int count = 0;
    psForEach(ps, psForEachCountCallback, &count);
    assert(count == 0);
    teardown();
    tsPass(__func__);
}

// Configuration ———————————————————————————————————————————————————————————————————————————————————

void Test_psSetLifetime_SetsRange(void)
{
    setup();
    assert(psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME * 2) == RES_OK);
    assert(ps->minLifetime == MOCK_LIFETIME);
    assert(ps->maxLifetime == MOCK_LIFETIME * 2);
    teardown();
    tsPass(__func__);
}

void Test_psSetLifetime_IsNullSafe(void)
{
    assert(psSetLifetime(nullptr, MOCK_LIFETIME, MOCK_LIFETIME) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psSetLifetime_RejectsMinGreaterThanMax(void)
{
    setup();
    assert(psSetLifetime(ps, MOCK_LIFETIME * 2, MOCK_LIFETIME) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetLifetime_AffectsSpawnedParticles(void)
{
    setup();
    psSetLifetime(ps, MOCK_LIFETIME, MOCK_LIFETIME);
    psBurst(ps, 1);
    assert(ps->particles[0].lifetime == MOCK_LIFETIME);
    teardown();
    tsPass(__func__);
}

void Test_psSetVelocity_SetsRange(void)
{
    setup();
    assert(psSetVelocity(ps, MOCK_VELOCITY_X, MOCK_VELOCITY_Y,
        MOCK_VELOCITY_X * 2, MOCK_VELOCITY_Y * 2) == RES_OK);
    assert(ps->minVelocityX == MOCK_VELOCITY_X);
    assert(ps->minVelocityY == MOCK_VELOCITY_Y);
    assert(ps->maxVelocityX == MOCK_VELOCITY_X * 2);
    assert(ps->maxVelocityY == MOCK_VELOCITY_Y * 2);
    teardown();
    tsPass(__func__);
}

void Test_psSetVelocity_IsNullSafe(void)
{
    assert(psSetVelocity(nullptr, MOCK_VELOCITY_X, MOCK_VELOCITY_Y,
        MOCK_VELOCITY_X, MOCK_VELOCITY_Y) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psSetVelocity_RejectsMinXGreaterThanMaxX(void)
{
    setup();
    assert(psSetVelocity(ps, MOCK_VELOCITY_X * 2, MOCK_VELOCITY_Y,
        MOCK_VELOCITY_X, MOCK_VELOCITY_Y) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetVelocity_RejectsMinYGreaterThanMaxY(void)
{
    setup();
    assert(psSetVelocity(ps, MOCK_VELOCITY_X, MOCK_VELOCITY_Y * 2,
        MOCK_VELOCITY_X, MOCK_VELOCITY_Y) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetVelocity_AffectsSpawnedParticles(void)
{
    setup();
    psSetVelocity(ps, MOCK_VELOCITY_X, MOCK_VELOCITY_Y, MOCK_VELOCITY_X, MOCK_VELOCITY_Y);
    psBurst(ps, 1);
    assert(ps->particles[0].velocityX == MOCK_VELOCITY_X);
    assert(ps->particles[0].velocityY == MOCK_VELOCITY_Y);
    teardown();
    tsPass(__func__);
}

void Test_psSetAcceleration_SetsRange(void)
{
    setup();
    assert(psSetAcceleration(ps, MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y,
        MOCK_ACCELERATION_X * 2, MOCK_ACCELERATION_Y * 2) == RES_OK);
    assert(ps->minAccelerationX == MOCK_ACCELERATION_X);
    assert(ps->minAccelerationY == MOCK_ACCELERATION_Y);
    assert(ps->maxAccelerationX == MOCK_ACCELERATION_X * 2);
    assert(ps->maxAccelerationY == MOCK_ACCELERATION_Y * 2);
    teardown();
    tsPass(__func__);
}

void Test_psSetAcceleration_IsNullSafe(void)
{
    assert(psSetAcceleration(nullptr, MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y,
        MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psSetAcceleration_RejectsMinXGreaterThanMaxX(void)
{
    setup();
    assert(psSetAcceleration(ps, MOCK_ACCELERATION_X * 2, MOCK_ACCELERATION_Y,
        MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetAcceleration_RejectsMinYGreaterThanMaxY(void)
{
    setup();
    assert(psSetAcceleration(ps, MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y * 2,
        MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetAcceleration_AffectsSpawnedParticles(void)
{
    setup();
    psSetAcceleration(ps, MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y,
                      MOCK_ACCELERATION_X, MOCK_ACCELERATION_Y);
    psBurst(ps, 1);
    assert(ps->particles[0].accelerationX == MOCK_ACCELERATION_X);
    assert(ps->particles[0].accelerationY == MOCK_ACCELERATION_Y);
    teardown();
    tsPass(__func__);
}

void Test_psSetEmissionShape_SetsShape(void)
{
    setup();
    assert(psSetEmissionShape(ps, PS_SHAPE_RECT) == RES_OK);
    assert(ps->emissionArea.shape == PS_SHAPE_RECT);
    teardown();
    tsPass(__func__);
}

void Test_psSetEmissionShape_IsNullSafe(void)
{
    assert(psSetEmissionShape(nullptr, PS_SHAPE_RECT) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psSetEmissionShape_SetsEllipse(void)
{
    setup();
    assert(psSetEmissionShape(ps, PS_SHAPE_ELLIPSE) == RES_OK);
    assert(ps->emissionArea.shape == PS_SHAPE_ELLIPSE);
    teardown();
    tsPass(__func__);
}

void Test_psSetEmissionShape_DefaultIsEllipse(void)
{
    setup();
    assert(ps->emissionArea.shape == PS_SHAPE_ELLIPSE);
    teardown();
    tsPass(__func__);
}

void Test_psSetEmissionShape_RejectsInvalidShape(void)
{
    setup();
    assert(psSetEmissionShape(ps, (psEmissionShape)-1) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetSpread_SetsSpreads(void)
{
    setup();
    assert(
        psSetSpread(ps, INNER_SPREAD_X, INNER_SPREAD_Y, OUTER_SPREAD_X, OUTER_SPREAD_Y) == RES_OK);
    assert(ps->emissionArea.innerSpreadX == INNER_SPREAD_X);
    assert(ps->emissionArea.innerSpreadY == INNER_SPREAD_Y);
    assert(ps->emissionArea.outerSpreadX == OUTER_SPREAD_X);
    assert(ps->emissionArea.outerSpreadY == OUTER_SPREAD_Y);
    teardown();
    tsPass(__func__);
}

void Test_psSetSpread_RejectsInnerExceedingOuter(void)
{
    setup();
    assert(
        psSetSpread(ps,
            OUTER_SPREAD_X,
            OUTER_SPREAD_Y,
            INNER_SPREAD_X,
            INNER_SPREAD_Y
        ) == RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetSpread_IsNullSafe(void)
{
    assert(
        psSetSpread(nullptr,
            INNER_SPREAD_X,
            INNER_SPREAD_Y,
            OUTER_SPREAD_X,
            OUTER_SPREAD_Y
        ) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_psSetSpread_RejectsInnerXExceedingOuterX(void)
{
    setup();
    assert(psSetSpread(ps, OUTER_SPREAD_X, INNER_SPREAD_Y, INNER_SPREAD_X, OUTER_SPREAD_Y) ==
        RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetSpread_RejectsInnerYExceedingOuterY(void)
{
    setup();
    assert(psSetSpread(ps, INNER_SPREAD_X, OUTER_SPREAD_Y, OUTER_SPREAD_X, INNER_SPREAD_Y) ==
        RES_INVALID_ARG);
    teardown();
    tsPass(__func__);
}

void Test_psSetSpread_AllowsInnerEqualToOuter(void)
{
    setup();
    assert(psSetSpread(ps, OUTER_SPREAD_X, OUTER_SPREAD_Y, OUTER_SPREAD_X, OUTER_SPREAD_Y) ==
        RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_psSetSpread_DefaultsToZero(void)
{
    setup();
    assert(ps->emissionArea.innerSpreadX == 0.0f);
    assert(ps->emissionArea.innerSpreadY == 0.0f);
    assert(ps->emissionArea.outerSpreadX == 0.0f);
    assert(ps->emissionArea.outerSpreadY == 0.0f);
    teardown();
    tsPass(__func__);
}

void Test_psSetOrigin_AffectsSpawnedParticles(void)
{
    setup();
    psSetOrigin(ps, ORIGIN_X * 2, ORIGIN_Y * 2);
    psBurst(ps, 1);
    assert(ps->particles[0].x == ORIGIN_X * 2);
    assert(ps->particles[0].y == ORIGIN_Y * 2);
    teardown();
    tsPass(__func__);
}

void Test_psSetOrigin_UpdatesOrigin(void)
{
    setup();
    assert(psSetOrigin(ps, ORIGIN_X * 2, ORIGIN_Y * 2) == RES_OK);
    assert(ps->originX == ORIGIN_X * 2);
    assert(ps->originY == ORIGIN_Y * 2);
    teardown();
    tsPass(__func__);
}

void Test_psSetOrigin_IsNullSafe(void)
{
    assert(psSetOrigin(nullptr, ORIGIN_X, ORIGIN_Y) == RES_NULL_ARG);
    tsPass(__func__);
}

// Influence ———————————————————————————————————————————————————————————————————————————————————————

void Test_psSetInfluence_SetsCallback(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetInfluence_NullCallbackClearsInfluence(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetInfluence_ContextIsPassedToCallback(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetInfluence_CanBeSwappedAtRuntime(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetInfluence_IsAppliedDuringUpdate(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetInfluence_IsNullSafe(void)
{
    assert(false);
    tsPass(__func__);
}

// Snapshot Influence ——————————————————————————————————————————————————————————————————————————————

void Test_psSetSnapshotInfluence_ReturnsZeroOnSuccess(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetSnapshotInfluence_ReturnsNegativeWhenMallocFails(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetSnapshotInfluence_NullCallbackClearsInfluence(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetSnapshotInfluence_SecondBufferPersistsAfterCallbackCleared(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetSnapshotInfluence_ContextIsPassedToCallback(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetSnapshotInfluence_CountIsPassedToCallback(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetSnapshotInfluence_IsAppliedDuringUpdate(void)
{
    assert(false);
    tsPass(__func__);
}

void Test_psSetSnapshotInfluence_IsNullSafe(void)
{
    assert(false);
    tsPass(__func__);
}

// Stress Tests ————————————————————————————————————————————————————————————————————————————————————

// void TestStress_psBurst_EmittingManyParticlesCausesNoSkips(void)
// {
//    assert(false);
//    tsPass(__func__);
// }

// void TestStress_psUpdate_UpdatingManyFramesCausesNoSkips(void)
// {
//    assert(false);
//    tsPass(__func__);
// }

// Main ————————————————————————————————————————————————————————————————————————————————————————————

int main(void)
{
    puts("\nCREATE TESTING");
    Test_psCreate_ReturnsNonNullOnValidArgs();
    Test_psCreate_ReturnsNullWhenMaxParticlesIsZero();
    Test_psCreate_ReturnsNullWhenMaxParticlesIsNegative();
    Test_psCreate_ReturnsNullWhenCallocFails();
    Test_psCreate_StoresOrigin();
    Test_psCreate_StartsWithZeroActiveParticles();

    puts("\nDESTROY TESTING");
    Test_psDestroy_FreesSystem();
    Test_psDestroy_IsNullSafe();

    puts("\nRESET TESTING");
    Test_psReset_KillsAllActiveParticles();
    Test_psReset_DoesNotResetStreamRate();
    Test_psReset_DoesNotResetAccumulator();
    Test_psReset_IsNullSafe();

    puts("\nBURST TESTING");
    Test_psBurst_IncreasesActiveCount();
    Test_psBurst_DoesNotExceedMaxParticles();
    Test_psBurst_IsNullSafe();
    Test_psBurst_RejectsNonPositiveAmount();
    Test_psBurst_AddsToExistingActiveParticles();
    Test_psBurst_WhenAtCapacityDoesNothing();
    Test_psBurst_SetsParticlePositionToOrigin();
    Test_psBurst_SpawnsParticlesWithinRectBounds();
    Test_psBurst_SpawnsParticlesWithinEllipseBounds();
    Test_psBurst_DoesNotOverwriteExistingParticlesOnSecondBurst();
    Test_psBurst_SpawnsParticlesOutsideInnerRectBounds();
    Test_psBurst_SpawnsParticlesOutsideInnerEllipseBounds();

    puts("\nSTREAM TESTING");
    Test_psStream_SetsRate();
    Test_psStream_ZeroDisablesStreaming();
    Test_psStream_RejectsNegativeRate();
    Test_psStream_RateCapIsMaxParticles();
    Test_psStream_IsNullSafe();
    Test_psStream_AccumulatorNotResetOnZero();
    Test_psStream_IsIndependentOfBurst();

    puts("\nGET ACTIVE TESTING");
    Test_psGetActive_ReturnsZeroAfterCreate();
    Test_psGetActive_ReturnsAmountAfterBurst();
    Test_psGetActive_DecreasesAfterUpdate();
    Test_psGetActive_IsNullSafe();
    Test_psGetActive_ReturnsMaxParticlesAtCapacity();
    Test_psGetActive_ReturnsZeroAfterReset();

    puts("\nGET IDLE TESTING");
    Test_psGetIdle_EqualsMaxParticlesAfterCreate();
    Test_psGetIdle_DecreasesAfterBurst();
    Test_psGetIdle_IncreasesAfterUpdate();
    Test_psGetIdle_IsNullSafe();
    Test_psGetIdle_ReturnsMaxParticlesAfterReset();
    Test_psGetIdle_ReturnsZeroAtCapacity();

    puts("\nGET POSITION TESTING");
    Test_psGetX_ReturnsOriginX();
    Test_psGetX_IsNullSafe();
    Test_psGetY_ReturnsOriginY();
    Test_psGetY_IsNullSafe();

    puts("\nUPDATE TESTING");
    Test_psUpdate_IsNullSafe();
    Test_psUpdate_RejectsZeroDt();
    Test_psUpdate_RejectsNegativeDt();
    Test_psUpdate_AdvancesParticleAge();
    Test_psUpdate_MovesParticlesWithVelocity();
    Test_psUpdate_AppliesAccelerationToVelocity();
    Test_psUpdate_KillsParticleWhenLifetimeExpires();
    Test_psUpdate_KillsMultipleExpiredParticlesInSameFrame();
    Test_psUpdate_KillsNonContiguousParticlesCorrectly();
    Test_psUpdate_SpawnsParticlesAfterSufficientTime();
    Test_psUpdate_AccumulatorBelowThresholdDoesNotSpawn();
    Test_psUpdate_AccumulatorContinuesWhenPoolExhausted();
    Test_psUpdate_AccumulatorDecrementsOnlyByActualSpawns();

    puts("\nFOREACH TESTING");
    Test_psForEach_IsNullSafe();
    Test_psForEach_SkipsNullCallback();
    Test_psForEach_WithEmptySystemCallsCallbackZeroTimes();
    Test_psForEach_IteratesAllActiveParticles();
    Test_psForEach_PassesArgsToCallback();
    Test_psForEach_WithNullArgsCallsCallback();
    Test_psForEach_ReturnsOkOnSuccess();
    Test_psForEach_ExposesCorrectParticleData();
    Test_psForEach_SkipsDeadParticlesAfterUpdate();

    puts("\nCONFIGURATION TESTING");
    puts("• psSetLifetime");
    Test_psSetLifetime_SetsRange();
    Test_psSetLifetime_IsNullSafe();
    Test_psSetLifetime_RejectsMinGreaterThanMax();
    Test_psSetLifetime_AffectsSpawnedParticles();
    puts("• psSetVelocity");
    Test_psSetVelocity_SetsRange();
    Test_psSetVelocity_IsNullSafe();
    Test_psSetVelocity_RejectsMinXGreaterThanMaxX();
    Test_psSetVelocity_RejectsMinYGreaterThanMaxY();
    Test_psSetVelocity_AffectsSpawnedParticles();
    puts("• psSetAcceleration");
    Test_psSetAcceleration_SetsRange();
    Test_psSetAcceleration_IsNullSafe();
    Test_psSetAcceleration_RejectsMinXGreaterThanMaxX();
    Test_psSetAcceleration_RejectsMinYGreaterThanMaxY();
    Test_psSetAcceleration_AffectsSpawnedParticles();
    puts("• psSetEmissionShape");
    Test_psSetEmissionShape_SetsShape();
    Test_psSetEmissionShape_IsNullSafe();
    Test_psSetEmissionShape_SetsEllipse();
    Test_psSetEmissionShape_DefaultIsEllipse();
    Test_psSetEmissionShape_RejectsInvalidShape();
    puts("• psSetSpread");
    Test_psSetSpread_SetsSpreads();
    Test_psSetSpread_RejectsInnerExceedingOuter();
    Test_psSetSpread_IsNullSafe();
    Test_psSetSpread_RejectsInnerXExceedingOuterX();
    Test_psSetSpread_RejectsInnerYExceedingOuterY();
    Test_psSetSpread_AllowsInnerEqualToOuter();
    Test_psSetSpread_DefaultsToZero();
    puts("• psSetOrigin");
    Test_psSetOrigin_AffectsSpawnedParticles();
    Test_psSetOrigin_UpdatesOrigin();
    Test_psSetOrigin_IsNullSafe();

    puts("\nINFLUENCE TESTING");
    // Test_psSetInfluence_SetsCallback();
    // Test_psSetInfluence_NullCallbackClearsInfluence();
    // Test_psSetInfluence_ContextIsPassedToCallback();
    // Test_psSetInfluence_CanBeSwappedAtRuntime();
    // Test_psSetInfluence_IsAppliedDuringUpdate();
    // Test_psSetInfluence_IsNullSafe();

    puts("\nSNAPSHOT INFLUENCE TESTING");
    // Test_psSetSnapshotInfluence_ReturnsZeroOnSuccess();
    // Test_psSetSnapshotInfluence_ReturnsNegativeWhenMallocFails();
    // Test_psSetSnapshotInfluence_NullCallbackClearsInfluence();
    // Test_psSetSnapshotInfluence_SecondBufferPersistsAfterCallbackCleared();
    // Test_psSetSnapshotInfluence_ContextIsPassedToCallback();
    // Test_psSetSnapshotInfluence_CountIsPassedToCallback();
    // Test_psSetSnapshotInfluence_IsAppliedDuringUpdate();
    // Test_psSetSnapshotInfluence_IsNullSafe();

    puts("\nSTRESS TESTING");
    // TestStress_psBurst_EmittingManyParticlesCausesNoSkips();
    // TestStress_psUpdate_UpdatingManyFramesCausesNoSkips();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
}
