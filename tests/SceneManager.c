// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "SceneManager.h"
#include "SceneManagerInternal.h"
#include "SceneManagerTestHooks.h"

#include "internal/Common/Common.h"
#include "internal/Test/Test.h"

// Defines —————————————————————————————————————————————————————————————————————————————————————————

#ifdef NDEBUG
#error "TestAPISceneManager must be compiled without NDEBUG (asserts required)."
#endif

// Variables ———————————————————————————————————————————————————————————————————————————————————————

// -- Constants

static constexpr long NS_PER_S = 1000000000L;
static constexpr float DT_TOLERANCE = 1e-6f;
static constexpr long EXPECTED_DT_NS = 16667000L;
static constexpr float EXPECTED_DT_S = 0.016667f;

static constexpr int FRAME_TIME_ITERATIONS = 300;
static constexpr int IDEMPOTENT_ITERATIONS = 3;
static constexpr int STRESS_ITERATIONS = 1000;

// Fixtures ————————————————————————————————————————————————————————————————————————————————————————

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

// Routine

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
    assert(smStart() == RES_OK);
    resetHooks();
}

static void teardown(void)
{
    resetHooks();
    assert(smStop() == RES_OK);
}


// Variables ———————————————————————————————————————————————————————————————————————————————————————

// Extern

smTestEnterFn smTestEnter;
smTestEnterWithArgsFn smTestEnterWithArgs;
smTestExitFn smTestExit;
MockData *smMockData;
MockArgs *smMockArgs;
struct timespec smMockCurrTime;
bool smMockClockGettimeFails;

// Static

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

// Base Tests ——————————————————————————————————————————————————————————————————————————————————————

void Test_smStop_SucceedsPostStart(void)
{
    setup();
    teardown();
    tsPass(__func__);
}

// Pre-Start ———————————————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smHasStarted_FailsPreStart(void)
{
    assert(!smIsRunning());
    tsPass(__func__);
}

// Scene Functions

void Test_smAddScene_FailsPreStart(void)
{
    assert(
        smAddScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smSceneExists_FailsPreStart(void)
{
    assert(!smSceneExists(nullptr));
    tsPass(__func__);
}

void Test_smSetScene_FailsPreStart(void)
{
    assert(smSetScene(nullptr, nullptr) == RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smGetCurrentSceneName_FailsPreStart(void)
{
    assert(!smGetCurrentSceneName());
    tsPass(__func__);
}

void Test_smRemoveScene_FailsPreStart(void)
{
    assert(smRemoveScene(nullptr) == RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smGetSceneCount_FailsPreStart(void)
{
    assert(smGetSceneCount() == RES_NOT_RUNNING);
    tsPass(__func__);
}

// Lifecycle Functions

void Test_smUpdate_FailsPreStart(void)
{
    assert(smUpdate(TS_MOCK_DT) == RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smGetDt_FailsPreStart(void)
{
    assert(smGetDt() == (float) RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smDraw_FailsPreStart(void)
{
    assert(smDraw() == RES_NOT_RUNNING);
    tsPass(__func__);
}

// Stop

void Test_smStop_FailsPreStart(void)
{
    assert(smStop() == RES_NOT_RUNNING);
    tsPass(__func__);
}

// Start ———————————————————————————————————————————————————————————————————————————————————————————

// Mem Alloc Fail

void Test_smStart_FailsWhenCallocFails(void)
{
    tsDisable(CALLOC, 1);
    assert(smStart() == RES_MEM_ALLOC_FAIL);
    tsPass(__func__);
}

// Post-Start ——————————————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smStart_IsIdempotentPostStart(void)
{
    assert(smStart() == RES_OK);
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++)
    {
        assert(smStart() == RES_ALREADY_RUNNING);
    }
    assert(smStop() == RES_OK);
    tsPass(__func__);
}

void Test_smHasStarted_SucceedsPostStart(void)
{
    setup();
    assert(smIsRunning());
    teardown();
    tsPass(__func__);
}

// Scene Functions

// -- smAddScene

// -- -- Name Related

void Test_smAddScene_AcceptsValidName(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_smAddScene_RejectsExistingName(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_SCENE_ALREADY_EXISTS);
    teardown();
    tsPass(__func__);
}

void Test_smAddScene_RejectsNullName(void)
{
    setup();
    assert(
        smAddScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        RES_NULL_ARG);
    teardown();
    tsPass(__func__);
}

void Test_smAddScene_RejectsEmptyName(void)
{
    setup();
    assert(
        smAddScene("", nullptr, nullptr, nullptr, nullptr) ==
        RES_EMPTY_ARG);
    teardown();
    tsPass(__func__);
}

// -- -- Scene Functions Related

void Test_smAddScene_AcceptsAllValidFunctionCombinations(void)
{
    typedef struct
    {
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
            smAddScene(mock.name, CASES[i].enter, CASES[i].update,
                CASES[i].draw, CASES[i].exit) == RES_OK);
        teardown();
    }

    tsPass(__func__);
}

void Test_smAddScene_RejectsValidNameAndAllNullFunctions(void)
{
    setup();
    assert(
        smAddScene(mock.name, nullptr, nullptr, nullptr, nullptr) ==
        RES_NO_VALID_FUNCS);
    teardown();
    tsPass(__func__);
}

void Test_smAddScene_FailsWhenSceneAllocFails(void)
{
    setup();
    tsDisable(MALLOC, 1);
    assert(
        smAddScene("malloc-scene-fail", mockEnter, nullptr, nullptr,
            nullptr) == RES_MEM_ALLOC_FAIL);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-scene-fail"));
    teardown();
    tsPass(__func__);
}

void Test_smAddScene_FailsWhenNameAllocFails(void)
{
    setup();
    tsDisable(MALLOC, 2);
    assert(
        smAddScene("malloc-name-fail", mockEnter, nullptr, nullptr,
            nullptr) == RES_MEM_ALLOC_FAIL);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-name-fail"));
    teardown();
    tsPass(__func__);
}

void Test_smAddScene_FailsWhenMapEntryAllocFails(void)
{
    setup();
    tsDisable(MALLOC, 3);
    assert(
        smAddScene("malloc-map-entry-fail", mockEnter, nullptr, nullptr,
            nullptr) == RES_MEM_ALLOC_FAIL);
    assert(smGetSceneCount() == 0);
    assert(!smSceneExists("malloc-map-entry-fail"));
    teardown();
    tsPass(__func__);
}

// smSceneExists

void Test_smSceneExists_AcceptsCreatedName(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSceneExists(mock.name));
    teardown();
    tsPass(__func__);
}

void Test_smSceneExists_RejectsNonCreatedName(void)
{
    setup();
    assert(!smSceneExists(mock.name));
    teardown();
    tsPass(__func__);
}

// -- smSetScene

void Test_smSetScene_AcceptsValidSceneFromNull(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_smSetScene_RejectsNullName(void)
{
    setup();
    assert(smSetScene(nullptr, nullptr) == RES_NULL_ARG);
    teardown();
    tsPass(__func__);
}

void Test_smSetScene_RejectsEmptyName(void)
{
    setup();
    assert(smSetScene("", nullptr) == RES_EMPTY_ARG);
    teardown();
    tsPass(__func__);
}

void Test_smSetScene_RejectsNonCreatedName(void)
{
    setup();
    assert(smSetScene(mock.name, nullptr) == RES_SCENE_NOT_FOUND);
    teardown();
    tsPass(__func__);
}

void Test_smSetScene_SucceedsChangingFromOneSceneToAnotherWithNoArgs(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(
        smAddScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smSetScene(mock2.name, nullptr) == RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_smSetScene_CallsNonNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smAddScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        RES_OK);
    assert(
        smAddScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smSetScene(mock2.name, nullptr) == RES_OK);
    assert(smMockData->exitCount == 1);

    teardown();
    tsPass(__func__);
}

void Test_smSetScene_SkipsNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(
        smAddScene(mock2.name, nullptr, nullptr, nullptr, mockExit) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smSetScene(mock2.name, nullptr) == RES_OK);
    assert(smMockData->exitCount == 0);

    teardown();
    tsPass(__func__);
}

void Test_smSetScene_CallsNonNullEnterOfTargetScene(void)
{
    setup();
    smTestEnter = onEnter;
    smMockData = &(MockData){0};

    assert(
        smAddScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        RES_OK);
    assert(
        smAddScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smSetScene(mock2.name, nullptr) == RES_OK);
    assert(smMockData->enterCount == 1);

    teardown();
    tsPass(__func__);
}

void Test_smSetScene_SkipsNullEnterOfTargetScene(void)
{
    setup();
    smTestEnter = onEnter;
    smMockData = &(MockData){0};

    assert(
        smAddScene(mock.name, nullptr, mockUpdate, nullptr, nullptr) ==
        RES_OK);
    assert(
        smAddScene(mock2.name, nullptr, nullptr, nullptr, mockExit) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smSetScene(mock2.name, nullptr) == RES_OK);
    assert(smMockData->enterCount == 0);

    teardown();
    tsPass(__func__);
}

void Test_smSetScene_CallsNonNullExitAndNonNullEnterWhenTargetingSameScene(
    void)
{
    setup();
    smTestEnter = onEnter;
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, mockExit) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smMockData->enterCount == 2);
    assert(smMockData->exitCount == 1);

    teardown();
    tsPass(__func__);
}

void Test_smSetScene_CallsNonNullEnterWithArgsOfTargetScene(void)
{
    setup();
    smTestEnterWithArgs = onEnterWithArgs;
    smMockData = &(MockData){0};
    smMockArgs = &(MockArgs){0};

    assert(
        smAddScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        RES_OK);
    assert(
        smAddScene(mock2.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smSetScene(mock2.name, smMockArgs) == RES_OK);
    assert(smMockArgs->flag);

    teardown();
    tsPass(__func__);
}

// -- smGetCurrentSceneName

void Test_smGetCurrentSceneName_FailsPreCreateScene(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    teardown();
    tsPass(__func__);
}

void Test_smGetCurrentSceneName_ReturnsCurrentSceneName(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(strcmp(smGetCurrentSceneName(), mock.name) == 0);
    teardown();
    tsPass(__func__);
}

// -- smRemoveScene

void Test_smRemoveScene_FailsPreCreateScene(void)
{
    setup();
    assert(smRemoveScene(mock.name) == RES_SCENE_NOT_FOUND);
    teardown();
    tsPass(__func__);
}

void Test_smRemoveScene_FailsToDeleteCurrentScene(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smRemoveScene(mock.name) == RES_CANT_DEL_CURR_SCENE);
    teardown();
    tsPass(__func__);
}

void Test_smRemoveScene_AcceptsNonCurrentScene(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smRemoveScene(mock.name) == RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_smRemoveScene_RejectsEmptyName(void)
{
    setup();
    assert(smRemoveScene("") == RES_EMPTY_ARG);
    teardown();
    tsPass(__func__);
}

void Test_smRemoveScene_FailsWhenDeletingSameSceneTwice(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smRemoveScene(mock.name) == RES_OK);
    assert(smRemoveScene(mock.name) == RES_SCENE_NOT_FOUND);
    teardown();
    tsPass(__func__);
}

// -- smGetSceneCount

void Test_smGetSceneCount_ReturnsZeroPostStart(void)
{
    setup();
    assert(smGetSceneCount() == 0);
    teardown();
    tsPass(__func__);
}

void Test_smGetSceneCount_ReturnsCorrectSceneCountPostCreateScene(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smGetSceneCount() == 1);
    teardown();
    tsPass(__func__);
}

void Test_smGetSceneCount_ReturnsCorrectSceneCountPostDeleteScene(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smRemoveScene(mock.name) == RES_OK);
    assert(smGetSceneCount() == 0);
    teardown();
    tsPass(__func__);
}

// Lifecycle Functions

// -- smUpdate

void Test_smUpdate_FailsWhenNullCurrentScene(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    assert(smUpdate(TS_MOCK_DT) == RES_NO_CURR_SCENE);
    teardown();
    tsPass(__func__);
}

void Test_smUpdate_CallsNonNullUpdateOfCurrentScene(void)
{
    setup();
    assert(
        smAddScene(mock.name, nullptr, mockUpdate, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smUpdate(TS_MOCK_DT) == RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_smUpdate_FailsWhenNullUpdate(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smUpdate(TS_MOCK_DT) == RES_NO_UPDATE_FUNC);
    teardown();
    tsPass(__func__);
}

// -- smGetDt

void Test_smGetDt_UsesDefaultDtOnFirstCall(void)
{
    setup();
    float target = 1.0f / DEFAULT_FPS;
    assert(fabsf(smGetDt() - target) < DT_TOLERANCE);
    teardown();
    tsPass(__func__);
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
    tsPass(__func__);
}

void Test_smGetDt_FailsWhenClockGettimeFails(void)
{
    setup();
    smMockClockGettimeFails = true;
    assert(smGetDt() == (float) RES_CLOCK_GETTIME_FAIL);
    teardown();
    tsPass(__func__);
}


// -- smDraw

void Test_smDraw_FailsWhenNullCurrentScene(void)
{
    setup();
    assert(!smGetCurrentSceneName());
    assert(smDraw() == RES_NO_CURR_SCENE);
    teardown();
    tsPass(__func__);
}

void Test_smDraw_CallsValidDrawFunction(void)
{
    setup();
    assert(
        smAddScene(mock.name, nullptr, nullptr, mockDraw, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smDraw() == RES_OK);
    teardown();
    tsPass(__func__);
}

void Test_smDraw_FailsWhenNullDraw(void)
{
    setup();
    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smDraw() == RES_NO_DRAW_FUNC);
    teardown();
    tsPass(__func__);
}

// Stop Related

void Test_smStop_CallsNonNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smAddScene(mock.name, nullptr, nullptr, nullptr, mockExit) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smStop() == RES_OK);
    assert(smMockData->exitCount == 1);

    resetHooks();
    tsPass(__func__);
}

void Test_smStop_SkipsNullExitOfCurrentScene(void)
{
    setup();
    smTestExit = onExit;
    smMockData = &(MockData){0};

    assert(
        smAddScene(mock.name, mockEnter, nullptr, nullptr, nullptr) ==
        RES_OK);
    assert(smSetScene(mock.name, nullptr) == RES_OK);
    assert(smStop() == RES_OK);
    assert(smMockData->exitCount == 0);

    resetHooks();
    tsPass(__func__);
}

// Post-Stop ———————————————————————————————————————————————————————————————————————————————————————

// Start Related

void Test_smHasStarted_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smIsRunning());
    tsPass(__func__);
}

// Scene Functions

void Test_smAddScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(
        smAddScene(nullptr, nullptr, nullptr, nullptr, nullptr) ==
        RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smSceneExists_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smSceneExists(nullptr));
    tsPass(__func__);
}

void Test_smSetScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smSetScene(nullptr, nullptr) == RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smGetCurrentSceneName_FailsPostStop(void)
{
    setup();
    teardown();
    assert(!smGetCurrentSceneName());
    tsPass(__func__);
}

void Test_smRemoveScene_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smRemoveScene(nullptr) == RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smGetSceneCount_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smGetSceneCount() == RES_NOT_RUNNING);
    tsPass(__func__);
}

// Lifecycle Functions

void Test_smUpdate_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smUpdate(TS_MOCK_DT) == RES_NOT_RUNNING);
    tsPass(__func__);
}

void Test_smDraw_FailsPostStop(void)
{
    setup();
    teardown();
    assert(smDraw() == RES_NOT_RUNNING);
    tsPass(__func__);
}

// Stop Related

void Test_smStop_IsIdempotentPostStop(void)
{
    assert(smStart() == RES_OK);
    assert(smStop() == RES_OK);
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++)
    {
        assert(smStop() == RES_NOT_RUNNING);
    }
    tsPass(__func__);
}

// Stress tests ————————————————————————————————————————————————————————————————————————————————————

// Scene Functions

void TestStress_smAddScene_CreatingMultipleScenesCausesNoSkips(void)
{
    setup();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        assert(
            smAddScene(buf, mockEnter, nullptr, nullptr, nullptr) ==
            RES_OK);
    }
    assert(smGetSceneCount() == STRESS_ITERATIONS);
    teardown();
    tsPass(__func__);
}

void TestStress_smSetScene_SettingScenesOftenCausesNoSkips(void)
{
    setup();
    char buf[8];
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        assert(
            smAddScene(buf, mockEnter, nullptr, nullptr, nullptr) ==
            RES_OK);
    }
    int counter = 0;
    for (int i = 0; i < STRESS_ITERATIONS; i++)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        assert(smSetScene(buf, nullptr) == RES_OK);

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
    tsPass(__func__);
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
            smAddScene(buf, mockEnter, nullptr, nullptr, nullptr) ==
            RES_OK);
    }
    teardown();
    tsPass(__func__);
}

// Main ————————————————————————————————————————————————————————————————————————————————————————————

int main()
{
    puts("\nBASE TESTS");
    Test_smStop_SucceedsPostStart();

    puts("\nPRE-START TESTING");
    puts("• Start Related");
    Test_smHasStarted_FailsPreStart();
    puts("• Scene Functions");
    Test_smAddScene_FailsPreStart();
    Test_smSceneExists_FailsPreStart();
    Test_smSetScene_FailsPreStart();
    Test_smGetCurrentSceneName_FailsPreStart();
    Test_smRemoveScene_FailsPreStart();
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
    puts(" • smAddScene");
    puts("  • Name Related");
    Test_smAddScene_AcceptsValidName();
    Test_smAddScene_RejectsExistingName();
    Test_smAddScene_RejectsNullName();
    Test_smAddScene_RejectsEmptyName();

    puts("  • Scene Functions Related");
    Test_smAddScene_AcceptsAllValidFunctionCombinations();
    Test_smAddScene_RejectsValidNameAndAllNullFunctions();
    Test_smAddScene_FailsWhenSceneAllocFails();
    Test_smAddScene_FailsWhenNameAllocFails();
    Test_smAddScene_FailsWhenMapEntryAllocFails();
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
    puts(" • smRemoveScene");
    Test_smRemoveScene_FailsPreCreateScene();
    Test_smRemoveScene_FailsToDeleteCurrentScene();
    Test_smRemoveScene_AcceptsNonCurrentScene();
    Test_smRemoveScene_RejectsEmptyName();
    Test_smRemoveScene_FailsWhenDeletingSameSceneTwice();
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
    Test_smAddScene_FailsPostStop();
    Test_smSceneExists_FailsPostStop();
    Test_smSetScene_FailsPostStop();
    Test_smGetCurrentSceneName_FailsPostStop();
    Test_smRemoveScene_FailsPostStop();
    Test_smGetSceneCount_FailsPostStop();
    puts("• Lifecycle Functions");
    Test_smUpdate_FailsPostStop();
    Test_smDraw_FailsPostStop();
    puts("• Stop Related");
    Test_smStop_IsIdempotentPostStop();

    puts("\nSTRESS TESTING");
    TestStress_smAddScene_CreatingMultipleScenesCausesNoSkips();
    TestStress_smSetScene_SettingScenesOftenCausesNoSkips();
    TestStress_smStop_FreeingMultipleScenesCausesNoSkips();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
}
