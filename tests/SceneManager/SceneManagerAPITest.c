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


// —————————————————————————————————————————————————————————————————————————————
// Base Tests
// —————————————————————————————————————————————————————————————————————————————

void Test_smStop_SucceedsPostStart(void)
{
    setup();
    teardown();
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
    assert(
        smCreateScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smCreateScene_FailsPreStart");
}

void Test_smSceneExists_FailsPreStart(void)
{
    assert(!smSceneExists(nullptr));
    tsInternalPass("Test_smSceneExists_FailsPreStart");
}

void Test_smSetScene_FailsPreStart(void)
{
    assert(smSetScene(nullptr, nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smSetScene_FailsPreStart");
}

void Test_smGetCurrentSceneName_FailsPreStart(void)
{
    assert(!smGetCurrentSceneName());
    tsInternalPass("Test_smGetCurrentSceneName_FailsPreStart");
}

void Test_smDeleteScene_FailsPreStart(void)
{
    assert(smDeleteScene(nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smDeleteScene_FailsPreStart");
}

void Test_smGetSceneCount_FailsPreStart(void)
{
    assert(smGetSceneCount() == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smGetSceneCount_FailsPreStart");
}

// Lifecycle Functions

void Test_smUpdate_FailsPreStart(void)
{
    assert(smUpdate(mockDt) == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smUpdate_FailsPreStart");
}

void Test_smGetDt_FailsPreStart(void)
{
    assert(smGetDt() == (float) CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smGetDt_FailsPreStart");
}

void Test_smDraw_FailsPreStart(void)
{
    assert(smDraw() == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smDraw_FailsPreStart");
}

// Stop

void Test_smStop_FailsPreStart(void)
{
    assert(smStop() == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smStop_FailsPreStart");
}


// —————————————————————————————————————————————————————————————————————————————
// Start
// —————————————————————————————————————————————————————————————————————————————

// Mem Alloc Fail

void Test_smStart_FailsWhenCallocFails(void)
{
    tsInternalDisable(CALLOC, 1);
    assert(smStart() == CM_RESULT_MEM_ALLOC_FAILED);
    tsInternalPass("Test_smStart_FailsWhenCallocFails");
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
    tsInternalPass("Test_smStart_IsIdempotentPostStart");
}

void Test_smHasStarted_SucceedsPostStart(void)
{
    setup();
    assert(smIsRunning());
    teardown();
    tsInternalPass("Test_smHasStarted_SucceedsPostStart");
}

// State Functions

// -- smCreateScene

// -- -- Name Related

void Test_smCreateScene_AcceptsValidName(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smCreateScene_AcceptsValidName");
}

void Test_smCreateScene_RejectsExistingName(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr) ==
        SM_RESULT_SCENE_ALREADY_EXISTS);
    teardown();
    tsInternalPass("Test_smCreateScene_RejectsExistingName");
}

void Test_smCreateScene_RejectsNullName(void)
{
    setup();
    assert(
        smCreateScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        CM_RESULT_NULL_ARG);
    teardown();
    tsInternalPass("Test_smCreateScene_RejectsNullName");
}

void Test_smCreateScene_RejectsEmptyName(void)
{
    setup();
    assert(
        smCreateScene("", nullptr, nullptr, nullptr, nullptr) ==
        CM_RESULT_EMPTY_ARG);
    teardown();
    tsInternalPass("Test_smCreateScene_RejectsEmptyName");
}

// -- -- State Functions Related

void Test_smCreateScene_AcceptsValidNameAndNoNullFunctions(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, mock.draw, mock.exit
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNoNullFunctions");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnter(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, mock.draw, mock.exit) ==
        CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullEnter");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdate(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, mock.draw, mock.exit) ==
        CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullUpdate");
}

void Test_smCreateScene_AcceptsValidNameAndNullDraw(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, nullptr, mock.exit) ==
        CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullExit(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, mock.draw, nullptr) ==
        CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdate(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr,nullptr, mock.draw, mock.exit) ==
        CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdate");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDraw(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, nullptr, mock.exit
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullExit(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, mock.draw, nullptr
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDraw(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, mock.exit
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullExit(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, mock.draw, nullptr
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullDrawAndNullExit(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, mock.update, nullptr, nullptr
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smCreateScene_AcceptsValidNameAndNullDrawAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw(
    void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, mock.exit
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit(
    void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, nullptr, mock.draw, nullptr
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit(
    void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, nullptr, nullptr
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit");
}

void Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit(
    void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr
        ) == CM_RESULT_OK);
    teardown();
    tsInternalPass(
        "Test_smCreateScene_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit");
}

void Test_smCreateScene_RejectsValidNameAndAllNullFunctions(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, nullptr, nullptr, nullptr) ==
        SM_RESULT_NO_VALID_FUNCTIONS);
    teardown();
    tsInternalPass("Test_smCreateScene_RejectsValidNameAndAllNullFunctions");
}

void Test_smCreateScene_FailsWhenSceneAllocFails(void)
{
    setup();
    tsInternalDisable(MALLOC, 1);
    assert(
        smCreateScene("malloc-scene-fail", mock.enter, nullptr, nullptr,
            nullptr) == CM_RESULT_MEM_ALLOC_FAILED);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-scene-fail"));
    teardown();
    tsInternalPass("Test_smCreateScene_FailsWhenSceneAllocFails");
}

void Test_smCreateScene_FailsWhenNameAllocFails(void)
{
    setup();
    tsInternalDisable(MALLOC, 2);
    assert(
        smCreateScene("malloc-name-fail", mock.enter, nullptr, nullptr,
            nullptr) == CM_RESULT_MEM_ALLOC_FAILED);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-name-fail"));
    teardown();
    tsInternalPass("Test_smCreateScene_FailsWhenNameAllocFails");
}

void Test_smCreateScene_FailsWhenMapEntryAllocFails(void)
{
    setup();
    tsInternalDisable(MALLOC, 3);
    assert(
        smCreateScene("malloc-map-entry-fail", mock.enter, nullptr, nullptr,
            nullptr) == CM_RESULT_MEM_ALLOC_FAILED);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-map-entry-fail"));
    teardown();
    tsInternalPass("Test_smCreateScene_FailsWhenMapEntryAllocFails");
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
    tsInternalPass("Test_smSceneExists_AcceptsCreatedName");
}

void Test_smSceneExists_RejectsNonCreatedName(void)
{
    setup();
    assert(!smSceneExists(mock.name));
    teardown();
    tsInternalPass("Test_smSceneExists_RejectsNonCreatedName");
}

// -- smSetScene

void Test_smSetScene_AcceptsValidStateFromNull(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smSetScene_AcceptsValidStateFromNull");
}

void Test_smSetScene_RejectsNullName(void)
{
    setup();
    assert(smSetScene(nullptr, nullptr) == CM_RESULT_NULL_ARG);
    teardown();
    tsInternalPass("Test_smSetScene_RejectsNullName");
}

void Test_smSetScene_RejectsNonCreatedName(void)
{
    setup();
    assert(smSetScene(mock.name, nullptr) == SM_RESULT_SCENE_NOT_FOUND);
    teardown();
    tsInternalPass("Test_smSetScene_RejectsNonCreatedName");
}

void Test_smSetScene_SucceedsChangingFromOneStateToAnotherWithNoArgs(void)
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
    tsInternalPass(
        "Test_smSetScene_SucceedsChangingFromOneStateToAnotherWithNoArgs");
}

void Test_smSetScene_CallsNonNullExitOfCurrentState(void)
{
    MockData *mockData = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    if (!mockData)
    {
        goto cleanup;
    }
    resetHooks();
    smTestExit = onExit;
    smMockData = mockData;

    if (smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock2.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (mockData->exitCount != 1)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smSetScene_CallsExitOfCurrentState");
}

void Test_smSetScene_SkipsNullExitOfCurrentState(void)
{
    MockData *mockData = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    if (!mockData)
    {
        goto cleanup;
    }
    resetHooks();
    smTestExit = onExit;
    smMockData = mockData;

    if (smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smCreateScene(mock2.name, nullptr, nullptr, nullptr, mockExit) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock2.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (mockData->exitCount != 0)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smSetScene_SkipsNullExitOfCurrentState");
}

void Test_smSetScene_CallsNonNullEnterOfTargetState(void)
{
    MockData *mockData = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    if (!mockData)
    {
        goto cleanup;
    }
    resetHooks();
    smTestEnter = onEnter;
    smMockData = mockData;

    if (smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock2.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (mockData->enterCount != 1)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smSetScene_CallsNonNullEnterOfTargetState");
}

void Test_smSetScene_SkipsNullEnterOfTargetState(void)
{
    MockData *mockData = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    if (!mockData)
    {
        goto cleanup;
    }
    resetHooks();
    smTestEnter = onEnter;
    smMockData = mockData;

    if (smCreateScene(mock.name, nullptr, mockUpdate, nullptr, nullptr) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smCreateScene(mock2.name, nullptr, nullptr, nullptr, mockExit) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock2.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (mockData->enterCount != 0)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smSetScene_SkipsNullEnterOfTargetState");
}

void Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameState(
    void)
{
    MockData *mockData = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    if (!mockData)
    {
        goto cleanup;
    }
    resetHooks();
    smTestEnter = onEnter;
    smTestExit = onExit;
    smMockData = mockData;

    if (smCreateScene(mock.name, mockEnter, nullptr, nullptr, mockExit) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (mockData->enterCount != 2 || mockData->exitCount != 1)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameState");
}

void Test_smSetScene_CallsNonNullEnterWithArgsOfTargetState(void)
{
    MockData *mockData = nullptr;
    MockArgs *mockArgs = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    mockArgs = calloc(1, sizeof(MockArgs));
    if (!mockData || !mockArgs)
    {
        goto cleanup;
    }
    resetHooks();
    smTestEnterWithArgs = onEnterWithArgs;
    smMockData = mockData;
    smMockArgs = mockArgs;

    if (smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smCreateScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock2.name, smMockArgs) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (!mockArgs->flag)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    free(mockArgs);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smSetScene_CallsNonNullEnterWithArgsOfTargetState");
}

// -- smGetCurrentSceneName

void Test_smGetCurrentSceneName_FailsPreCreateScene(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    teardown();
    tsInternalPass("Test_smGetCurrentSceneName_FailsPreCreateScene");
}

void Test_smGetCurrentSceneName_ReturnsCurrentStateName(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(strcmp(smGetCurrentSceneName(), mock.name) == 0);
    teardown();
    tsInternalPass("Test_smGetCurrentSceneName_ReturnsCurrentStateName");
}

// -- smDeleteScene

void Test_smDeleteScene_FailsPreCreateScene(void)
{
    setup();
    assert(smDeleteScene(mock.name) == SM_RESULT_SCENE_NOT_FOUND);
    teardown();
    tsInternalPass("Test_smDeleteScene_FailsPreCreateScene");
}

void Test_smDeleteScene_FailsToDeleteCurrentState(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == SM_RESULT_CANNOT_DELETE_CURRENT_SCENE);
    teardown();
    tsInternalPass("Test_smDeleteScene_FailsToDeleteCurrentState");
}

void Test_smDeleteScene_AcceptsNonCurrentState(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smDeleteScene_AcceptsNonCurrentState");
}

// -- smGetSceneCount

void Test_smGetSceneCount_ReturnsZeroPostStart(void)
{
    setup();
    assert(smGetSceneCount() == 0);
    teardown();
    tsInternalPass("Test_smGetSceneCount_ReturnsZeroPostStart");
}

void Test_smGetSceneCount_ReturnsCorrectStateCountPostCreateScene(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smGetSceneCount() == 1);
    teardown();
    tsInternalPass(
        "Test_smGetSceneCount_ReturnsCorrectStateCountPostCreateScene");
}

void Test_smGetSceneCount_ReturnsCorrectStateCountPostDeleteScene(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smDeleteScene(mock.name) == CM_RESULT_OK);
    assert(smGetSceneCount() == 0);
    teardown();
    tsInternalPass(
        "Test_smGetSceneCount_ReturnsCorrectStateCountPostDeleteScene");
}

// Lifecycle Functions

// -- smUpdate

void Test_smUpdate_FailsWhenNullCurrentState(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    assert(smUpdate(mockDt) == SM_RESULT_NO_CURRENT_SCENE);
    teardown();
    tsInternalPass("Test_smUpdate_FailsWhenNullCurrentState");
}

void Test_smUpdate_CallsNonNullUpdateOfCurrentState(void)
{
    setup();
    assert(
        smCreateScene(mock.name, nullptr, mock.update, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smUpdate(mockDt) == CM_RESULT_OK);
    teardown();
    tsInternalPass("Test_smUpdate_CallsNonNullUpdateOfCurrentState");
}

void Test_smUpdate_FailsWhenNullUpdate(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smUpdate(mockDt) == SM_RESULT_NO_UPDATE_FUNCTION);
    teardown();
    tsInternalPass("Test_smUpdate_FailsWhenNullUpdate");
}

// -- smGetDt

void Test_smGetDt_UsesDefaultDtOnFirstCall(void)
{
    setup();
    const float target = 1.0f / DEFAULT_FPS;
    assert(fabsf(smGetDt() - target) < DT_TOLERANCE);
    teardown();
    tsInternalPass("Test_smGetDt_UsesDefaultDtOnFirstCall");
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
    tsInternalPass("Test_smGetDt_UpdatesDtOnConsecutiveCalls");
}


// -- smDraw

void Test_smDraw_FailsWhenNullCurrentState(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    assert(smDraw() == SM_RESULT_NO_CURRENT_SCENE);
    teardown();
    tsInternalPass("Test_smDraw_FailsWhenNullCurrentState");
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
    tsInternalPass("Test_smDraw_CallsValidDrawFunction");
}

void Test_smDraw_FailsWhenNullDraw(void)
{
    setup();
    assert(
        smCreateScene(mock.name, mock.enter, nullptr, nullptr, nullptr) ==
        CM_RESULT_OK);
    assert(smSetScene(mock.name, nullptr) == CM_RESULT_OK);
    assert(smDraw() == SM_RESULT_NO_DRAW_FUNCTION);
    teardown();
    tsInternalPass("Test_smDraw_FailsWhenNullDraw");
}

// Stop Related

void Test_smStop_CallsNonNullExitOfCurrentState(void)
{
    MockData *mockData = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    if (!mockData)
    {
        goto cleanup;
    }
    resetHooks();
    smTestExit = onExit;
    smMockData = mockData;

    if (smCreateScene(mock.name, nullptr, nullptr, nullptr, mockExit) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smStop() != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (mockData->exitCount != 1)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smStop_CallsExitOfCurrentState");
}

void Test_smStop_SkipsNullExitOfCurrentState(void)
{
    MockData *mockData = nullptr;
    int passed = false;

    setup();
    mockData = calloc(1, sizeof(MockData));
    if (!mockData)
    {
        goto cleanup;
    }
    resetHooks();
    smTestExit = onExit;
    smMockData = mockData;

    if (smCreateScene(mock.name, mockEnter, nullptr, nullptr, nullptr) !=
        CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smSetScene(mock.name, nullptr) != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (smStop() != CM_RESULT_OK)
    {
        goto cleanup;
    }
    if (mockData->exitCount != 0)
    {
        goto cleanup;
    }

    passed = true;

cleanup:
    free(mockData);
    resetHooks();
    if (smIsRunning())
    {
        assert(smStop() == CM_RESULT_OK);
    }
    assert(passed);
    tsInternalPass(
        "Test_smStop_SkipsNullExitOfCurrentState");
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
    tsInternalPass("Test_smHasStarted_FailsPostStop");
}

// State Functions

void Test_smCreateScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(
        smCreateScene(nullptr, nullptr, nullptr,nullptr,nullptr) ==
        CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smCreateScene_FailsPostStop");
}

void Test_smSceneExists_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smSceneExists(nullptr));
    tsInternalPass("Test_smSceneExists_FailsPostStop");
}

void Test_smSetScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smSetScene(nullptr, nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smSetScene_FailsPostStop");
}

void Test_smGetCurrentSceneName_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smGetCurrentSceneName());
    tsInternalPass("Test_smGetCurrentSceneName_FailsPostStop");
}

void Test_smDeleteScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smDeleteScene(nullptr) == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smDeleteScene_FailsPostStop");
}

void Test_smGetSceneCount_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smGetSceneCount() == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smGetSceneCount_FailsPostStop");
}

// Lifecycle Functions

void Test_smUpdate_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smUpdate(mockDt) == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smUpdate_FailsPostStop");
}

void Test_smDraw_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smDraw() == CM_RESULT_NOT_RUNNING);
    tsInternalPass("Test_smDraw_FailsPostStop");
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
    tsInternalPass("Test_smStop_IsIdempotentPostStop");
}


// —————————————————————————————————————————————————————————————————————————————
// Stress tests
// —————————————————————————————————————————————————————————————————————————————

// State Functions

void TestStress_smCreateScene_CreatingMultipleStatesCausesNoSkips(void)
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
    tsInternalPass(
        "TestStress_smCreateScene_CreatingMultipleStatesCausesNoSkips");
}

void TestStress_smSetScene_SettingStatesOftenCausesNoSkips(void)
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
    tsInternalPass("TestStress_smSetScene_SettingStatesOftenCausesNoSkips");
}

// Stop Related

void TestStress_smStop_FreeingMultipleStatesCausesNoSkips(void)
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
    Test_smCreateScene_FailsWhenSceneAllocFails();
    Test_smCreateScene_FailsWhenNameAllocFails();
    Test_smCreateScene_FailsWhenMapEntryAllocFails();
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
    Test_smDeleteScene_FailsToDeleteCurrentState();
    Test_smDeleteScene_AcceptsNonCurrentState();
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
