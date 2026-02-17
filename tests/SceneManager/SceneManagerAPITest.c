/**
 * @file
 * @brief Implementation of the SceneManager API Tests.
 *
 * @see SceneManagerAPITests.h
 *
 * @bug No known bugs.
 *
 * @author Vitor Betmann
 * @date 2025-11-03
 * @version 1.0.0
 */

// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "SceneManagerAPITest.h"

#include "SceneManagerInternal.h"
#include "SceneManager.h"
#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————
// Mock & Test Functions
// —————————————————————————————————————————————————————————————————————————————

static void mockEnter(void *args)
{
    // Mock Enter
}

static void onEnter(MockData *data)
{
    data->enterCount++;
}

static void onEnterWithArgs(MockData *data, MockArgs *args)
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

static void onExit(MockData *data)
{
    data->exitCount++;
}

// —————————————————————————————————————————————————————————————————————————————
// variables
// —————————————————————————————————————————————————————————————————————————————

static float mockDt = 0.016f;

static smInternalScene mock = {
    .name = "mock",
    .enter = mockEnter,
    .update = mockUpdate,
    .draw = mockDraw,
    .exit = mockExit,
};

static smInternalScene mock2 = {
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


// —————————————————————————————————————————————————————————————————————————————
// Base Tests
// —————————————————————————————————————————————————————————————————————————————

void Test_smStop_SucceedsPostStart(void)
{
    assert(smStart());
    assert(smStop());
    tsInternalPass("Test_smStop_SucceedsPostStart");
}


// —————————————————————————————————————————————————————————————————————————————
// Pre-Start
// —————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smHasStarted_FailsPreStart(void)
{
    assert(!smIsRunning());
    tsInternalPass("Test_smHasStarted_FailsPreStart");
}

// State Functions

void Test_smCreateScene_FailsPreStart(void)
{
    assert(!smCreateScene(nullptr, nullptr, nullptr, nullptr, nullptr));
    tsInternalPass("Test_smCreateScene_FailsPreStart");
}

void Test_smSceneExists_FailsPreStart(void)
{
    assert(!smSceneExists(nullptr));
    tsInternalPass("Test_smSceneExists_FailsPreStart");
}

void Test_smSetScene_FailsPreStart(void)
{
    assert(!smSetScene(nullptr, nullptr));
    tsInternalPass("Test_smSetScene_FailsPreStart");
}

void Test_smGetCurrentSceneName_FailsPreStart(void)
{
    assert(!smGetCurrentSceneName());
    tsInternalPass("Test_smGetCurrentSceneName_FailsPreStart");
}

void Test_smDeleteScene_FailsPreStart(void)
{
    assert(!smDeleteScene(nullptr));
    tsInternalPass("Test_smDeleteScene_FailsPreStart");
}

void Test_smGetSceneCount_FailsPreStart(void)
{
    assert(smGetSceneCount() == -1);
    tsInternalPass("Test_smGetSceneCount_FailsPreStart");
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


// —————————————————————————————————————————————————————————————————————————————
// Start
// —————————————————————————————————————————————————————————————————————————————

// Mem Alloc Fail

void Test_smStart_FailsWhenCallocFails(void)
{
    tsInternalDisable(CALLOC, 1);
    assert(!smStart());
    tsInternalPass("Test_smStart_FailsWhenCallocFails");
}


// —————————————————————————————————————————————————————————————————————————————
// Post-Start
// —————————————————————————————————————————————————————————————————————————————

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

// -- smCreateScene

// -- -- Name Related

void Test_smCreateScene_AcceptsValidName(void)
{
    smStart();
    assert(smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateScene_AcceptsValidName");
}

void Test_smCreateScene_RejectsExistingName(void)
{
    smStart();
    smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr);
    assert(!smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateScene_RejectsExistingName");
}

void Test_smCreateScene_RejectsNullName(void)
{
    smStart();
    assert(!smCreateScene(nullptr, nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateScene_RejectsNullName");
}

void Test_smCreateScene_RejectsEmptyName(void)
{
    smStart();
    assert(!smCreateScene("", nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateScene_RejectsEmptyName");
}

// -- -- State Functions Related

void Test_smCreateScene_AcceptsValidNameAndNoNullFunctions(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, mock.draw, mock.exit
        ));
    smStop();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNoNullFunctions");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnter(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, mock.draw, mock.exit));
    smStop();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullEnter");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdate(void)
{
    smStart();
    assert(smCreateScene(mock.name, mock.enter, nullptr, mock.draw, mock.exit));
    smStop();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullUpdate");
}

void Test_smCreateScene_AcceptsValidNameAndNullDraw(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, nullptr, mock.exit));
    smStop();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullExit(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, mock.draw, nullptr));
    smStop();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdate(void)
{
    smStart();
    assert(smCreateScene(mock.name, nullptr,nullptr, mock.draw, mock.exit));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdate");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDraw(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, nullptr, mock.exit
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullExit(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, mock.draw, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDraw(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, mock.exit
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullExit(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, mock.draw, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullDrawAndNullExit(void)
{
    smStart();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, nullptr, nullptr
        ));
    smStop();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullDrawAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw(
    void)
{
    smStart();
    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, mock.exit
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit(
    void)
{
    smStart();
    assert(
        smCreateScene(mock.name, nullptr, nullptr, mock.draw, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit(
    void)
{
    smStart();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, nullptr, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit(
    void)
{
    smStart();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr
        ));
    smStop();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit");
}

void Test_smCreateScene_RejectsValidNameAndAllNullFunctions(void)
{
    smStart();
    assert(!smCreateScene(mock.name, nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateScene_RejectsValidNameAndAllNullFunctions");
}

// smSceneExists

void Test_smSceneExists_AcceptsCreatedName(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(smSceneExists(mock.name));
    smStop();
    tsInternalPass("Test_smSceneExists_AcceptsCreatedName");
}

void Test_smSceneExists_RejectsNonCreatedName(void)
{
    smStart();
    assert(!smSceneExists(mock.name));
    smStop();
    tsInternalPass("Test_smSceneExists_RejectsNonCreatedName");
}

// -- smSetScene

void Test_smSetScene_AcceptsValidStateFromNull(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(smSetScene(mock.name, nullptr));
    smStop();
    tsInternalPass("Test_smSetScene_AcceptsValidStateFromNull");
}

void Test_smSetScene_RejectsNullName(void)
{
    smStart();
    assert(!smSetScene(nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smSetScene_RejectsNullName");
}

void Test_smSetScene_RejectsNonCreatedName(void)
{
    smStart();
    assert(!smSetScene(mock.name, nullptr));
    smStop();
    tsInternalPass("Test_smSetScene_RejectsNonCreatedName");
}

void Test_smSetScene_SucceedsChangingFromOneStateToAnotherWithNoArgs(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr);
    smSetScene(mock.name, nullptr);
    assert(smSetScene(mock2.name, nullptr));
    smStop();
    tsInternalPass(
        "Test_smSetScene_SucceedsChangingFromOneStateToAnotherWithNoArgs");
}

void Test_smSetScene_CallsNonNullExitOfCurrentState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestExit = onExit;
    smMockData = mockData;

    smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit);
    smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr);

    smSetScene(mock.name, nullptr);
    smSetScene(mock2.name, nullptr);

    assert(mockData->exitCount == 1);
    free(mockData);
    smTestExit = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetScene_CallsExitOfCurrentState");
}

void Test_smSetScene_SkipsNullExitOfCurrentState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestExit = onExit;
    smMockData = mockData;

    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smCreateScene(mock2.name, nullptr, nullptr, nullptr, mockExit);

    smSetScene(mock.name, nullptr);
    smSetScene(mock2.name, nullptr);

    assert(mockData->exitCount == 0);
    free(mockData);
    smTestExit = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetScene_SkipsNullExitOfCurrentState");
}

void Test_smSetScene_CallsNonNullEnterOfTargetState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestEnter = onEnter;
    smMockData = mockData;

    smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit);
    smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr);

    smSetScene(mock.name, nullptr);
    smSetScene(mock2.name, nullptr);

    assert(mockData->enterCount == 1);
    free(mockData);
    smTestEnter = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetScene_CallsNonNullEnterOfTargetState");
}

void Test_smSetScene_SkipsNullEnterOfTargetState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestEnter = onEnter;
    smMockData = mockData;

    smCreateScene(mock.name, nullptr, mockUpdate, nullptr, nullptr);
    smCreateScene(mock2.name, nullptr, nullptr, nullptr, mockExit);

    smSetScene(mock.name, nullptr);
    smSetScene(mock2.name, nullptr);

    assert(mockData->enterCount == 0);
    free(mockData);
    smTestEnter = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetScene_SkipsNullEnterOfTargetState");
}

void Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameState(
    void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    smTestEnter = onEnter;
    smTestExit = onExit;
    smMockData = mockData;

    smCreateScene(mock.name, mockEnter, nullptr, nullptr, mockExit);

    smSetScene(mock.name, nullptr);
    smSetScene(mock.name, nullptr);

    assert(mockData->enterCount == 2);
    assert(mockData->exitCount == 1);

    free(mockData);
    smTestEnter = nullptr;
    smTestExit = nullptr;
    smMockData = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameState");
}

void Test_smSetScene_CallsNonNullEnterWithArgsOfTargetState(void)
{
    smStart();

    MockData *mockData = calloc(1, sizeof(MockData));
    MockArgs *mockArgs = calloc(1, sizeof(MockArgs));
    smTestEnterWithArgs = onEnterWithArgs;
    smMockData = mockData;
    smMockArgs = mockArgs;

    smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit);
    smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr);

    smSetScene(mock.name, nullptr);
    smSetScene(mock2.name, smMockArgs);

    assert(mockArgs->flag);

    free(mockData);
    free(mockArgs);
    smTestEnter = nullptr;
    smMockData = nullptr;
    smMockArgs = nullptr;

    smStop();
    tsInternalPass(
        "Test_smSetScene_CallsEnterWithArgs");
}

// -- smGetCurrentSceneName

void Test_smGetCurrentSceneName_FailsPreCreateScene(void)
{
    smStart();
    assert(!smGetCurrentSceneName());
    smStop();
    tsInternalPass("Test_smGetCurrentSceneName_FailsPreCreateScene");
}

void Test_smGetCurrentSceneName_ReturnsCurrentStateName(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smSetScene(mock.name, nullptr);
    assert(strcmp(smGetCurrentSceneName(), mock.name) == 0);
    smStop();
    tsInternalPass("Test_smGetCurrentSceneName_ReturnsCurrentStateName");
}

// -- smDeleteScene

void Test_smDeleteScene_FailsPreCreateScene(void)
{
    smStart();
    assert(!smDeleteScene(mock.name));
    smStop();
    tsInternalPass("Test_smDeleteScene_FailsPreCreateScene");
}

void Test_smDeleteScene_FailsToDeleteCurrentState(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smSetScene(mock.name, nullptr);
    assert(!smDeleteScene(mock.name));
    smStop();
    tsInternalPass("Test_smDeleteScene_FailsToDeleteCurrentState");
}

void Test_smDeleteScene_AcceptsNonCurrentState(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(!smDeleteScene(mock.name));
    smStop();
    tsInternalPass("Test_smDeleteScene_FailsToDeleteCurrentState");
}

// -- smGetSceneCount

void Test_smGetSceneCount_ReturnsZeroPostStart(void)
{
    smStart();
    assert(smGetSceneCount() == 0);
    smStop();
    tsInternalPass("Test_smGetSceneCount_ReturnsZeroPostStart");
}

void Test_smGetSceneCount_ReturnsCorrectStateCountPostCreateScene(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    assert(smGetSceneCount() == 1);
    smStop();
    tsInternalPass(
        "Test_smGetSceneCount_ReturnsCorrectStateCountPostCreateScene");
}

void Test_smGetSceneCount_ReturnsCorrectStateCountPostDeleteScene(void)
{
    smStart();
    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smDeleteScene(mock.name);
    assert(smGetSceneCount() == 0);
    smStop();
    tsInternalPass(
        "Test_smGetSceneCount_ReturnsCorrectStateCountPostDeleteScene");
}

// Lifecycle Functions

// -- smUpdate

void Test_smUpdate_FailsWhenNullCurrentState(void)
{
    smStart();
    assert(!smGetCurrentSceneName());
    assert(!smUpdate(mockDt));
    smStop();
    tsInternalPass("Test_smUpdate_FailsWhenNullCurrentState");
}

void Test_smUpdate_CallsNonNullUpdateOfCurrentState(void)
{
    smStart();
    smCreateScene(mock.name, nullptr, mock.update, nullptr, nullptr);
    smSetScene(mock.name, nullptr);
    assert(smUpdate(mockDt));
    smStop();
    tsInternalPass("Test_smUpdate_CallsNonNullUpdateOfCurrentState");
}

void Test_smUpdate_FailsWhenNullUpdate(void)
{
    smStart();
    smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr);
    smSetScene(mock.name, nullptr);
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
        smMockCurrTime.tv_nsec += EXPECTED_DT * NANOSEC_PER_SEC;
        if (smMockCurrTime.tv_nsec >= NANOSEC_PER_SEC)
        {
            smMockCurrTime.tv_sec += smMockCurrTime.tv_nsec / NANOSEC_PER_SEC;
            smMockCurrTime.tv_nsec %= NANOSEC_PER_SEC;
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
    assert(!smGetCurrentSceneName());
    assert(!smDraw());
    smStop();
    tsInternalPass("Test_smDraw_FailsWhenNullCurrentState");
}

void Test_smDraw_CallsValidDrawFunction(void)
{
    smStart();
    smCreateScene(mock.name, nullptr, nullptr, mockDraw, nullptr);
    smSetScene(mock.name, nullptr);
    assert(smDraw());
    smStop();
    tsInternalPass("Test_smDraw_CallsValidDrawFunction");
}

void Test_smDraw_FailsWhenNullDraw(void)
{
    smStart();
    smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr);
    smSetScene(mock.name, nullptr);
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

    smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit);
    smSetScene(mock.name, nullptr);

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

    smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr);
    smSetScene(mock.name, nullptr);

    smStop();

    assert(mockData->exitCount == 0);

    free(mockData);
    smTestExit = nullptr;
    smMockData = nullptr;

    tsInternalPass(
        "Test_smStop_SkipsNullExitOfCurrentState");
}


// —————————————————————————————————————————————————————————————————————————————
// Post-Stop
// —————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smHasStarted_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smIsRunning());
    tsInternalPass("Test_smHasStarted_FailsPostStop");
}

// State Functions

void Test_smCreateScene_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smCreateScene(nullptr, nullptr, nullptr,nullptr,nullptr));
    tsInternalPass("Test_smCreateScene_FailsPostStop");
}

void Test_smSceneExists_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smSceneExists(nullptr));
    tsInternalPass("Test_smSceneExists_FailsPostStop");
}

void Test_smSetScene_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smSetScene(nullptr, nullptr));
    tsInternalPass("Test_smSetScene_FailsPostStop");
}

void Test_smGetCurrentSceneName_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smGetCurrentSceneName());
    tsInternalPass("Test_smGetCurrentSceneName_FailsPostStop");
}

void Test_smDeleteScene_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(!smDeleteScene(nullptr));
    tsInternalPass("Test_smDeleteScene_FailsPostStop");
}

void Test_smGetSceneCount_FailsPostStop(void)
{
    smStart();
    smStop();
    assert(smGetSceneCount() == -1);
    tsInternalPass("Test_smGetSceneCount_FailsPostStop");
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

void TestStress_smCreateScene_CreatingMultipleStatesCausesNoSkips(void)
{
    smStart();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smCreateScene(buf, mockEnter, nullptr, nullptr, nullptr);
    }
    assert(smGetSceneCount() == STRESS_ITERATIONS);
    smStop();
    tsInternalPass(
        "TestStress_smCreateScene_CreatingMultipleStatesCausesNoSkips");
}

void TestStress_smSetScene_SettingStatesOftenCausesNoSkips(void)
{
    smStart();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smCreateScene(buf, mockEnter, nullptr, nullptr, nullptr);
    }
    int counter = 0;
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smSetScene(buf, nullptr);
        int stateNum = atoi(smGetCurrentSceneName());
        assert(stateNum == counter);
        counter++;
    }
    assert(counter == STRESS_ITERATIONS);
    smStop();
    tsInternalPass("TestStress_smSetScene_SettingStatesOftenCausesNoSkips");
}

// Stop Related

void TestStress_smStop_FreeingMultipleStatesCausesNoSkips(void)
{
    smStart();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        smCreateScene(buf, mockEnter, nullptr, nullptr, nullptr);
    }
    assert(smStop());
    tsInternalPass("TestStress_smStop_FreeingMultipleStatesCausesNoSkips");
}


// —————————————————————————————————————————————————————————————————————————————
// Main
// —————————————————————————————————————————————————————————————————————————————

int main()
{
    puts("\nBASE TESTS");
    Test_smStop_SucceedsPostStart();

    puts("\nPRE-START TESTING");
    puts("• Start Related");
    Test_smHasStarted_FailsPreStart();
    puts("• State Functions");
    Test_smCreateScene_FailsPreStart();
    Test_smSceneExists_FailsPreStart();
    Test_smSetScene_FailsPreStart();
    Test_smGetCurrentSceneName_FailsPreStart();
    Test_smDeleteScene_FailsPreStart();
    Test_smGetSceneCount_FailsPreStart();
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
    puts(" • smCreateScene");
    puts("  • Name Related");
    Test_smCreateScene_AcceptsValidName();
    Test_smCreateScene_RejectsExistingName();
    Test_smCreateScene_RejectsNullName();
    Test_smCreateScene_RejectsEmptyName();

    puts("  • State Functions Related");
    Test_smCreateScene_AcceptsValidNameAndNoNullFunctions();
    Test_smCreateScene_AcceptsValidNameAndNullEnter();
    Test_smCreateScene_AcceptsValidNameAndNullUpdate();
    Test_smCreateScene_AcceptsValidNameAndNullDraw();
    Test_smCreateScene_AcceptsValidNameAndNullExit();
    Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdate();
    Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDraw();
    Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullExit();
    Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDraw();
    Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullExit();
    Test_smCreateScene_AcceptsValidNameAndNullDrawAndNullExit();
    Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw();
    Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit();
    Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit();
    Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit();
    Test_smCreateScene_RejectsValidNameAndAllNullFunctions();
    puts(" • smSceneExists");
    Test_smSceneExists_AcceptsCreatedName();
    Test_smSceneExists_RejectsNonCreatedName();
    puts(" • smSetScene");
    Test_smSetScene_AcceptsValidStateFromNull();
    Test_smSetScene_RejectsNullName();
    Test_smSetScene_RejectsNonCreatedName();
    Test_smSetScene_SucceedsChangingFromOneStateToAnotherWithNoArgs();
    Test_smSetScene_CallsNonNullExitOfCurrentState();
    Test_smSetScene_SkipsNullExitOfCurrentState();
    Test_smSetScene_CallsNonNullEnterOfTargetState();
    Test_smSetScene_SkipsNullEnterOfTargetState();
    Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameState();
    Test_smSetScene_CallsNonNullEnterWithArgsOfTargetState();
    puts(" • smGetCurrentSceneName");
    Test_smGetCurrentSceneName_FailsPreCreateScene();
    Test_smGetCurrentSceneName_ReturnsCurrentStateName();
    puts(" • smDeleteScene");
    Test_smDeleteScene_FailsPreCreateScene();
    puts(" • smGetSceneCount");
    Test_smGetSceneCount_ReturnsZeroPostStart();
    Test_smGetSceneCount_ReturnsCorrectStateCountPostCreateScene();
    Test_smGetSceneCount_ReturnsCorrectStateCountPostDeleteScene();
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

    puts("\nPOST-STOP TESTING");
    puts("• Start Related");
    Test_smHasStarted_FailsPostStop();
    puts("• State Functions");
    Test_smCreateScene_FailsPostStop();
    Test_smSceneExists_FailsPostStop();
    Test_smSetScene_FailsPostStop();
    Test_smGetCurrentSceneName_FailsPostStop();
    Test_smDeleteScene_FailsPostStop();
    Test_smGetSceneCount_FailsPostStop();
    puts("• Lifecycle Functions");
    Test_smUpdate_FailsPostStop();
    Test_smDraw_FailsPostStop();
    puts("• Stop Related");
    Test_smStop_IsIdempotentPostStop();

    puts("\nSTRESS TESTING");
    TestStress_smCreateScene_CreatingMultipleStatesCausesNoSkips();
    TestStress_smSetScene_SettingStatesOftenCausesNoSkips();
    TestStress_smStop_FreeingMultipleStatesCausesNoSkips();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
    return 0;
}
