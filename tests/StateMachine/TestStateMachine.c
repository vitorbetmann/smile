// TODO(#1) add tests for failed memory allocation in SM_RegisterState
// TODO(#10) implement tests for `SM_Internal_EnableWarnings()`

/*
 * Test Naming Convention:
 *
 * Each test function is named following the pattern:
 *
 *   Test_<FunctionName>_<ExpectedBehavior>
 *
 * where:
 *   - Test_SM_: prefix indicating this is a StateMachine test.
 *   - <FunctionName>: the name of the StateMachine function or feature under
 * test.
 *   - <ExpectedBehavior>: a short description of what the test checks,
 *                        often stating the expected outcome or condition.
 *
 * Example:
 *   Test_SM_RegisterState_ReturnsFalseIfNameIsNULL
 *     Tests that SM_RegisterState returns false when the provided name is NULL.
 *
 * This naming pattern helps organize and clearly communicate test purposes.
 * @author Vitor Betmann
 */

#include "../include/StateMachine.h"
#include "../src/StateMachine/StateMachineInternal.h"
#include "StateMachineTest.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// --------------------------------------------------
// Data types
// --------------------------------------------------

#define SM_COMP_NAME(name1, name2) strcmp(name1, name2) == 0
#define TEST_PASS(funcName) printf("\t[PASS] %s\n", funcName)

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef struct {
  bool hasEntered, hasEnteredArgs;
  int enteredTimes;

  bool hasUpdated;
  bool hasDrawn;

  bool hasExited;
  int exitedTimes;
} MockData;

typedef struct {
  bool flag;
} MockStateArgs;

// --------------------------------------------------
// variables
// --------------------------------------------------
static unsigned int MULTIPLE_STATES = 1000;
static float mockDT = 0.016;
MockData md;
static State mockState = {.name = "mockState"};

// --------------------------------------------------
// Mock Functions
// --------------------------------------------------

void mockEnter(void *args) {
  md.hasEntered = true;
  md.enteredTimes++;

  if (args) {
    MockStateArgs *msa = (MockStateArgs *)args;
    md.hasEnteredArgs = msa->flag;
  }
}
void mockUpdate(float dt) { md.hasUpdated = true; }
void mockDraw(void) { md.hasDrawn = true; }
void mockExit(void) {
  md.hasExited = true;
  md.exitedTimes++;
}

// --------------------------------------------------
// Pre-initialization - Internal
// --------------------------------------------------

void Test_SM_Internal_SetCurrState_ReturnsFalseBeforeInitialization(void) {
  assert(!SM_Internal_SetCurrState(&mockState));
  TEST_PASS("Test_SM_Internal_SetCurrState_ReturnsFalseBeforeInitialization");
}
void Test_SM_Internal_GetCurrState_ReturnsNullBeforeInitialization(void) {
  assert(!SM_Internal_GetCurrState());
  TEST_PASS("Test_SM_Internal_GetCurrState_ReturnsNullBeforeInitialization");
}
void Test_SM_Internal_GetState_ReturnsNullBeforeInitialization(void) {
  assert(!SM_Internal_GetState("mockState"));
  TEST_PASS("Test_SM_Internal_GetState_ReturnsNullBeforeInitialization");
}
void Test_SM_Test_GetTracker_ReturnsNullBeforeInitialization(void) {
  assert(!SM_Test_GetTracker());
  TEST_PASS("Test_SM_Test_GetTracker_ReturnsNullBeforeInitialization");
}

void Test_SM_Test_SetCanMalloc_TogglesCorrectly(void) {
  TEST_PASS("Test_SM_Test_SetCanMalloc_TogglesCorrectly (true)");
  assert(SM_Test_SetCanMalloc(true));

  TEST_PASS("Test_SM_Test_SetCanMalloc_TogglesCorrectly (false)");
  assert(!SM_Test_SetCanMalloc(false));
}

void Test_SM_Test_Malloc_ReturnsNullIfNotAllowed(void) {
  SM_Test_SetCanMalloc(false);
  void *temp = SM_Test_Malloc(sizeof(int));
  TEST_PASS("Test_SM_Test_Malloc_ReturnsNullIfNotAllowed");
  assert(!temp);

  SM_Test_SetCanMalloc(true);
  temp = SM_Test_Malloc(sizeof(int));
  TEST_PASS("Test_SM_Test_Malloc_ReturnsValidPointerIfAllowed");
  assert(temp);
  free(temp);
}

// --------------------------------------------------
// Pre-initialization - Public
// --------------------------------------------------

void Test_SM_IsInitialized_ReturnsFalseBeforeInitialization(void) {
  assert(!SM_IsInitialized());
  TEST_PASS("Test_SM_IsInitialized_ReturnsFalseBeforeInitialization");
}

void Test_SM_RegisterState_ReturnsFalseBeforeInitialization(void) {
  assert(!SM_RegisterState("setToFail", mockEnter, mockUpdate, mockDraw,
                           mockExit));
  TEST_PASS("Test_SM_RegisterState_ReturnsFalseBeforeInitialization");
}

void Test_SM_ChangeStateTo_ReturnsFalseBeforeInitialization(void) {
  assert(!SM_ChangeStateTo("testBeforeInit", NULL));
  TEST_PASS("Test_SM_ChangeStateTo_ReturnsFalseBeforeInitialization");
}

void Test_SM_Update_ReturnsFalseBeforeInitialization(void) {
  assert(!SM_Update(mockDT));
  TEST_PASS("Test_SM_Update_ReturnsFalseBeforeInitialization");
}

void Test_SM_Draw_ReturnsFalseBeforeInitialization(void) {
  assert(!SM_Draw());
  TEST_PASS("Test_SM_Draw_ReturnsFalseBeforeInitialization");
}

void Test_SM_Shutdown_ReturnsFalseBeforeInitialization(void) {
  assert(!SM_Shutdown());
  TEST_PASS("Test_SM_Shutdown_ReturnsFalseBeforeInitialization");
}

void Test_SM_GetCurrStateName_ReturnsNullBeforeInitialization(void) {
  assert(!SM_GetCurrStateName());
  TEST_PASS("Test_SM_GetCurrStateName_ReturnsNullBeforeInitialization");
}

// --------------------------------------------------
// Initialization
// --------------------------------------------------

void Test_SM_Init_ReturnsFalseIfMallocFails(void) {
  SM_Test_SetCanMalloc(false);
  SM_Init();
  assert(!SM_Test_GetTracker());
  TEST_PASS("Test_SM_Init_ReturnsFalseIfMallocFails");
}

void Test_SM_Init_ReturnsTrueAndInitializesTracker(void) {
  SM_Test_SetCanMalloc(true);
  SM_Init();
  assert(SM_Test_GetTracker());
  TEST_PASS("Test_SM_Init_ReturnsTrueAndInitializesTracker");
}

void Test_SM_Init_ReturnsFalseIfCalledTwice(void) {
  assert(!SM_Init());
  TEST_PASS("Test_SM_Init_ReturnsFalseIfCalledTwice");
}

// --------------------------------------------------
// Post-Initialization - Internal
// --------------------------------------------------

void Test_SM_Test_GetTracker_ReturnsTrackerAfterInitialization(void) {
  assert(SM_Test_GetTracker());
  TEST_PASS("Test_SM_Test_GetTracker_ReturnsTrackerAfterInitialization");
}

// --------------------------------------------------
// Pre-Registration - Internal
// --------------------------------------------------

void Test_SM_Internal_GetCurrState_ReturnsNullIfNoCurrentState(void) {
  assert(!SM_Internal_GetCurrState());
  TEST_PASS("Test_SM_Internal_GetCurrState_ReturnsNullIfNoCurrentState");
}

void Test_SM_Internal_SetCurrState_SetsValidStateCorrectly(void) {
  SM_Internal_SetCurrState(&mockState);
  assert(SM_COMP_NAME(SM_Internal_GetCurrState()->name, mockState.name));
  TEST_PASS("Test_SM_Internal_SetCurrState_SetsValidStateCorrectly");
}

void Test_SM_Internal_SetCurrState_SetsNULLStateCorrectly(void) {
  SM_Internal_SetCurrState(NULL);
  assert(!SM_Internal_GetCurrState());
  TEST_PASS("Test_SM_Internal_SetCurrState_SetsNULLStateCorrectly");
}

// --------------------------------------------------
// State Registration
// --------------------------------------------------

void Test_SM_RegisterState_ReturnsFalseIfNameIsNULL(void) {
  assert(!SM_RegisterState(NULL, NULL, NULL, NULL, NULL));
  TEST_PASS("Test_SM_RegisterState_ReturnsFalseIfNameIsNULL");
}

void Test_SM_RegisterState_ReturnsFalseIfNameIsEmpty(void) {
  assert(!SM_RegisterState("", NULL, NULL, NULL, NULL));
  TEST_PASS("Test_SM_RegisterState_ReturnsFalseIfNameIsEmpty");
}

void Test_SM_RegisterState_ReturnsTrueIfAllFunctionsPresent(void) {
  assert(SM_RegisterState("testNoNULL", mockEnter, mockUpdate, mockDraw,
                          mockExit));
  TEST_PASS("Test_SM_RegisterState_ReturnsTrueIfAllFunctionsPresent");
}

void Test_SM_RegisterState_ReturnsFalseIfNameAlreadyExists(void) {
  assert(!SM_RegisterState("testNoNULL", mockEnter, NULL, NULL, NULL));
  TEST_PASS("Test_SM_RegisterState_ReturnsFalseIfNameAlreadyExists");
}

void Test_SM_RegisterState_ReturnsTrueIfEnterAndUpdateNULL(void) {
  assert(SM_RegisterState("testNULLEnterAndUpdate", NULL, NULL, mockDraw,
                          mockExit));
  TEST_PASS("Test_SM_RegisterState_ReturnsTrueIfEnterAndUpdateNULL");
}

void Test_SM_RegisterState_ReturnsTrueIfDrawAndExitNULL(void) {
  assert(SM_RegisterState("testNULLDrawAndExit", mockEnter, mockUpdate, NULL,
                          NULL));
  TEST_PASS("Test_SM_RegisterState_ReturnsTrueIfDrawAndExitNULL");
}

void Test_SM_RegisterState_ReturnsFalseIfAllFunctionsNULL(void) {
  assert(!SM_RegisterState("testAllNULL", NULL, NULL, NULL, NULL));
  TEST_PASS("Test_SM_RegisterState_ReturnsFalseIfAllFunctionsNULL");
}

// --------------------------------------------------
// Post-Registration - Internal
// --------------------------------------------------

void Test_SM_Internal_GetState_ReturnsRegisteredStateByName(void) {
  SM_RegisterState("mockState", mockEnter, NULL, NULL, NULL);
  assert(SM_COMP_NAME(SM_Internal_GetState("mockState")->name, mockState.name));
  TEST_PASS("Test_SM_Internal_GetState_ReturnsRegisteredStateByName");
}

// --------------------------------------------------
// Pre-transitions
// --------------------------------------------------

void Test_SM_GetCurrStateName_ReturnsNullIfCurrentStateIsNULL(void) {
  assert(!SM_GetCurrStateName());
  TEST_PASS("Test_SM_GetCurrStateName_ReturnsNullIfCurrentStateIsNULL");
}

void Test_SM_Update_ReturnsFalseIfCurrentStateIsNULL(void) {
  assert(!SM_Update(mockDT));
  TEST_PASS("Test_SM_Update_ReturnsFalseIfCurrentStateIsNULL");
}

void Test_SM_Draw_ReturnsFalseIfCurrentStateIsNULL(void) {
  assert(!SM_Draw());
  TEST_PASS("Test_SM_Draw_ReturnsFalseIfCurrentStateIsNULL");
}

// --------------------------------------------------
// Transitions
// --------------------------------------------------

void Test_SM_ChangeStateTo_ReturnsTrueChangingFromNULLToValidState(void) {
  assert(SM_ChangeStateTo("testNoNULL", NULL));
  puts(
      "\t[PASS] Test_SM_ChangeStateTo_ReturnsTrueChangingFromNULLToValidState");
}

void Test_SM_ChangeStateTo_ReturnsFalseIfNameIsNULL(void) {
  assert(!SM_ChangeStateTo(NULL, NULL));
  TEST_PASS("Test_SM_ChangeStateTo_ReturnsFalseIfNameIsNULL");
}

void Test_SM_ChangeStateTo_ReturnsFalseIfStateIsUnregistered(void) {
  assert(!SM_ChangeStateTo("testUnregistered", NULL));
  TEST_PASS("Test_SM_ChangeStateTo_ReturnsFalseIfStateIsUnregistered");
}

void Test_SM_ChangeStateTo_ReturnsTrueChangingFromOneValidStateToAnother(void) {
  assert(SM_ChangeStateTo("testNULLEnterAndUpdate", NULL));
  TEST_PASS(
      ""
      "Test_SM_ChangeStateTo_ReturnsTrueChangingFromOneValidStateToAnother");
}

void Test_SM_ChangeStateTo_CallsExitFunctionOfCurrentState(void) {
  md.hasExited = false;
  SM_ChangeStateTo("testNULLDrawAndExit", NULL);
  assert(md.hasExited);
  TEST_PASS("Test_SM_ChangeStateTo_CallsExitFunctionOfCurrentState");
}

void Test_SM_ChangeStateTo_CallsEnterFunctionOfNewState(void) {
  md.hasEntered = false;
  SM_ChangeStateTo("testNoNULL", NULL);
  assert(md.hasEntered);
  TEST_PASS("Test_SM_ChangeStateTo_CallsEnterFunctionOfNewState");
}

void Test_SM_ChangeStateTo_CallsExitAndEnterIfChangingToSameState(void) {
  md.hasEntered = false;
  md.hasExited = false;
  SM_ChangeStateTo("testNoNULL", NULL);
  assert(md.hasEntered && md.hasExited);
  TEST_PASS("Test_SM_ChangeStateTo_CallsExitAndEnterIfChangingToSameState");
}

void Test_SM_ChangeStateTo_CallsEnterFunctionWithArgs(void) {
  MockStateArgs temp = {.flag = true};
  SM_ChangeStateTo("testNoNULL", &temp);
  assert(md.hasEnteredArgs);
  TEST_PASS("Test_SM_ChangeStateTo_CallsEnterFunctionWithArgs");
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

void Test_SM_GetCurrStateName_ReturnsCurrentStateName(void) {
  assert(SM_COMP_NAME(SM_GetCurrStateName(), "testNoNULL"));
  TEST_PASS("Test_SM_GetCurrStateName_ReturnsCurrentStateName");
}

// --------------------------------------------------
// Checks
// --------------------------------------------------

void Test_SM_IsInitialized_ReturnsTrueAfterInitialization(void) {
  assert(SM_IsInitialized());
  TEST_PASS("Test_SM_IsInitialized_ReturnsTrueAfterInitialization");
}

void Test_SM_IsStateRegistered_ReturnsTrueForValidStateName(void) {
  assert(SM_IsStateRegistered("testNoNULL"));
  TEST_PASS("Test_SM_IsStateRegistered_ReturnsTrueForValidStateName");
}

void Test_SM_IsStateRegistered_ReturnsFalseForInvalidStateName(void) {
  assert(!SM_IsStateRegistered("testUnregistered"));
  TEST_PASS("Test_SM_IsStateRegistered_ReturnsFalseForInvalidStateName");
}

// --------------------------------------------------
// Update and Draw
// --------------------------------------------------

void Test_SM_Update_CallsValidUpdateFunction(void) {
  SM_Update(mockDT);
  assert(md.hasUpdated);
  TEST_PASS("Test_SM_Update_CallsValidUpdateFunction");
}

void Test_SM_Draw_CallsValidDrawFunction(void) {
  SM_Draw();
  assert(md.hasDrawn);
  TEST_PASS("Test_SM_Draw_CallsValidDrawFunction");
}

void Test_SM_Update_CallsValidUpdateFunctionEvenIfNullUpdate(void) {
  SM_ChangeStateTo("testNULLEnterAndUpdate", NULL);
  SM_Update(mockDT);
  assert(md.hasUpdated);
  TEST_PASS("Test_SM_Update_CallsValidUpdateFunctionEvenIfNullUpdate");
}

void Test_SM_Draw_CallsValidDrawFunctionEvenIfNullDraw(void) {
  SM_ChangeStateTo("testNULLDrawAndExit", NULL);
  SM_Draw();
  assert(md.hasDrawn);
  TEST_PASS("Test_SM_Draw_CallsValidDrawFunctionEvenIfNullDraw");
}

// --------------------------------------------------
// Shutdown
// --------------------------------------------------

void Test_SM_Shutdown_CallsExitFunctionOfCurrentState(void) {
  SM_ChangeStateTo("testNoNULL", NULL);
  md.hasExited = false;
  SM_Shutdown();
  assert(md.hasExited);
  TEST_PASS("Test_SM_Shutdown_CallsExitFunctionOfCurrentState");
}

void Test_SM_Shutdown_SkipsExitIfNull(void) {
  SM_Init();
  SM_RegisterState("testNoExit", mockEnter, NULL, NULL, NULL);
  SM_ChangeStateTo("testNoExit", NULL);
  md.hasExited = false;
  SM_Shutdown();
  assert(!md.hasExited);
  TEST_PASS("Test_SM_Shutdown_SkipsExitIfNull");
}

void Test_SM_Shutdown_SetsTrackerToNull(void) {
  assert(!SM_Test_GetTracker());
  TEST_PASS("Test_SM_Shutdown_SetsTrackerToNull");
}

void Test_SM_Shutdown_SetsStateCountToZero(void) {
  assert(SM_Test_GetStateCount() == 0);
  TEST_PASS("Test_SM_Shutdown_SetsStateCountToZero");
}

// --------------------------------------------------
// Post-Shutdown access - Public
// --------------------------------------------------

void Test_SM_IsInitialized_ReturnsFalseAfterShutdown(void) {
  assert(!SM_IsInitialized());
  TEST_PASS("Test_SM_IsInitialized_ReturnsFalseAfterShutdown");
}

void Test_SM_GetCurrStateName_ReturnsNullAfterShutdown(void) {
  assert(!SM_GetCurrStateName());
  TEST_PASS("Test_SM_GetCurrStateName_ReturnsNullAfterShutdown");
}

void Test_SM_RegisterState_ReturnsFalseAfterShutdown(void) {
  assert(!SM_RegisterState("setToFail", mockEnter, mockUpdate, mockDraw,
                           mockExit));
  TEST_PASS("Test_SM_RegisterState_ReturnsFalseAfterShutdown");
}

void Test_SM_ChangeStateTo_ReturnsFalseAfterShutdown(void) {
  assert(!SM_ChangeStateTo("testBeforeInit", NULL));
  TEST_PASS("Test_SM_ChangeStateTo_ReturnsFalseAfterShutdown");
}

void Test_SM_Shutdown_ReturnsFalseIfCalledMultipleTimesAfterShutdown(void) {
  assert(!SM_Shutdown());
  assert(!SM_Shutdown());
  assert(!SM_Shutdown());
  TEST_PASS("Test_SM_Shutdown_ReturnsFalseIfCalledMultipleTimesAfterShutdown");
}

// --------------------------------------------------
// Post-Shutdown access - Internal
// --------------------------------------------------

void Test_SM_Internal_SetCurrState_ReturnsFalseAfterShutdown(void) {
  assert(!SM_Internal_SetCurrState(&mockState));
  TEST_PASS("Test_SM_Internal_SetCurrState_ReturnsFalseAfterShutdown");
}
void Test_SM_Internal_GetCurrState_ReturnsNullAfterShutdown(void) {
  assert(!SM_Internal_GetCurrState());
  TEST_PASS("Test_SM_Internal_GetCurrState_ReturnsNullAfterShutdown");
}
void Test_SM_Internal_GetState_ReturnsNullAfterShutdown(void) {
  assert(!SM_Internal_GetState("testNoNULL"));
  TEST_PASS("Test_SM_Internal_GetState_ReturnsNullAfterShutdown");
}
void Test_SM_Test_GetTracker_ReturnsNullAfterShutdown(void) {
  assert(!SM_Test_GetTracker());
  TEST_PASS("Test_SM_Test_GetTracker_ReturnsNullAfterShutdown");
}
void Test_SM_Update_ReturnsFalseAfterShutdown(void) {
  assert(!SM_Update(mockDT));
  TEST_PASS("Test_SM_Update_ReturnsFalseAfterShutdown");
}
void Test_SM_Draw_ReturnsFalseAfterShutdown(void) {
  assert(!SM_Draw());
  TEST_PASS("Test_SM_Draw_ReturnsFalseAfterShutdown");
}

// --------------------------------------------------
// Stress tests
// --------------------------------------------------

void Test_SM_RegisteringMultipleStatesCausesNoSkips(void) {
  SM_Init();
  for (int i = 0; i < MULTIPLE_STATES; i++) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", i);
    SM_RegisterState(buffer, mockEnter, mockUpdate, mockDraw, mockExit);
  }
  assert(SM_Test_GetStateCount() == MULTIPLE_STATES);
  printf("\t[PASS] Test_SM_RegisteringMultipleStatesCausesNoSkips: %d states "
         "registered\n",
         MULTIPLE_STATES);
}

void Test_SM_ChangingStatesOftenCausesNoSkips(void) {
  md.enteredTimes = 0;
  md.exitedTimes = 0;
  for (int i = 0; i < MULTIPLE_STATES; i++) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", i);
    SM_ChangeStateTo(buffer, NULL);
  }

  assert(md.enteredTimes == MULTIPLE_STATES &&
         md.exitedTimes == MULTIPLE_STATES - 1);

  printf(
      "\t[PASS] Test_SM_ChangingStatesOften_CausesNoSkips: %d state changes\n",
      MULTIPLE_STATES);
}

void Test_SM_Shutdown_FreeingMultipleStatesCausesNoSkips(void) {
  SM_Shutdown();
  assert(SM_Test_GetStateCount() == 0);
  printf("\t[PASS] Test_SM_Shutdown_FreeingMultipleStatesCausesNoSkips\n");
}

// --------------------------------------------------
// Finger's crossed!
// --------------------------------------------------

int main() {
  puts("");
  puts("Testing Pre-initialization - Internal");
  Test_SM_Internal_SetCurrState_ReturnsFalseBeforeInitialization();
  Test_SM_Internal_GetCurrState_ReturnsNullBeforeInitialization();
  Test_SM_Internal_GetState_ReturnsNullBeforeInitialization();
  Test_SM_Test_GetTracker_ReturnsNullBeforeInitialization();
  puts("");

  puts("Testing Pre-initialization - Public");
  Test_SM_IsInitialized_ReturnsFalseBeforeInitialization();
  Test_SM_RegisterState_ReturnsFalseBeforeInitialization();
  Test_SM_ChangeStateTo_ReturnsFalseBeforeInitialization();
  Test_SM_Update_ReturnsFalseBeforeInitialization();
  Test_SM_Draw_ReturnsFalseBeforeInitialization();
  Test_SM_Shutdown_ReturnsFalseBeforeInitialization();
  Test_SM_GetCurrStateName_ReturnsNullBeforeInitialization();
  puts("");

  puts("Testing Initialization");
  Test_SM_Init_ReturnsFalseIfMallocFails();
  Test_SM_Init_ReturnsTrueAndInitializesTracker();
  Test_SM_Init_ReturnsFalseIfCalledTwice();
  puts("");

  puts("Testing Post-Initialization - Internal");
  Test_SM_Test_GetTracker_ReturnsTrackerAfterInitialization();
  puts("");

  puts("Testing Pre-Registration - Internal");
  Test_SM_Internal_GetCurrState_ReturnsNullIfNoCurrentState();
  Test_SM_Internal_SetCurrState_SetsValidStateCorrectly();
  Test_SM_Internal_SetCurrState_SetsNULLStateCorrectly();
  puts("");

  puts("Testing State Registration");
  Test_SM_RegisterState_ReturnsFalseIfNameIsNULL();
  Test_SM_RegisterState_ReturnsFalseIfNameIsEmpty();
  Test_SM_RegisterState_ReturnsTrueIfAllFunctionsPresent();
  Test_SM_RegisterState_ReturnsFalseIfNameAlreadyExists();
  Test_SM_RegisterState_ReturnsTrueIfEnterAndUpdateNULL();
  Test_SM_RegisterState_ReturnsTrueIfDrawAndExitNULL();
  Test_SM_RegisterState_ReturnsFalseIfAllFunctionsNULL();
  puts("");

  puts("Testing Post-Registration - Internal");
  Test_SM_Internal_GetState_ReturnsRegisteredStateByName();
  puts("");

  puts("Testing Pre-transitions");
  Test_SM_GetCurrStateName_ReturnsNullIfCurrentStateIsNULL();
  Test_SM_Update_ReturnsFalseIfCurrentStateIsNULL();
  Test_SM_Draw_ReturnsFalseIfCurrentStateIsNULL();
  puts("");

  puts("Testing Transitions");
  Test_SM_ChangeStateTo_ReturnsTrueChangingFromNULLToValidState();
  Test_SM_ChangeStateTo_ReturnsFalseIfNameIsNULL();
  Test_SM_ChangeStateTo_ReturnsFalseIfStateIsUnregistered();
  Test_SM_ChangeStateTo_ReturnsTrueChangingFromOneValidStateToAnother();
  Test_SM_ChangeStateTo_CallsExitFunctionOfCurrentState();
  Test_SM_ChangeStateTo_CallsEnterFunctionOfNewState();
  Test_SM_ChangeStateTo_CallsExitAndEnterIfChangingToSameState();
  Test_SM_ChangeStateTo_CallsEnterFunctionWithArgs();
  puts("");

  puts("Testing Getters");
  Test_SM_GetCurrStateName_ReturnsCurrentStateName();
  puts("");

  puts("Testing Checks");
  Test_SM_IsInitialized_ReturnsTrueAfterInitialization();
  Test_SM_IsStateRegistered_ReturnsTrueForValidStateName();
  Test_SM_IsStateRegistered_ReturnsFalseForInvalidStateName();
  puts("");

  puts("Testing Update and Draw");
  Test_SM_Update_CallsValidUpdateFunction();
  Test_SM_Draw_CallsValidDrawFunction();
  Test_SM_Update_CallsValidUpdateFunctionEvenIfNullUpdate();
  Test_SM_Draw_CallsValidDrawFunctionEvenIfNullDraw();
  puts("");

  puts("Testing Shutdown");
  Test_SM_Shutdown_CallsExitFunctionOfCurrentState();
  Test_SM_Shutdown_SkipsExitIfNull();
  Test_SM_Shutdown_SetsTrackerToNull();
  Test_SM_Shutdown_SetsStateCountToZero();
  puts("");

  puts("Testing Post-Shutdown Access - Public");
  Test_SM_IsInitialized_ReturnsFalseAfterShutdown();
  Test_SM_GetCurrStateName_ReturnsNullAfterShutdown();
  Test_SM_RegisterState_ReturnsFalseAfterShutdown();
  Test_SM_ChangeStateTo_ReturnsFalseAfterShutdown();
  Test_SM_Shutdown_ReturnsFalseIfCalledMultipleTimesAfterShutdown();
  puts("");

  puts("Testing Post-Shutdown Access - Internal");
  Test_SM_Internal_SetCurrState_ReturnsFalseAfterShutdown();
  Test_SM_Internal_GetCurrState_ReturnsNullAfterShutdown();
  Test_SM_Internal_GetState_ReturnsNullAfterShutdown();
  Test_SM_Test_GetTracker_ReturnsNullAfterShutdown();
  Test_SM_Update_ReturnsFalseAfterShutdown();
  Test_SM_Draw_ReturnsFalseAfterShutdown();
  puts("");

  puts("Testing Stress Tests");
  Test_SM_RegisteringMultipleStatesCausesNoSkips();
  Test_SM_ChangingStatesOftenCausesNoSkips();
  Test_SM_Shutdown_FreeingMultipleStatesCausesNoSkips();
  puts("");

  puts("All tests completed successfully!");
  return 0;
}