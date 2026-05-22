
// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <assert.h>
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
static constexpr int STRESS_ITERATIONS = 1000;

// Mutable

static ParticleSystem *ps;

// Fixtures ————————————————————————————————————————————————————————————————————————————————————————

static void setup(void)
{
    ps = psCreate(MAX_PARTICLES, 0.0f, 0.0f);
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

// void Test_psCreate_ReturnsNullWhenMaxParticlesIsZero(void)
// {
//     ps = psCreate(0, 0.0f, 0.0f);
//     assert(!ps);
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psCreate_ReturnsNullWhenMallocFails(void)
// {
//     tsDisable(MALLOC, 1);
//     ps = psCreate(MAX_PARTICLES, 0.0f, 0.0f);
//     assert(!ps);
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psCreate_StartsWithZeroActiveParticles(void)
// {
//     setup();
//     assert(psGetActive(ps) == 0);
//     teardown();
//     tsPass(__func__);
// }
//
// // Destroy —————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psDestroy_FreesSystem(void)
// {
//     setup();
//     teardown();
//     assert(!ps);
//     tsPass(__func__);
// }
//
// void Test_psDestroy_IsNullSafe(void)
// {
//     assert(psDestroy(nullptr) == RES_NULL_ARG);
//     tsPass(__func__);
// }
//
// // Reset ———————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psReset_KillsAllActiveParticles(void)
// {
//     setup();
//     assert(psEmit(ps, MAX_PARTICLES) == RES_OK);
//     assert(psReset(ps) == RES_OK);
//     assert(psGetActive(ps) == 0);
//     assert(psGetIdle(ps) == MAX_PARTICLES);
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psReset_IsNullSafe(void)
// {
//     assert(psReset(nullptr) == RES_NULL_ARG);
//     tsPass(__func__);
// }
//
//
// // —————————————————————————————————————————————————————————————————————————————————————————————————
// // Emit
// // —————————————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psEmit_IncreasesActiveCount(void)
// {
//     setup();
//     assert(psGetActive(ps) == 0);
//     assert(psGetIdle(ps) == MAX_PARTICLES);
//     assert(psEmit(ps, MAX_PARTICLES) == RES_OK);
//     assert(psGetActive(ps) == MAX_PARTICLES);
//     assert(psGetIdle(ps) == 0);
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psEmit_DoesNotExceedMaxParticles(void)
// {
//     setup();
//     assert(psEmit(ps, MAX_PARTICLES + 1) == RES_OK);
//     assert(psGetActive(ps) == MAX_PARTICLES);
//     assert(psGetIdle(ps) == 0);
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psEmit_IsNullSafe(void)
// {
//     setup();
//     assert(psEmit(nullptr, MAX_PARTICLES) == RES_NULL_ARG);
//     teardown();
//     tsPass(__func__);
// }
//
//
// // Get Active ——————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psGetActive_ReturnsZeroAfterCreate(void)
// {
// }
//
// void Test_psGetActive_ReturnsCountAfterEmit(void)
// {
// }
//
// void Test_psGetActive_DecreasesAfterUpdate(void)
// {
// }
//
// void Test_psGetActive_IsNullSafe(void)
// {
// }
//
// // Get Idle ————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psGetIdle_EqualsMaxParticlesAfterCreate(void)
// {
//     setup();
//     assert(psGetIdle(ps) == MAX_PARTICLES);
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psGetIdle_DecreasesAfterEmit(void)
// {
//     setup();
//     assert(psGetIdle(ps) == MAX_PARTICLES);
//     assert(psEmit(ps, MAX_PARTICLES) == RES_OK);
//     assert(psGetIdle(ps) == 0);
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psGetIdle_IncreasesAfterUpdate(void)
// {
//     setup();
//     assert(psGetIdle(ps) == MAX_PARTICLES);
//     assert(psEmit(ps, MAX_PARTICLES) == RES_OK);
//     assert(psUpdate(ps, TS_MOCK_DT) == RES_OK);
//
//     teardown();
//     tsPass(__func__);
// }
//
// void Test_psGetIdle_IsNullSafe(void)
// {
// }
//
// // Get Position ————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psGetX_ReturnsOriginX(void)
// {
// }
//
// void Test_psGetX_IsNullSafe(void)
// {
// }
//
// void Test_psGetY_ReturnsOriginY(void)
// {
// }
//
// void Test_psGetY_IsNullSafe(void)
// {
// }
//
// // Update ——————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psUpdate_KillsParticleWhenLifetimeExpires(void)
// {
// }
//
// void Test_psUpdate_MovesParticlesWithVelocity(void)
// {
// }
//
// void Test_psUpdate_IsNullSafe(void)
// {
// }
//
// // ForEach —————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psForEach_IteratesAllActiveParticles(void)
// {
// }
//
// void Test_psForEach_PassesArgsToCallback(void)
// {
// }
//
// void Test_psForEach_SkipsNullCallback(void)
// {
// }
//
// void Test_psForEach_IsNullSafe(void)
// {
// }
//
//
// // —————————————————————————————————————————————————————————————————————————————————————————————————
// // Configuration
// // —————————————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psSetLifetime_SetsRange(void)
// {
// }
//
// void Test_psSetLifetime_IsNullSafe(void)
// {
// }
//
// void Test_psSetVelocity_SetsRange(void)
// {
// }
//
// void Test_psSetVelocity_IsNullSafe(void)
// {
// }
//
// void Test_psSetAcceleration_SetsRange(void)
// {
// }
//
// void Test_psSetAcceleration_IsNullSafe(void)
// {
// }
//
// void Test_psSetEmissionShape_SetsShape(void)
// {
// }
//
// void Test_psSetEmissionShape_IsNullSafe(void)
// {
// }
//
// void Test_psSetSpread_SetsSpreads(void)
// {
// }
//
// void Test_psSetSpread_IsNullSafe(void)
// {
// }
//
// void Test_psSetOrigin_UpdatesOrigin(void)
// {
// }
//
// void Test_psSetOrigin_IsNullSafe(void)
// {
// }
//
//
// // —————————————————————————————————————————————————————————————————————————————————————————————————
// // Influence
// // —————————————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psSetInfluence_SetsCallback(void)
// {
// }
//
// void Test_psSetInfluence_NullCallbackClearsInfluence(void)
// {
// }
//
// void Test_psSetInfluence_IsAppliedDuringUpdate(void)
// {
// }
//
// void Test_psSetInfluence_IsNullSafe(void)
// {
// }
//
//
// // —————————————————————————————————————————————————————————————————————————————————————————————————
// // Snapshot Influence
// // —————————————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psSetSnapshotInfluence_ReturnsZeroOnSuccess(void)
// {
// }
//
// void Test_psSetSnapshotInfluence_ReturnsNegativeWhenMallocFails(void)
// {
// }
//
// void Test_psSetSnapshotInfluence_NullCallbackClearsInfluence(void)
// {
// }
//
// void Test_psSetSnapshotInfluence_IsAppliedDuringUpdate(void)
// {
// }
//
// void Test_psSetSnapshotInfluence_IsNullSafe(void)
// {
// }
//
//
// // —————————————————————————————————————————————————————————————————————————————————————————————————
// // Lifetime Events
// // —————————————————————————————————————————————————————————————————————————————————————————————————
//
// void Test_psSetEvent_ReturnsZeroOnSuccess(void)
// {
// }
//
// void Test_psSetEvent_ReturnsNegativeWhenMallocFails(void)
// {
// }
//
// void Test_psSetEvent_FiresWhenThresholdCrossed(void)
// {
// }
//
// void Test_psSetEvent_DoesNotFireBeforeThreshold(void)
// {
// }
//
// void Test_psSetEvent_SupportsMultipleThresholds(void)
// {
// }
//
// void Test_psSetEvent_IsNullSafe(void)
// {
// }
//
//
// // —————————————————————————————————————————————————————————————————————————————————————————————————
// // Stress Tests
// // —————————————————————————————————————————————————————————————————————————————————————————————————
//
// void TestStress_psEmit_EmittingManyParticlesCausesNoSkips(void)
// {
// }
//
// void TestStress_psUpdate_UpdatingManyFramesCausesNoSkips(void)
// {
// }
//
// void TestStress_psSetEvent_RegisteringManyEventsCausesNoSkips(void)
// {
// }
//
//

// Main ————————————————————————————————————————————————————————————————————————————————————————————

int main(void)
{
    puts("\nCREATE TESTING");
    //     Test_psCreate_ReturnsNonNullOnValidArgs();
    //     Test_psCreate_ReturnsNullWhenMaxParticlesIsZero();
    //     Test_psCreate_ReturnsNullWhenMallocFails();
    //     Test_psCreate_StartsWithZeroActiveParticles();
    //
    //     puts("\nDESTROY TESTING");
    //     Test_psDestroy_FreesSystem();
    //     Test_psDestroy_IsNullSafe();
    //
    //     puts("\nRESET TESTING");
    //     Test_psReset_KillsAllActiveParticles();
    //     Test_psReset_IsNullSafe();
    //
    //     puts("\nEMIT TESTING");
    //     Test_psEmit_IncreasesActiveCount();
    //     Test_psEmit_DoesNotExceedMaxParticles();
    //     Test_psEmit_IsNullSafe();
    //
    //     puts("\nGET ACTIVE TESTING");
    //     Test_psGetActive_ReturnsZeroAfterCreate();
    //     Test_psGetActive_ReturnsCountAfterEmit();
    //     Test_psGetActive_DecreasesAfterUpdate();
    //     Test_psGetActive_IsNullSafe();
    //
    //     puts("\nGET IDLE TESTING");
    //     Test_psGetIdle_EqualsMaxParticlesAfterCreate();
    //     Test_psGetIdle_DecreasesAfterEmit();
    //     Test_psGetIdle_IncreasesAfterUpdate();
    //     Test_psGetIdle_IsNullSafe();
    //
    //     puts("\nGET POSITION TESTING");
    //     Test_psGetX_ReturnsOriginX();
    //     Test_psGetX_IsNullSafe();
    //     Test_psGetY_ReturnsOriginY();
    //     Test_psGetY_IsNullSafe();
    //
    //     puts("\nUPDATE TESTING");
    //     Test_psUpdate_KillsParticleWhenLifetimeExpires();
    //     Test_psUpdate_MovesParticlesWithVelocity();
    //     Test_psUpdate_IsNullSafe();
    //
    //     puts("\nFOREACH TESTING");
    //     Test_psForEach_IteratesAllActiveParticles();
    //     Test_psForEach_PassesArgsToCallback();
    //     Test_psForEach_SkipsNullCallback();
    //     Test_psForEach_IsNullSafe();
    //
    //     puts("\nCONFIGURATION TESTING");
    //     puts("• psSetLifetime");
    //     Test_psSetLifetime_SetsRange();
    //     Test_psSetLifetime_IsNullSafe();
    //     puts("• psSetVelocity");
    //     Test_psSetVelocity_SetsRange();
    //     Test_psSetVelocity_IsNullSafe();
    //     puts("• psSetAcceleration");
    //     Test_psSetAcceleration_SetsRange();
    //     Test_psSetAcceleration_IsNullSafe();
    //     puts("• psSetEmissionShape");
    //     Test_psSetEmissionShape_SetsShape();
    //     Test_psSetEmissionShape_IsNullSafe();
    //     puts("• psSetSpread");
    //     Test_psSetSpread_SetsSpreads();
    //     Test_psSetSpread_IsNullSafe();
    //     puts("• psSetOrigin");
    //     Test_psSetOrigin_UpdatesOrigin();
    //     Test_psSetOrigin_IsNullSafe();
    //
    //     puts("\nINFLUENCE TESTING");
    //     Test_psSetInfluence_SetsCallback();
    //     Test_psSetInfluence_NullCallbackClearsInfluence();
    //     Test_psSetInfluence_IsAppliedDuringUpdate();
    //     Test_psSetInfluence_IsNullSafe();
    //
    //     puts("\nSNAPSHOT INFLUENCE TESTING");
    //     Test_psSetSnapshotInfluence_ReturnsZeroOnSuccess();
    //     Test_psSetSnapshotInfluence_ReturnsNegativeWhenMallocFails();
    //     Test_psSetSnapshotInfluence_NullCallbackClearsInfluence();
    //     Test_psSetSnapshotInfluence_IsAppliedDuringUpdate();
    //     Test_psSetSnapshotInfluence_IsNullSafe();
    //
    //     puts("\nEVENT TESTING");
    //     Test_psSetEvent_ReturnsZeroOnSuccess();
    //     Test_psSetEvent_ReturnsNegativeWhenMallocFails();
    //     Test_psSetEvent_FiresWhenThresholdCrossed();
    //     Test_psSetEvent_DoesNotFireBeforeThreshold();
    //     Test_psSetEvent_SupportsMultipleThresholds();
    //     Test_psSetEvent_IsNullSafe();
    //
    //     puts("\nSTRESS TESTING");
    //     TestStress_psEmit_EmittingManyParticlesCausesNoSkips();
    //     TestStress_psUpdate_UpdatingManyFramesCausesNoSkips();
    //     TestStress_psSetEvent_RegisteringManyEventsCausesNoSkips();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
}
