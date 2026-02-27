/**
 * @file
 * @brief Implementation of the SceneManager API Tests.
 *
 * @author Vitor Betmann
 */


// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SceneManager.h"
#include "SceneManagerInternal.h"
#include "CommonInternal.h"
#include "SceneManagerTestHooks.h"
#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#ifdef NDEBUG
#error "SceneManagerAPITest must be compiled without NDEBUG (asserts required)."
#endif

#define NS_PER_S 1000000000L

#define DT_TOLERANCE 1e-6f
#define EXPECTED_DT_NS 16667000L
#define EXPECTED_DT_S  0.016667f

#define FRAME_TIME_ITERATIONS 300
#define IDEMPOTENT_ITERATIONS 3
#define STRESS_ITERATIONS 1000


// —————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————

// Mock

static void mockEnter(void *args)
{
    // Mock Enter
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

// Callbacks

static void onEnter(MockData *data)
{
    data->enterCount++;
}

static void onEnterWithArgs(MockData *data, MockArgs *args)
{
    data->enterCount++;
    args->flag = true;
}

static void onExit(MockData *data)
{
    data->exitCount++;
}

// Fixtures

static void resetHooks(void)
{
    smTestEnter = nullptr;
    smTestEnterWithArgs = nullptr;
    smTestExit = nullptr;
    smMockData = nullptr;
    smMockArgs = nullptr;
    smMockCurrTime = (struct timespec){0};
    smMockClockGettimeFails = false;
}

static void setup(void)
{
    assert(smStart() == CM_RESULT_OK);
    resetHooks();
}

static void teardown(void)
{
    resetHooks();
    assert(smStop() == CM_RESULT_OK);
}


// —————————————————————————————————————————————————————————————————————————————
// Variables
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
bool smMockClockGettimeFails;


// —————————————————————————————————————————————————————————————————————————————
// Base Tests
// —————————————————————————————————————————————————————————————————————————————

void Test_smStop_SucceedsPostStart(void)
{
    setup();
    teardown();
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————
// Pre-Start
// —————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smHasStarted_FailsPreStart(void)
{
    assert(!smIsRunning());
    tsInternalPass(__func__);
}

// Scene Functions

void Test_smCreateScene_FailsPreStart(void)
{
    assert(
        smCreateScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smSceneExists_FailsPreStart(void)
{
    assert(!smSceneExists(nullptr));
    tsInternalPass(__func__);
}

void Test_smSetScene_FailsPreStart(void)
{
    assert(smSetScene(nullptr, nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smGetCurrentSceneName_FailsPreStart(void)
{
    assert(!smGetCurrentSceneName());
    tsInternalPass(__func__);
}

void Test_smDeleteScene_FailsPreStart(void)
{
    assert(smDeleteScene(nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smGetSceneCount_FailsPreStart(void)
{
    assert(smGetSceneCount() == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

// Lifecycle Functions

void Test_smUpdate_FailsPreStart(void)
{
    assert(smUpdate(mockDt) == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smGetDt_FailsPreStart(void)
{
    assert(smGetDt() == (float) CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smDraw_FailsPreStart(void)
{
    assert(smDraw() == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

// Stop

void Test_smStop_FailsPreStart(void)
{
    assert(smStop() == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————
// Start
// —————————————————————————————————————————————————————————————————————————————

// Mem Alloc Fail

void Test_smStart_FailsWhenCallocFails(void)
{
    tsInternalDisable(CALLOC, 1);
    assert(smStart() == CM_RESULT_MEM_ALLOC_FAILED);
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————
// Post-Start
// —————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smStart_IsIdempotentPostStart(void)
{
    assert(smStart() == CM_RESULT_OK);
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++)
    {
        assert(smStart() == CM_RESULT_ALREADY_RUNNING);
    }
    assert(smStop() == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_smHasStarted_SucceedsPostStart(void)
{
    setup();
    assert(smIsRunning());
    teardown();
    tsInternalPass(__func__);
}

// Scene Functions

// -- smCreateScene

// -- -- Name Related

void Test_smCreateScene_AcceptsValidName(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    teardown();
    tsInternalPass(__func__);
}

void Test_smCreateScene_RejectsExistingName(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        SM_RESULT_SCENE_ALREADY_EXISTS);
    teardown();
    tsInternalPass(__func__);
}

void Test_smCreateScene_RejectsNullName(void)
{
    setup();
    assert(
        smCreateScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        CM_RESULT_NULL_ARG);
    teardown();
    tsInternalPass(__func__);
}

void Test_smCreateScene_RejectsEmptyName(void)
{
    setup();
    assert(
        smCreateScene("", nullptr, nullptr, nullptr, nullptr) ==
        CM_RESULT_EMPTY_ARG);
    teardown();
    tsInternalPass(__func__);
}

// -- -- Scene Functions Related

void Test_smCreateScene_AcceptsAllValidFunctionCombinations(void)
{
    typedef struct {
        smEnterFn enter;
        smUpdateFn update;
        smDrawFn draw;
        smExitFn exit;
    } CreateSceneCase;

    static const CreateSceneCase CASES[] = {
        {mockEnter, mockUpdate, mockDraw, mockExit},
        {nullptr, mockUpdate, mockDraw, mockExit},
        {mockEnter, nullptr, mockDraw, mockExit},
        {mockEnter, mockUpdate, nullptr, mockExit},
        {mockEnter, mockUpdate, mockDraw, nullptr},
        {nullptr, nullptr, mockDraw, mockExit},
        {nullptr, mockUpdate, nullptr, mockExit},
        {nullptr, mockUpdate, mockDraw, nullptr},
        {mockEnter, nullptr, nullptr, mockExit},
        {mockEnter, nullptr, mockDraw, nullptr},
        {mockEnter, mockUpdate, nullptr, nullptr},
        {nullptr, nullptr, nullptr, mockExit},
        {nullptr, nullptr, mockDraw, nullptr},
        {nullptr, mockUpdate, nullptr, nullptr},
        {mockEnter, nullptr, nullptr, nullptr},
    };

    for (size_t i = 0; i < sizeof(CASES) / sizeof(CASES[0]); i++)
    {
        setup();
        assert(
            smCreateScene(mock.name, CASES[i].enter, CASES[i].update,
                CASES[i].draw, CASES[i].exit) == CM_RESULT_OK);
        teardown();
    }

    tsInternalPass(__func__);
}

void Test_smCreateScene_RejectsValidNameAndAllNullFunctions(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, nullptr) ==
        SM_RESULT_NO_VALID_FUNCTIONS);
    teardown();
    tsInternalPass(__func__);
}

void Test_smCreateScene_FailsWhenSceneAllocFails(void)
{
    setup();
    tsInternalDisable(MALLOC, 1);
    assert(
        smCreateScene("malloc-scene-fail", mockEnter, nullptr, nullptr,
            nullptr) == CM_RESULT_MEM_ALLOC_FAILED);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-scene-fail"));
    teardown();
    tsInternalPass(__func__);
}

void Test_smCreateScene_FailsWhenNameAllocFails(void)
{
    setup();
    tsInternalDisable(MALLOC, 2);
    assert(
        smCreateScene("malloc-name-fail", mockEnter, nullptr, nullptr,
            nullptr) == CM_RESULT_MEM_ALLOC_FAILED);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-name-fail"));
    teardown();
    tsInternalPass(__func__);
}

void Test_smCreateScene_FailsWhenMapEntryAllocFails(void)
{
    setup();
    tsInternalDisable(MALLOC, 3);
    assert(
        smCreateScene("malloc-map-entry-fail", mockEnter, nullptr, nullptr,
            nullptr) == CM_RESULT_MEM_ALLOC_FAILED);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-map-entry-fail"));
    teardown();
    tsInternalPass(__func__);
}

// smSceneExists

void Test_smSceneExists_AcceptsCreatedName(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSceneExists(mock.name));
    teardown();
    tsInternalPass(__func__);
}

void Test_smSceneExists_RejectsNonCreatedName(void)
{
    setup();
    assert(!smSceneExists(mock.name));
    teardown();
    tsInternalPass(__func__);
}

// -- smSetScene

void Test_smSetScene_AcceptsValidSceneFromNull(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_RejectsNullName(void)
{
    setup();
    assert(smSetScene(nullptr, nullptr) == CM_RESULT_NULL_ARG);
    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_RejectsEmptyName(void)
{
    setup();
    assert(smSetScene("", nullptr) == CM_RESULT_EMPTY_ARG);
    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_RejectsNonCreatedName(void)
{
    setup();
    assert(smSetScene(mock.name, nullptr) == SM_RESULT_SCENE_NOT_FOUND);
    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_SucceedsChangingFromOneSceneToAnotherWithNoArgs(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smSetScene(mock2.name, nullptr) == CM_RESULT_OK);
    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_CallsNonNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smSetScene(mock2.name, nullptr) == CM_RESULT_OK);
    assert(smMockData->exitCount == 1);

    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_SkipsNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock2.name, nullptr, nullptr, nullptr, mockExit) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smSetScene(mock2.name, nullptr) == CM_RESULT_OK);
    assert(smMockData->exitCount == 0);

    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_CallsNonNullEnterOfTargetScene(void)
{
    setup();
    smTestEnter = onEnter;
    smMockData = &(MockData){0};

    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smSetScene(mock2.name, nullptr) == CM_RESULT_OK);
    assert(smMockData->enterCount == 1);

    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_SkipsNullEnterOfTargetScene(void)
{
    setup();
    smTestEnter = onEnter;
    smMockData = &(MockData){0};

    assert(
        smCreateScene(mock.name, nullptr, mockUpdate, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock2.name, nullptr, nullptr, nullptr, mockExit) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smSetScene(mock2.name, nullptr) == CM_RESULT_OK);
    assert(smMockData->enterCount == 0);

    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameScene(
    void)
{
    setup();
    smTestEnter = onEnter;
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, mockExit) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smMockData->enterCount == 2);
    assert(smMockData->exitCount == 1);

    teardown();
    tsInternalPass(__func__);
}

void Test_smSetScene_CallsNonNullEnterWithArgsOfTargetScene(void)
{
    setup();
    smTestEnterWithArgs = onEnterWithArgs;
    smMockData = &(MockData){0};
    smMockArgs = &(MockArgs){0};

    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smSetScene(mock2.name, smMockArgs) == CM_RESULT_OK);
    assert(smMockArgs->flag);

    teardown();
    tsInternalPass(__func__);
}

// -- smGetCurrentSceneName

void Test_smGetCurrentSceneName_FailsPreCreateScene(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    teardown();
    tsInternalPass(__func__);
}

void Test_smGetCurrentSceneName_ReturnsCurrentSceneName(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(strcmp(smGetCurrentSceneName(), mock.name) == 0);
    teardown();
    tsInternalPass(__func__);
}

// -- smDeleteScene

void Test_smDeleteScene_FailsPreCreateScene(void)
{
    setup();
    assert(smDeleteScene(mock.name) == SM_RESULT_SCENE_NOT_FOUND);
    teardown();
    tsInternalPass(__func__);
}

void Test_smDeleteScene_FailsToDeleteCurrentScene(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == SM_RESULT_CANNOT_DELETE_CURRENT_SCENE);
    teardown();
    tsInternalPass(__func__);
}

void Test_smDeleteScene_AcceptsNonCurrentScene(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == CM_RESULT_OK);
    teardown();
    tsInternalPass(__func__);
}

void Test_smDeleteScene_RejectsEmptyName(void)
{
    setup();
    assert(smDeleteScene("") == CM_RESULT_EMPTY_ARG);
    teardown();
    tsInternalPass(__func__);
}

void Test_smDeleteScene_FailsWhenDeletingSameSceneTwice(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == SM_RESULT_SCENE_NOT_FOUND);
    teardown();
    tsInternalPass(__func__);
}

// -- smGetSceneCount

void Test_smGetSceneCount_ReturnsZeroPostStart(void)
{
    setup();
    assert(smGetSceneCount() == 0);
    teardown();
    tsInternalPass(__func__);
}

void Test_smGetSceneCount_ReturnsCorrectSceneCountPostCreateScene(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smGetSceneCount() == 1);
    teardown();
    tsInternalPass(__func__);
}

void Test_smGetSceneCount_ReturnsCorrectSceneCountPostDeleteScene(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == CM_RESULT_OK);
    assert(smGetSceneCount() == 0);
    teardown();
    tsInternalPass(__func__);
}

// Lifecycle Functions

// -- smUpdate

void Test_smUpdate_FailsWhenNullCurrentScene(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    assert(smUpdate(mockDt) == SM_RESULT_NO_CURRENT_SCENE);
    teardown();
    tsInternalPass(__func__);
}

void Test_smUpdate_CallsNonNullUpdateOfCurrentScene(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, mockUpdate, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smUpdate(mockDt) == CM_RESULT_OK);
    teardown();
    tsInternalPass(__func__);
}

void Test_smUpdate_FailsWhenNullUpdate(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smUpdate(mockDt) == SM_RESULT_NO_UPDATE_FUNCTION);
    teardown();
    tsInternalPass(__func__);
}

// -- smGetDt

void Test_smGetDt_UsesDefaultDtOnFirstCall(void)
{
    setup();
    const float target = 1.0f / DEFAULT_FPS;
    assert(fabsf(smGetDt() - target) < DT_TOLERANCE);
    teardown();
    tsInternalPass(__func__);
}

void Test_smGetDt_UpdatesDtOnConsecutiveCalls(void)
{
    setup();

    // Arbitrary time for first smGetDt call
    smMockCurrTime.tv_nsec = EXPECTED_DT_NS;
    smMockCurrTime.tv_sec = 0;

    smGetDt(); // First call uses default dt

    for (int i = 0; i < FRAME_TIME_ITERATIONS; i++)
    {
        smMockCurrTime.tv_nsec += EXPECTED_DT_NS;
        if (smMockCurrTime.tv_nsec >= NS_PER_S)
        {
            smMockCurrTime.tv_sec += smMockCurrTime.tv_nsec / NS_PER_S;
            smMockCurrTime.tv_nsec %= NS_PER_S;
        }
        assert(fabsf(smGetDt() - EXPECTED_DT_S) < DT_TOLERANCE);
    }

    teardown();
    tsInternalPass(__func__);
}

void Test_smGetDt_FailsWhenClockGettimeFails(void)
{
    setup();
    smMockClockGettimeFails = true;
    assert(smGetDt() == (float) CM_RESULT_CLOCK_GETTIME_FAILED);
    teardown();
    tsInternalPass(__func__);
}


// -- smDraw

void Test_smDraw_FailsWhenNullCurrentScene(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    assert(smDraw() == SM_RESULT_NO_CURRENT_SCENE);
    teardown();
    tsInternalPass(__func__);
}

void Test_smDraw_CallsValidDrawFunction(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, nullptr, mockDraw, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smDraw() == CM_RESULT_OK);
    teardown();
    tsInternalPass(__func__);
}

void Test_smDraw_FailsWhenNullDraw(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smDraw() == SM_RESULT_NO_DRAW_FUNCTION);
    teardown();
    tsInternalPass(__func__);
}

// Stop Related

void Test_smStop_CallsNonNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smStop() == CM_RESULT_OK);
    assert(smMockData->exitCount == 1);

    resetHooks();
    tsInternalPass(__func__);
}

void Test_smStop_SkipsNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smStop() == CM_RESULT_OK);
    assert(smMockData->exitCount == 0);

    resetHooks();
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————
// Post-Stop
// —————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smHasStarted_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smIsRunning());
    tsInternalPass(__func__);
}

// Scene Functions

void Test_smCreateScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(
        smCreateScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smSceneExists_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smSceneExists(nullptr));
    tsInternalPass(__func__);
}

void Test_smSetScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smSetScene(nullptr, nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smGetCurrentSceneName_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smGetCurrentSceneName());
    tsInternalPass(__func__);
}

void Test_smDeleteScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smDeleteScene(nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smGetSceneCount_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smGetSceneCount() == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

// Lifecycle Functions

void Test_smUpdate_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smUpdate(mockDt) == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

void Test_smDraw_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smDraw() == CM_RESULT_NOT_RUNNING);
    tsInternalPass(__func__);
}

// Stop Related

void Test_smStop_IsIdempotentPostStop(void)
{
    assert(smStart() == CM_RESULT_OK);
    assert(smStop() == CM_RESULT_OK);
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++)
    {
        assert(smStop() == CM_RESULT_NOT_RUNNING);
    }
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————
// Stress tests
// —————————————————————————————————————————————————————————————————————————————

// Scene Functions

void TestStress_smCreateScene_CreatingMultipleScenesCausesNoSkips(void)
{
    setup();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        assert(
            smCreateScene(buf, mockEnter, nullptr, nullptr, nullptr) ==
            CM_RESULT_OK);
    }
    assert(smGetSceneCount() == STRESS_ITERATIONS);
    teardown();
    tsInternalPass(__func__);
}

void TestStress_smSetScene_SettingScenesOftenCausesNoSkips(void)
{
    setup();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        assert(
            smCreateScene(buf, mockEnter, nullptr, nullptr, nullptr) ==
            CM_RESULT_OK);
    }
    int counter = 0;
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        assert(smSetScene(buf, nullptr) == CM_RESULT_OK);

        const char *currSceneName = smGetCurrentSceneName();
        assert(currSceneName);

        char *end;
        long sceneNum = strtol(currSceneName, &end, 10);
        assert(end != currSceneName);
        assert(*end == '\0');
        assert(sceneNum == counter);

        counter++;
    }
    assert(counter == STRESS_ITERATIONS);
    teardown();
    tsInternalPass(__func__);
}

// Stop Related

void TestStress_smStop_FreeingMultipleScenesCausesNoSkips(void)
{
    setup();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        assert(
            smCreateScene(buf, mockEnter, nullptr, nullptr, nullptr) ==
            CM_RESULT_OK);
    }
    teardown();
    tsInternalPass(__func__);
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
    puts("• Scene Functions");
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
    puts("• Scene Functions");
    puts(" • smCreateScene");
    puts("  • Name Related");
    Test_smCreateScene_AcceptsValidName();
    Test_smCreateScene_RejectsExistingName();
    Test_smCreateScene_RejectsNullName();
    Test_smCreateScene_RejectsEmptyName();

    puts("  • Scene Functions Related");
    Test_smCreateScene_AcceptsAllValidFunctionCombinations();
    Test_smCreateScene_RejectsValidNameAndAllNullFunctions();
    Test_smCreateScene_FailsWhenSceneAllocFails();
    Test_smCreateScene_FailsWhenNameAllocFails();
    Test_smCreateScene_FailsWhenMapEntryAllocFails();
    puts(" • smSceneExists");
    Test_smSceneExists_AcceptsCreatedName();
    Test_smSceneExists_RejectsNonCreatedName();
    puts(" • smSetScene");
    Test_smSetScene_AcceptsValidSceneFromNull();
    Test_smSetScene_RejectsNullName();
    Test_smSetScene_RejectsEmptyName();
    Test_smSetScene_RejectsNonCreatedName();
    Test_smSetScene_SucceedsChangingFromOneSceneToAnotherWithNoArgs();
    Test_smSetScene_CallsNonNullExitOfCurrentScene();
    Test_smSetScene_SkipsNullExitOfCurrentScene();
    Test_smSetScene_CallsNonNullEnterOfTargetScene();
    Test_smSetScene_SkipsNullEnterOfTargetScene();
    Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameScene();
    Test_smSetScene_CallsNonNullEnterWithArgsOfTargetScene();
    puts(" • smGetCurrentSceneName");
    Test_smGetCurrentSceneName_FailsPreCreateScene();
    Test_smGetCurrentSceneName_ReturnsCurrentSceneName();
    puts(" • smDeleteScene");
    Test_smDeleteScene_FailsPreCreateScene();
    Test_smDeleteScene_FailsToDeleteCurrentScene();
    Test_smDeleteScene_AcceptsNonCurrentScene();
    Test_smDeleteScene_RejectsEmptyName();
    Test_smDeleteScene_FailsWhenDeletingSameSceneTwice();
    puts(" • smGetSceneCount");
    Test_smGetSceneCount_ReturnsZeroPostStart();
    Test_smGetSceneCount_ReturnsCorrectSceneCountPostCreateScene();
    Test_smGetSceneCount_ReturnsCorrectSceneCountPostDeleteScene();
    puts("• Lifecycle Functions");
    puts(" • smUpdate");
    Test_smUpdate_FailsWhenNullCurrentScene();
    Test_smUpdate_CallsNonNullUpdateOfCurrentScene();
    Test_smUpdate_FailsWhenNullUpdate();
    puts(" • smGetDt");
    Test_smGetDt_UsesDefaultDtOnFirstCall();
    Test_smGetDt_UpdatesDtOnConsecutiveCalls();
    Test_smGetDt_FailsWhenClockGettimeFails();
    puts(" • smDraw");
    Test_smDraw_FailsWhenNullCurrentScene();
    Test_smDraw_CallsValidDrawFunction();
    Test_smDraw_FailsWhenNullDraw();

    puts("\nSTOP TESTING");
    Test_smStop_CallsNonNullExitOfCurrentScene();
    Test_smStop_SkipsNullExitOfCurrentScene();

    puts("\nPOST-STOP TESTING");
    puts("• Start Related");
    Test_smHasStarted_FailsPostStop();
    puts("• Scene Functions");
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
    TestStress_smCreateScene_CreatingMultipleScenesCausesNoSkips();
    TestStress_smSetScene_SettingScenesOftenCausesNoSkips();
    TestStress_smStop_FreeingMultipleScenesCausesNoSkips();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
    return 0;
}
