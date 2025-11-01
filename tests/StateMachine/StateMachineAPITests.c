/**
* @file
 * @brief Implementation of the StateMachine API Tests.
 *
 * @see StateMachineAPITests.h
 *
 * @bug No known bugs.
 *
 * @author Vitor Betmann
 * @date 2025-10-29
 * @version 1.0.0
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "StateMachineApiTests.h"

#include "StateMachineInternal.h"
#include "include/StateMachine.h"
#include "src/_Internal/Test/TestInternal.h"


// -----------------------------------------------------------------------------
// Mock & Test Functions
// -----------------------------------------------------------------------------

static void mockEnter(void *args)
{
    // Mock Enter
}

void onEnter(MockData *data)
{
    data->enterCount++;
}

void onEnterWithArgs(MockData *data, MockArgs *args)
{
    data->enterCount++;
    args->flag = true;
}

static void mockUpdate(float dt)
{
    // Mock Update
}

static void mockDraw(void)
{
    // Mock Draw
}

static void mockExit(void)
{
    // Mock Exit
}

void onExit(MockData *data)
{
    data->exitCount++;
}

// -----------------------------------------------------------------------------
// variables
// -----------------------------------------------------------------------------

static float mockDt = 0.016f;

static InternalState mock = {
    .name = "mock",
    .enter = mockEnter,
    .update = mockUpdate,
    .draw = mockDraw,
    .exit = mockExit,
};

static InternalState mock2 = {
    .name = "mock2",
    .enter = mockEnter,
    .update = mockUpdate,
    .draw = mockDraw,
    .exit = mockExit,
};

smTestEnterFn smTestEnter;
smTestEnterWithArgsFn smTestEnterWithArgs;
smTestExitFn smTestExit;
MockData *smMockData;
MockArgs *smMockArgs;
struct timespec smMockCurrTime;


// -----------------------------------------------------------------------------
// Base Tests
// -----------------------------------------------------------------------------

void Test_smStop_SucceedsPostStart(void)
{
    assert(smStart());
    assert(smStop());
    tsInternalPass("Test_smStop_SucceedsPostStart");
}


// -----------------------------------------------------------------------------
// Pre-Start
// -----------------------------------------------------------------------------

// Start Related

void Test_smHasStarted_FailsPreStart(void)
{
    assert(!smIsRunning());
    tsInternalPass("Test_smHasStarted_FailsPreStart");
}

// State Functions

void Test_smCreateState_FailsPreStart(void)
{
    assert(!smCreateState(nullptr, nullptr, nullptr, nullptr, nullptr));
    tsInternalPass("Test_smCreateState_FailsPreStart");
}

void Test_smStateExists_FailsPreStart(void)
{
    assert(!smStateExists(nullptr));
    tsInternalPass("Test_smStateExists_FailsPreStart");
}

void Test_smSetState_FailsPreStart(void)
{
    assert(!smSetState(nullptr, nullptr));
    tsInternalPass("Test_smSetState_FailsPreStart");
}

void Test_smGetCurrentStateName_FailsPreStart(void)
{
    assert(!smGetCurrentStateName());
    tsInternalPass("Test_smGetCurrentStateName_FailsPreStart");
}

void Test_smDeleteState_FailsPreStart(void)
{
    assert(!smDeleteState(nullptr));
    tsInternalPass("Test_smDeleteState_FailsPreStart");
}

void Test_smGetStateCount_FailsPreStart(void)
{
    assert(smGetStateCount() == -1);
    tsInternalPass("Test_smGetStateCount_FailsPreStart");
}


// Lifecycle Functions

void Test_smUpdate_FailsPreStart(void)
{
    assert(!smUpdate(mockDt));
    tsInternalPass("Test_smUpdate_FailsPreStart");
}

void Test_smGetDt_FailsPreStart(void)
{
    assert(smGetDt() == -1.0f);
    tsInternalPass("Test_smGetDt_FailsPreStart");
}

void Test_smDraw_FailsPreStart(void)
{
    assert(!smDraw());
    tsInternalPass("Test_smDraw_FailsPreStart");
}

// Stop

void Test_smStop_FailsPreStart(void)
{
    assert(!smStop());
    tsInternalPass("Test_smStop_FailsPreStart");
}


// -----------------------------------------------------------------------------
// Start
// -----------------------------------------------------------------------------

// Mem Alloc Fail

void Test_smStart_FailsWhenCallocFails(void)
{
    tsInternalDisable(CALLOC, 1);
    assert(!smStart());
    tsInternalPass("Test_smStart_FailsWhenCallocFails");
}


// -----------------------------------------------------------------------------
// Post-Start
// -----------------------------------------------------------------------------

// Start Related

void Test_smStart_IsIdempotentPostStart(void)
{
    smStart();
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++)
    {
        assert(!smStart());
    }
    smStop();
    tsInternalPass("Test_smStart_IsIdempotentPostStart");
}

void Test_smHasStarted_SucceedsPostStart(void)
{
    smStart();
    assert(smIsRunning());
    smStop();
    tsInternalPass("Test_smHasStarted_SucceedsPostStart");
}

// State Functions

// -- smCreateState

// -- -- Name Related

void Test_smCreateState_AcceptsValidName(void)
{
    smStart();
    assert(smCreateState(mock.name, mock.enter, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_AcceptsValidName");
}

void Test_smCreateState_RejectsExistingName(void)
{
    smStart();
    smCreateState(mock.name, mock.enter, nullptr, nullptr, nullptr);
    assert(!smCreateState(mock.name, mock.enter, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_RejectsExistingName");
}

void Test_smCreateState_RejectsLongName(void)
{
    // TODO
}

void Test_smCreateState_RejectsNullName(void)
{
    smStart();
    assert(!smCreateState(nullptr, nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_RejectsNullName");
}

void Test_smCreateState_RejectsEmptyName(void)
{
    smStart();
    assert(!smCreateState("", nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_RejectsEmptyName");
}

void Test_smCreateState_RejectsWhitespaceOnlyName(void)
{
    // TODO
}

void Test_smCreateState_AcceptsAndTrimsLeadingWhitespaceOnName(void)
{
    // TODO
}

void Test_smCreateState_AcceptsAndTrimsTrailingWhitespaceOnName(void)
{
    // TODO
}

void Test_smCreateState_AcceptsAndTrimsLeadingAndTrailingWhitespaceOnName(void)
{
    // TODO
}

// -- -- State Functions Related

void Test_smCreateState_AcceptsValidNameAndNoNullFunctions(void)
{
    smStart();
    assert(
        smCreateState(mock.name, mock.enter, mock.update, mock.draw, mock.exit
        ));
    smStop();
    tsInternalPass("Test_smCreateState_AcceptsValidNameAndNoNullFunctions");
}

void Test_smCreateState_AcceptsValidNameAndNullEnter(void)
{
    smStart();
    assert(
        smCreateState(mock.name, nullptr, mock.update, mock.draw, mock.exit));
    smStop();
    tsInternalPass("Test_smCreateState_AcceptsValidNameAndNullEnter");
}

void Test_smCreateState_AcceptsValidNameAndNullUpdate(void)
{
    smStart();
    assert(smCreateState(mock.name, mock.enter, nullptr, mock.draw, mock.exit));
    smStop();
    tsInternalPass("Test_smCreateState_AcceptsValidNameAndNullUpdate");
}

void Test_smCreateState_AcceptsValidNameAndNullDraw(void)
{
    smStart();
    assert(
        smCreateState(mock.name, mock.enter, mock.update, nullptr, mock.exit));
    smStop();
    tsInternalPass("Test_smCreateState_AcceptsValidNameAndNullDraw");
}

void Test_smCreateState_AcceptsValidNameAndNullExit(void)
{
    smStart();
    assert(
        smCreateState(mock.name, mock.enter, mock.update, mock.draw, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_AcceptsValidNameAndNullExit");
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdate(void)
{
    smStart();
    assert(smCreateState(mock.name, nullptr,nullptr, mock.draw, mock.exit));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdate");
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDraw(void)
{
    smStart();
    assert(
        smCreateState(mock.name, nullptr, mock.update, nullptr, mock.exit
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDraw");
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullExit(void)
{
    smStart();
    assert(
        smCreateState(mock.name, nullptr, mock.update, mock.draw, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullEnterAndNullExit");
}

void Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDraw(void)
{
    smStart();
    assert(
        smCreateState(mock.name, mock.enter, nullptr, nullptr, mock.exit
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDraw");
}

void Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullExit(void)
{
    smStart();
    assert(
        smCreateState(mock.name, mock.enter, nullptr, mock.draw, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullExit");
}

void Test_smCreateState_AcceptsValidNameAndNullDrawAndNullExit(void)
{
    smStart();
    assert(
        smCreateState(mock.name, mock.enter, mock.update, nullptr, nullptr
        ));
    smStop();
    tsInternalPass("Test_smCreateState_AcceptsValidNameAndNullDrawAndNullExit");
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw(
    void)
{
    smStart();
    assert(
        smCreateState(mock.name, nullptr, nullptr, nullptr, mock.exit
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw");
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit(
    void)
{
    smStart();
    assert(
        smCreateState(mock.name, nullptr, nullptr, mock.draw, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit");
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit(
    void)
{
    smStart();
    assert(
        smCreateState(mock.name, nullptr, mock.update, nullptr, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit");
}

void Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit(
    void)
{
    smStart();
    assert(
        smCreateState(mock.name, mock.enter, nullptr, nullptr, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit");
}

void Test_smCreateState_RejectsValidNameAndAllNullFunctions(void)
{
    smStart();
    assert(!smCreateState(mock.name, nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_RejectsValidNameAndAllNullFunctions");
}

// smStateExists

void Test_smStateExists_AcceptsCreatedName(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(smStateExists(mock.name));
    smStop();
    tsInternalPass("Test_smStateExists_AcceptsCreatedName");
}

void Test_smStateExists_RejectsNonCreatedName(void)
{
    smStart();
    assert(!smStateExists(mock.name));
    smStop();
    tsInternalPass("Test_smStateExists_RejectsNonCreatedName");
}

// -- smSetState

void Test_smSetState_AcceptsValidStateFromNull(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(smSetState(mock.name, nullptr));
    smStop();
    tsInternalPass("Test_smSetState_AcceptsValidStateFromNull");
}

void Test_smSetState_RejectsNullName(void)
{
    smStart();
    assert(!smSetState(nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smSetState_RejectsNullName");
}

void Test_smSetState_RejectsNonCreatedName(void)
{
    smStart();
    assert(!smSetState(mock.name, nullptr));
    smStop();
    tsInternalPass("Test_smSetState_RejectsNonCreatedName");
}

void Test_smSetState_SucceedsChangingFromOneStateToAnotherWithNoArgs(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smCreateState(mock2.name, mockEnter, nullptr, nullptr, nullptr);
    smSetState(mock.name, nullptr);
    assert(smSetState(mock2.name, nullptr));
    smStop();
    tsInternalPass(
        "Test_smSetState_SucceedsChangingFromOneStateToAnotherWithNoArgs");
}

void Test_smSetState_CallsNonNullExitOfCurrentState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestExit = onExit;
    smMockData = mockData;

    smCreateState(mock.name, nullptr, nullptr, nullptr, mockExit);
    smCreateState(mock2.name, mockEnter, nullptr, nullptr, nullptr);

    smSetState(mock.name, nullptr);
    smSetState(mock2.name, nullptr);

    assert(mockData->exitCount == 1);
    free(mockData);
    smTestExit = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetState_CallsExitOfCurrentState");
}

void Test_smSetState_SkipsNullExitOfCurrentState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestExit = onExit;
    smMockData = mockData;

    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smCreateState(mock2.name, nullptr, nullptr, nullptr, mockExit);

    smSetState(mock.name, nullptr);
    smSetState(mock2.name, nullptr);

    assert(mockData->exitCount == 0);
    free(mockData);
    smTestExit = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetState_SkipsNullExitOfCurrentState");
}

void Test_smSetState_CallsNonNullEnterOfTargetState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestEnter = onEnter;
    smMockData = mockData;

    smCreateState(mock.name, nullptr, nullptr, nullptr, mockExit);
    smCreateState(mock2.name, mockEnter, nullptr, nullptr, nullptr);

    smSetState(mock.name, nullptr);
    smSetState(mock2.name, nullptr);

    assert(mockData->enterCount == 1);
    free(mockData);
    smTestEnter = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetState_CallsNonNullEnterOfTargetState");
}

void Test_smSetState_SkipsNullEnterOfTargetState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestEnter = onEnter;
    smMockData = mockData;

    smCreateState(mock.name, nullptr, mockUpdate, nullptr, nullptr);
    smCreateState(mock2.name, nullptr, nullptr, nullptr, mockExit);

    smSetState(mock.name, nullptr);
    smSetState(mock2.name, nullptr);

    assert(mockData->enterCount == 0);
    free(mockData);
    smTestEnter = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetState_SkipsNullEnterOfTargetState");
}

void Test_smSetState_CallsNonNullExitAndNonNullEnterWhenTargetingSameState(
    void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestEnter = onEnter;
    smTestExit = onExit;
    smMockData = mockData;

    smCreateState(mock.name, mockEnter, nullptr, nullptr, mockExit);

    smSetState(mock.name, nullptr);
    smSetState(mock.name, nullptr);

    assert(mockData->enterCount == 2);
    assert(mockData->exitCount == 1);

    free(mockData);
    smTestEnter = nullptr;
    smTestExit = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetState_CallsNonNullExitAndNonNullEnterWhenTargetingSameState");
}

void Test_smSetState_CallsNonNullEnterWithArgsOfTargetState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    MockArgs *mockArgs = calloc(1, sizeof(MockArgs));
    smTestEnterWithArgs = onEnterWithArgs;
    smMockData = mockData;
    smMockArgs = mockArgs;

    smCreateState(mock.name, nullptr, nullptr, nullptr, mockExit);
    smCreateState(mock2.name, mockEnter, nullptr, nullptr, nullptr);

    smSetState(mock.name, nullptr);
    smSetState(mock2.name, smMockArgs);

    assert(mockArgs->flag);

    free(mockData);
    free(mockArgs);
    smTestEnter = nullptr;
    smMockData = nullptr;
    smMockArgs = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetState_CallsEnterWithArgs");
}

// -- smGetCurrentStateName

void Test_smGetCurrentStateName_FailsPreCreateState(void)
{
    smStart();
    assert(!smGetCurrentStateName());
    smStop();
    tsInternalPass("Test_smGetCurrentStateName_FailsPreCreateState");
}

void Test_smGetCurrentStateName_ReturnsCurrentStateName(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smSetState(mock.name, nullptr);
    assert(strcmp(smGetCurrentStateName(), mock.name) == 0);
    smStop();
    tsInternalPass("Test_smGetCurrentStateName_ReturnsCurrentStateName");
}

// -- smDeleteState

void Test_smDeleteState_FailsPreCreateState(void)
{
    smStart();
    assert(!smDeleteState(mock.name));
    smStop();
    tsInternalPass("Test_smDeleteState_FailsPreCreateState");
}

void Test_smDeleteState_FailsToDeleteCurrentState(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smSetState(mock.name, nullptr);
    assert(!smDeleteState(mock.name));
    smStop();
    tsInternalPass("Test_smDeleteState_FailsToDeleteCurrentState");
}

void Test_smDeleteState_AcceptsNonCurrentState(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(!smDeleteState(mock.name));
    smStop();
    tsInternalPass("Test_smDeleteState_FailsToDeleteCurrentState");
}

// -- smGetStateCount

void Test_smGetStateCount_ReturnsZeroPostStart(void)
{
    smStart();
    assert(smGetStateCount() == 0);
    smStop();
    tsInternalPass("Test_smGetStateCount_ReturnsZeroPostStart");
}

void Test_smGetStateCount_ReturnsCorrectStateCountPostCreateState(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(smGetStateCount() == 1);
    smStop();
    tsInternalPass(
        "Test_smGetStateCount_ReturnsCorrectStateCountPostCreateState");
}

void Test_smGetStateCount_ReturnsCorrectStateCountPostDeleteState(void)
{
    smStart();
    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smDeleteState(mock.name);
    assert(smGetStateCount() == 0);
    smStop();
    tsInternalPass(
        "Test_smGetStateCount_ReturnsCorrectStateCountPostDeleteState");
}

// Lifecycle Functions

// -- smUpdate

void Test_smUpdate_FailsWhenNullCurrentState(void)
{
    smStart();
    assert(!smGetCurrentStateName());
    assert(!smUpdate(mockDt));
    smStop();
    tsInternalPass("Test_smUpdate_FailsWhenNullCurrentState");
}

void Test_smUpdate_CallsNonNullUpdateOfCurrentState(void)
{
    smStart();
    smCreateState(mock.name, nullptr, mock.update, nullptr, nullptr);
    smSetState(mock.name, nullptr);
    assert(smUpdate(mockDt));
    smStop();
    tsInternalPass("Test_smUpdate_CallsNonNullUpdateOfCurrentState");
}

void Test_smUpdate_FailsWhenNullUpdate(void)
{
    smStart();
    smCreateState(mock.name, mock.enter, nullptr, nullptr, nullptr);
    smSetState(mock.name, nullptr);
    assert(!smUpdate(mockDt));
    smStop();
    tsInternalPass("Test_smUpdate_FailsWhenNullUpdate");
}

// -- smGetDt

void Test_smGetDt_UsesDefaultDtOnFirstCall(void)
{
    smStart();
    const float target = 1.0 / DEFAULT_FPS;
    assert(smGetDt() == target);
    smStop();
    tsInternalPass("Test_smGetDt_UsesDefaultDtOnFirstCall");
}

void Test_smGetDt_UpdatesDtOnConsecutiveCalls(void)
{
    smStart();

    // Arbitrary time for first smGetDt call
    smMockCurrTime.tv_nsec = SM_GET_DT_FIRST_CALL_TIME_NS;
    smMockCurrTime.tv_sec = 0;

    smGetDt(); // First call uses default dt

    for (int i = 0; i < FRAME_TIME_ITERATIONS; i++)
    {
        smMockCurrTime.tv_nsec += EXPECTED_DT * NANO_SEC_PER_SEC;
        if (smMockCurrTime.tv_nsec >= NANO_SEC_PER_SEC)
        {
            smMockCurrTime.tv_sec += smMockCurrTime.tv_nsec / NANO_SEC_PER_SEC;
            smMockCurrTime.tv_nsec %= NANO_SEC_PER_SEC;
        }
        assert(fabsf(smGetDt() - EXPECTED_DT) < DT_TOLERANCE);
    }

    smStop();
    tsInternalPass("Test_smGetDt_UpdatesDtOnConsecutiveCalls");
}


// -- smDraw

void Test_smDraw_FailsWhenNullCurrentState(void)
{
    smStart();
    assert(!smGetCurrentStateName());
    assert(!smDraw());
    smStop();
    tsInternalPass("Test_smDraw_FailsWhenNullCurrentState");
}

void Test_smDraw_CallsValidDrawFunction(void)
{
    smStart();
    smCreateState(mock.name, nullptr, nullptr, mockDraw, nullptr);
    smSetState(mock.name, nullptr);
    assert(smDraw());
    smStop();
    tsInternalPass("Test_smDraw_CallsValidDrawFunction");
}

void Test_smDraw_FailsWhenNullDraw(void)
{
    smStart();
    smCreateState(mock.name, mock.enter, nullptr, nullptr, nullptr);
    smSetState(mock.name, nullptr);
    assert(!smDraw());
    smStop();
    tsInternalPass("Test_smDraw_FailsWhenNullDraw");
}

// Stop Related

void Test_smStop_CallsNonNullExitOfCurrentState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestExit = onExit;
    smMockData = mockData;

    smCreateState(mock.name, nullptr, nullptr, nullptr, mockExit);
    smSetState(mock.name, nullptr);

    smStop();

    assert(mockData->exitCount == 1);

    free(mockData);
    smTestExit = nullptr;
    smMockData = nullptr;

    tsInternalPass(
        "Test_smStop_CallsExitOfCurrentState");
}

void Test_smStop_SkipsNullExitOfCurrentState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestExit = onExit;
    smMockData = mockData;

    smCreateState(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smSetState(mock.name, nullptr);

    smStop();

    assert(mockData->exitCount == 0);

    free(mockData);
    smTestExit = nullptr;
    smMockData = nullptr;

    tsInternalPass(
        "Test_smStop_SkipsNullExitOfCurrentState");
}


// -----------------------------------------------------------------------------
// Post-Stop
// -----------------------------------------------------------------------------

// Start Related

void Test_smHasStarted_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smIsRunning());
    tsInternalPass("Test_smHasStarted_FailsPostStop");
}

// State Functions

void Test_smCreateState_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smCreateState(nullptr, nullptr, nullptr,nullptr,nullptr));
    tsInternalPass("Test_smCreateState_FailsPostStop");
}

void Test_smStateExists_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smStateExists(nullptr));
    tsInternalPass("Test_smStateExists_FailsPostStop");
}

void Test_smSetState_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smSetState(nullptr, nullptr));
    tsInternalPass("Test_smSetState_FailsPostStop");
}

void Test_smGetCurrentStateName_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smGetCurrentStateName());
    tsInternalPass("Test_smGetCurrentStateName_FailsPostStop");
}

void Test_smDeleteState_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smDeleteState(nullptr));
    tsInternalPass("Test_smDeleteState_FailsPostStop");
}

void Test_smGetStateCount_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(smGetStateCount() == -1);
    tsInternalPass("Test_smGetStateCount_FailsPostStop");
}

// Lifecycle Functions

void Test_smUpdate_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smUpdate(mockDt));
    tsInternalPass("Test_smUpdate_FailsPostStop");
}

void Test_smDraw_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smDraw());
    tsInternalPass("Test_smDraw_FailsPostStop");
}

// Stop Related

void Test_smStop_IsIdempotentPostStop(void)
{
    smStart();
    smStop();
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++)
    {
        assert(!smStop());
    }
    tsInternalPass("Test_smStop_IsIdempotentPostStop");
}


// --------------------------------------------------
// Stress tests
// --------------------------------------------------

// State Functions

void TestStress_smCreateState_CreatingMultipleStatesCausesNoSkips(void)
{
    smStart();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smCreateState(buf, mockEnter, nullptr, nullptr, nullptr);
    }
    assert(smGetStateCount() == STRESS_ITERATIONS);
    smStop();
    tsInternalPass(
        "TestStress_smCreateState_CreatingMultipleStatesCausesNoSkips");
}

void TestStress_smSetState_SettingStatesOftenCausesNoSkips(void)
{
    smStart();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smCreateState(buf, mockEnter, nullptr, nullptr, nullptr);
    }
    int counter = 0;
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smSetState(buf, nullptr);
        const int stateNum = atoi(smGetCurrentStateName());
        assert(stateNum == counter);
        counter++;
    }
    assert(counter == STRESS_ITERATIONS);
    smStop();
    tsInternalPass("TestStress_smSetState_SettingStatesOftenCausesNoSkips");
}

// Stop Related

void TestStress_smStop_FreeingMultipleStatesCausesNoSkips(void)
{
    smStart();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smCreateState(buf, mockEnter, nullptr, nullptr, nullptr);
    }
    assert(smStop());
    tsInternalPass("TestStress_smStop_FreeingMultipleStatesCausesNoSkips");
}


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main()
{
    puts("\nBASE TESTS");
    Test_smStop_SucceedsPostStart();

    puts("\nPRE-START TESTING");
    puts("• Start Related");
    Test_smHasStarted_FailsPreStart();
    puts("• State Functions");
    Test_smCreateState_FailsPreStart();
    Test_smStateExists_FailsPreStart();
    Test_smSetState_FailsPreStart();
    Test_smGetCurrentStateName_FailsPreStart();
    Test_smDeleteState_FailsPreStart();
    Test_smGetStateCount_FailsPreStart();
    puts("• Lifecycle Functions");
    Test_smUpdate_FailsPreStart();
    Test_smGetDt_FailsPreStart();
    Test_smDraw_FailsPreStart();
    puts("• Stop Related");
    Test_smStop_FailsPreStart();

    puts("\nSTART TESTING");
    puts("• Mem Alloc Fail");
    Test_smStart_FailsWhenCallocFails();

    puts("\nPOST-START TESTING");
    puts("• Start Related");
    Test_smStart_IsIdempotentPostStart();
    Test_smHasStarted_SucceedsPostStart();
    puts("• State Functions");
    puts(" • smCreateState");
    puts("  • Name Related");
    Test_smCreateState_AcceptsValidName();
    Test_smCreateState_RejectsExistingName();
    Test_smCreateState_RejectsLongName();
    Test_smCreateState_RejectsNullName();
    Test_smCreateState_RejectsEmptyName();

    // Test_smCreateState_RejectsWhitespaceOnlyStateName();
    // TODO create more tests for different types of whitespace (\n, \t, \r...)
    // Test_smCreateState_AcceptsLeadingWhitespaceOnStateName();
    // Test_smCreateState_AcceptsTrailingWhitespaceOnStateName();
    // Test_smCreateState_AcceptsLeadingAndTrailingWhitespaceOnStateName();
    // Test_smCreateState_TrimsLeadingWhitespaceOnStateName();
    // Test_smCreateState_TrimsTrailingWhitespaceOnStateName();
    // Test_smCreateState_TrimsLeadingAndTrailingWhitespaceOnStateName();

    puts("  • State Functions Related");
    Test_smCreateState_AcceptsValidNameAndNoNullFunctions();
    Test_smCreateState_AcceptsValidNameAndNullEnter();
    Test_smCreateState_AcceptsValidNameAndNullUpdate();
    Test_smCreateState_AcceptsValidNameAndNullDraw();
    Test_smCreateState_AcceptsValidNameAndNullExit();
    Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdate();
    Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDraw();
    Test_smCreateState_AcceptsValidNameAndNullEnterAndNullExit();
    Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDraw();
    Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullExit();
    Test_smCreateState_AcceptsValidNameAndNullDrawAndNullExit();
    Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw();
    Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit();
    Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit();
    Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit();
    Test_smCreateState_RejectsValidNameAndAllNullFunctions();
    puts(" • smStateExists");
    Test_smStateExists_AcceptsCreatedName();
    Test_smStateExists_RejectsNonCreatedName();
    puts(" • smSetState");
    Test_smSetState_AcceptsValidStateFromNull();
    Test_smSetState_RejectsNullName();
    Test_smSetState_RejectsNonCreatedName();
    Test_smSetState_SucceedsChangingFromOneStateToAnotherWithNoArgs();
    Test_smSetState_CallsNonNullExitOfCurrentState();
    Test_smSetState_SkipsNullExitOfCurrentState();
    Test_smSetState_CallsNonNullEnterOfTargetState();
    Test_smSetState_SkipsNullEnterOfTargetState();
    Test_smSetState_CallsNonNullExitAndNonNullEnterWhenTargetingSameState();
    Test_smSetState_CallsNonNullEnterWithArgsOfTargetState();
    puts(" • smGetCurrentStateName");
    Test_smGetCurrentStateName_FailsPreCreateState();
    Test_smGetCurrentStateName_ReturnsCurrentStateName();
    puts(" • smDeleteState");
    Test_smDeleteState_FailsPreCreateState();
    puts(" • smGetStateCount");
    Test_smGetStateCount_ReturnsZeroPostStart();
    Test_smGetStateCount_ReturnsCorrectStateCountPostCreateState();
    Test_smGetStateCount_ReturnsCorrectStateCountPostDeleteState();
    puts("• Lifecycle Functions");
    puts(" • smUpdate");
    Test_smUpdate_FailsWhenNullCurrentState();
    Test_smUpdate_CallsNonNullUpdateOfCurrentState();
    Test_smUpdate_FailsWhenNullUpdate();
    puts(" • smGetDt");
    Test_smGetDt_UsesDefaultDtOnFirstCall();
    Test_smGetDt_UpdatesDtOnConsecutiveCalls();
    puts(" • smDraw");
    Test_smDraw_FailsWhenNullCurrentState();
    Test_smDraw_CallsValidDrawFunction();
    Test_smDraw_FailsWhenNullDraw();

    puts("\nSTOP TESTING");
    Test_smStop_CallsNonNullExitOfCurrentState();
    Test_smStop_SkipsNullExitOfCurrentState();
    // TODO Check for no memory leaks

    puts("\nPOST-STOP TESTING");
    puts("• Start Related");
    Test_smHasStarted_FailsPostStop();
    puts("• State Functions");
    Test_smCreateState_FailsPostStop();
    Test_smStateExists_FailsPostStop();
    Test_smSetState_FailsPostStop();
    Test_smGetCurrentStateName_FailsPostStop();
    Test_smDeleteState_FailsPostStop();
    Test_smGetStateCount_FailsPostStop();
    puts("• Lifecycle Functions");
    Test_smUpdate_FailsPostStop();
    Test_smDraw_FailsPostStop();
    puts("• Stop Related");
    Test_smStop_IsIdempotentPostStop();

    puts("\nSTRESS TESTING");
    TestStress_smCreateState_CreatingMultipleStatesCausesNoSkips();
    TestStress_smSetState_SettingStatesOftenCausesNoSkips();
    TestStress_smStop_FreeingMultipleStatesCausesNoSkips();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
    return 0;
}
