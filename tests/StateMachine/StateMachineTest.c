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
 *   Test_SM_Init_FailsIfMallocFails
 *
 * @author Vitor Betmann
 */

#include "../include/StateMachine.h"
#include "../src/StateMachine/StateMachineInternal.h"
#include "../src/_Internal/Test/TestInternal.h"
#include "StateMachineTest.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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
    MockStateArgs *msa = (MockStateArgs *) args;
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
// Pre-Init
// --------------------------------------------------

// Init Related -------------------------------------

void Test_SM_IsInitialized_FailsPreInit(void) {
  assert(!SM_IsInitialized());
  TEST_Pass("Test_SM_IsInitialized_FailsPreInit");
}

// State Functions ----------------------------------

void Test_SM_RegisterState_FailsPreInit(void) {
  assert(!SM_RegisterState("setToFail", mockEnter, mockUpdate, mockDraw, mockExit));
  TEST_Pass("Test_SM_RegisterState_FailsPreInit");
}

void Test_SM_ChangeStateTo_FailsPreInit(void) {
  assert(!SM_ChangeStateTo("testPreInit", NULL));
  TEST_Pass("Test_SM_ChangeStateTo_FailsPreInit");
}

void Test_SM_GetCurrStateName_FailsPreInit(void) {
  assert(!SM_GetCurrStateName());
  TEST_Pass("Test_SM_GetCurrStateName_FailsPreInit");
}

void Test_SM_ListStates_FailsPreInit(void) {
  // TODO implement test
  TEST_Pass("Test_SM_ListStates_FailsPreInit");
}

// Lifecycle Functions ------------------------------

void Test_SM_Update_FailsPreInit(void) {
  assert(!SM_Update(mockDT));
  TEST_Pass("Test_SM_Update_FailsPreInit");
}

void Test_SM_Draw_FailsPreInit(void) {
  assert(!SM_Draw());
  TEST_Pass("Test_SM_Draw_FailsPreInit");
}

// Shutdown -----------------------------------------

void Test_SM_Shutdown_FailsPreInit(void) {
  assert(!SM_Shutdown());
  TEST_Pass("Test_SM_Shutdown_FailsPreInit");
}

// --------------------------------------------------
// Init
// --------------------------------------------------

// Mem Alloc Fail -----------------------------------

void Test_SM_Init_FailsIfCallocFails(void) {
  TEST_Disable(CALLOC, 1);
  assert(!SM_Init());
  TEST_Pass("Test_SM_Init_FailsIfCallocFails");
}

// Success ------------------------------------------

void Test_SM_Init_Succeeds(void) {
  assert(SM_Init());
  TEST_Pass("Test_SM_Init_Succeeds");
}

// --------------------------------------------------
// Post-Init
// --------------------------------------------------

// Init ---------------------------------------------

void Test_SM_Init_FailsPostInit(void) {
  assert(!SM_Init());
  TEST_Pass("Test_SM_Init_FailsPostInit");
}

// IsInitialized ------------------------------------

void Test_SM_IsInitialized_SucceedsPostInit(void) {
  assert(SM_IsInitialized());
  TEST_Pass("Test_SM_IsInitialized_SucceedsPostInit");
}

// State Functions ----------------------------------

// -- ListStates Pre-RegisterState ------------------

// TODO

// -- RegisterState ---------------------------------

void Test_SM_RegisterState_RejectsNullStateName(void) {
  assert(!SM_RegisterState(NULL, NULL, NULL, NULL, NULL));
  TEST_Pass("Test_SM_RegisterState_RejectsNullStateName");
}

void Test_SM_RegisterState_RejectsEmptyStateName(void) {
  assert(!SM_RegisterState("", NULL, NULL, NULL, NULL));
  TEST_Pass("Test_SM_RegisterState_RejectsEmptyStateName");
}

void Test_SM_RegisterState_AcceptsAllNonNullArgs(void) {
  assert(SM_RegisterState("testNoNULL", mockEnter, mockUpdate, mockDraw,mockExit));
  TEST_Pass("Test_SM_RegisterState_AcceptsAllNonNullArgs");
}

void Test_SM_RegisterState_RejectsExistingStateName(void) {
  assert(!SM_RegisterState("testNoNULL", mockEnter, NULL, NULL, NULL));
  TEST_Pass("Test_SM_RegisterState_RejectsExistingStateName");
}

void Test_SM_RegisterState_AcceptsNullEnterAndUpdate(void) {
  assert(SM_RegisterState("testNULLEnterAndUpdate", NULL, NULL, mockDraw,mockExit));
  TEST_Pass("Test_SM_RegisterState_AcceptsNullEnterAndUpdate");
}

void Test_SM_RegisterState_AcceptsNullDrawAndExit(void) {
  assert(SM_RegisterState("testNULLDrawAndExit", mockEnter, mockUpdate, NULL,NULL));
  TEST_Pass("Test_SM_RegisterState_AcceptsNullDrawAndExit");
}

void Test_SM_RegisterState_RejectsAllNullArgs(void) {
  assert(!SM_RegisterState("testAllNULL", NULL, NULL, NULL, NULL));
  TEST_Pass("Test_SM_RegisterState_RejectsAllNullArgs");
}

// IsStateRegistered --------------------------------

void Test_SM_IsStateRegistered_AcceptsRegisteredStateName(void) {
  assert(SM_IsStateRegistered("testNoNULL"));
  TEST_Pass("Test_SM_IsStateRegistered_AcceptsRegisteredStateName");
}

void Test_SM_IsStateRegistered_RejectsNonRegisteredStateName(void) {
  assert(!SM_IsStateRegistered("testUnregistered"));
  TEST_Pass("Test_SM_IsStateRegistered_RejectsNonRegisteredStateName");
}

// -- ListStates Post-RegisterState -----------------

// TODO

// -- GetCurrStateName Pre-ChangeStateTo ------------

void Test_SM_GetCurrStateName_FailsPreRegisterState(void) {
  assert(!SM_GetCurrStateName());
  TEST_Pass("Test_SM_GetCurrStateName_FailsPreRegisterState");
}

// -- Lifecycle Functions Pre-ChangeStateTo ---------

void Test_SM_Update_FailsIfNullCurrentState(void) {
  assert(!SM_Update(mockDT));
  TEST_Pass("Test_SM_Update_FailsIfNullCurrentState");
}

void Test_SM_Draw_FailsIfNullCurrentState(void) {
  assert(!SM_Draw());
  TEST_Pass("Test_SM_Draw_FailsIfNullCurrentState");
}

// ChangeStateTo ------------------------------------

void Test_SM_ChangeStateTo_AcceptsValidStateFromNull(void) {
  assert(SM_ChangeStateTo("testNoNULL", NULL));
  puts(
    "\t[PASS] Test_SM_ChangeStateTo_SucceedsChangingFromNullToValidState");
}

void Test_SM_ChangeStateTo_RejectsNullStateName(void) {
  assert(!SM_ChangeStateTo(NULL, NULL));
  TEST_Pass("Test_SM_ChangeStateTo_FailsIfNullName");
}

void Test_SM_ChangeStateTo_RejectsUnregisteredStateName(void) {
  assert(!SM_ChangeStateTo("testUnregistered", NULL));
  TEST_Pass("Test_SM_ChangeStateTo_FailsForUnregisteredState");
}

void Test_SM_ChangeStateTo_SucceedsChangingFromOneValidStateToAnother(void) {
  assert(SM_ChangeStateTo("testNULLEnterAndUpdate", NULL));
  TEST_Pass("Test_SM_ChangeStateTo_SucceedsChangingFromOneValidStateToAnother");
}

void Test_SM_ChangeStateTo_CallsExitFunctionOfCurrentState(void) {
  md.hasExited = false;
  SM_ChangeStateTo("testNULLDrawAndExit", NULL);
  assert(md.hasExited);
  TEST_Pass("Test_SM_ChangeStateTo_CallsExitFunctionOfCurrentState");
}

void Test_SM_ChangeStateTo_CallsEnterFunctionOfNewState(void) {
  md.hasEntered = false;
  SM_ChangeStateTo("testNoNULL", NULL);
  assert(md.hasEntered);
  TEST_Pass("Test_SM_ChangeStateTo_CallsEnterFunctionOfNewState");
}

void Test_SM_ChangeStateTo_CallsExitAndEnterIfChangingToSameState(void) {
  md.hasEntered = false;
  md.hasExited = false;
  SM_ChangeStateTo("testNoNULL", NULL);
  assert(md.hasEntered && md.hasExited);
  TEST_Pass("Test_SM_ChangeStateTo_CallsExitAndEnterIfChangingToSameState");
}

void Test_SM_ChangeStateTo_CallsEnterFunctionWithArgs(void) {
  MockStateArgs temp = {.flag = true};
  SM_ChangeStateTo("testNoNULL", &temp);
  assert(md.hasEnteredArgs);
  TEST_Pass("Test_SM_ChangeStateTo_CallsEnterFunctionWithArgs");
}

// -- GetCurrStateName Post-ChangeStateTo -----------

void Test_SM_GetCurrStateName_ReturnsCurrentStateName(void) {
  assert(strcmp(SM_GetCurrStateName(), "testNoNULL") == 0);
  TEST_Pass("Test_SM_GetCurrStateName_ReturnsCurrentStateName");
}

// --------------------------------------------------
// Lifecycle Functions
// --------------------------------------------------

void Test_SM_Update_CallsValidUpdateFunction(void) {
  SM_Update(mockDT);
  assert(md.hasUpdated);
  TEST_Pass("Test_SM_Update_CallsValidUpdateFunction");
}

void Test_SM_Draw_CallsValidDrawFunction(void) {
  SM_Draw();
  assert(md.hasDrawn);
  TEST_Pass("Test_SM_Draw_CallsValidDrawFunction");
}

void Test_SM_Update_CallsValidUpdateFunctionEvenIfNullUpdate(void) {
  SM_ChangeStateTo("testNULLEnterAndUpdate", NULL);
  SM_Update(mockDT);
  assert(md.hasUpdated);
  TEST_Pass("Test_SM_Update_CallsValidUpdateFunctionEvenIfNullUpdate");
}

void Test_SM_Draw_CallsValidDrawFunctionEvenIfNullDraw(void) {
  SM_ChangeStateTo("testNULLDrawAndExit", NULL);
  SM_Draw();
  assert(md.hasDrawn);
  TEST_Pass("Test_SM_Draw_CallsValidDrawFunctionEvenIfNullDraw");
}

// --------------------------------------------------
// Shutdown
// --------------------------------------------------

void Test_SM_Shutdown_SucceedsPostInit(void) {
  assert(SM_Shutdown());
  TEST_Pass("Test_SM_Shutdown_SucceedsIfFirstCallPostInit");
}

// --------------------------------------------------
// Post-Shutdown
// --------------------------------------------------

// Shutdown -----------------------------------------

void Test_SM_Shutdown_FailsPostShutdown(void) {
  assert(!SM_Shutdown());
  TEST_Pass("Test_SM_Shutdown_FailsIfCalledTwicePostShutdown");
}

// IsInitialized ------------------------------------

void Test_SM_IsInitialized_FailsPostShutdown(void) {
  assert(!SM_IsInitialized());
  TEST_Pass("Test_SM_IsInitialized_FailsPostShutdown");
}

// State Functions ----------------------------------

void Test_SM_RegisterState_FailsPostShutdown(void) {
  assert(!SM_RegisterState("setToFail", mockEnter, mockUpdate, mockDraw,
    mockExit));
  TEST_Pass("Test_SM_RegisterState_FailsPostShutdown");
}

void Test_SM_ChangeStateTo_FailsPostShutdown(void) {
  assert(!SM_ChangeStateTo("testPreInit", NULL));
  TEST_Pass("Test_SM_ChangeStateTo_FailsPostShutdown");
}

void Test_SM_GetCurrStateName_FailsPostShutdown(void) {
  assert(!SM_GetCurrStateName());
  TEST_Pass("Test_SM_GetCurrStateName_FailsPostShutdown");
}

void Test_SM_ListStates_FailsPostShutdown(void) {
  // TODO
}

// Lifecycle Functions ------------------------------

void Test_SM_Update_FailsPostShutdown(void) {
  // TODO
}

void Test_SM_Draw_FailsPostShutdown(void) {
  // TODO
}

void Test_SM_Shutdown_CallsExitFunctionOfCurrentState(void) {
  // TODO
}

void Test_SM_Shutdown_SkipsExitIfNull(void) {
  // TODO
}

// --------------------------------------------------
// Stress tests
// --------------------------------------------------

void Test_SM_RegisteringMultipleStatesCausesNoSkips(void) {
  // SM_Init();
  // for (int i = 0; i < MULTIPLE_STATES; i++) {
  //   char buffer[16];
  //   snprintf(buffer, sizeof(buffer), "%d", i);
  //   SM_RegisterState(buffer, mockEnter, mockUpdate, mockDraw, mockExit);
  // }
  // assert(SM_Test_GetStateCount() == MULTIPLE_STATES);
  // printf("\t[PASS] Test_SM_RegisteringMultipleStatesCausesNoSkips: %d states "
  //        "registered\n",
  //        MULTIPLE_STATES);
}

void Test_SM_ChangingStatesOftenCausesNoSkips(void) {
  // md.enteredTimes = 0;
  // md.exitedTimes = 0;
  // for (int i = 0; i < MULTIPLE_STATES; i++) {
  //   char buffer[16];
  //   snprintf(buffer, sizeof(buffer), "%d", i);
  //   SM_ChangeStateTo(buffer, NULL);
  // }
  //
  // assert(md.enteredTimes == MULTIPLE_STATES &&
  //   md.exitedTimes == MULTIPLE_STATES - 1);
  //
  // printf(
  //   "\t[PASS] Test_SM_ChangingStatesOften_CausesNoSkips: %d state changes\n",
  //   MULTIPLE_STATES);
}

void Test_SM_Shutdown_FreeingMultipleStatesCausesNoSkips(void) {
  // SM_Shutdown();
  // assert(SM_Test_GetStateCount() == 0);
  // TEST_Pass("Test_SM_Shutdown_FreeingMultipleStatesCausesNoSkips");
}

// --------------------------------------------------
// Finger's crossed!
// --------------------------------------------------

int main() {
  puts("\nTESTING PRE-INIT");
  puts("• Init Related");
  Test_SM_IsInitialized_FailsPreInit();
  puts("• State Functions");
  Test_SM_RegisterState_FailsPreInit();
  Test_SM_ChangeStateTo_FailsPreInit();
  Test_SM_GetCurrStateName_FailsPreInit();
  Test_SM_ListStates_FailsPreInit();
  puts("• Lifecycle Functions");
  Test_SM_Update_FailsPreInit();
  Test_SM_Draw_FailsPreInit();
  puts("• Shutdown");
  Test_SM_Shutdown_FailsPreInit();

  puts("\nTESTING INIT");
  puts("• Mem Alloc Fail");
  Test_SM_Init_FailsIfCallocFails();
  puts("• Success");
  Test_SM_Init_Succeeds();

  puts("\nTESTING POST-INIT");
  puts("• Init Related");
  Test_SM_Init_FailsPostInit();
  Test_SM_IsInitialized_SucceedsPostInit();
  puts("• State Functions");
  puts(" • ListStates Pre-RegisterState");
  // TODO check for list, check if statecount == 0
  puts(" • RegisterState");

  // TODO review the below
  Test_SM_RegisterState_RejectsNullStateName();
  Test_SM_RegisterState_RejectsEmptyStateName();
  Test_SM_RegisterState_AcceptsAllNonNullArgs();
  Test_SM_RegisterState_RejectsExistingStateName();

  // TODO review the below
  Test_SM_RegisterState_AcceptsNullEnterAndUpdate(); // TODO Test more
  Test_SM_RegisterState_AcceptsNullDrawAndExit(); // TODO Test more

  Test_SM_RegisterState_RejectsAllNullArgs();
  puts("• IsStateRegistered");
  Test_SM_IsStateRegistered_AcceptsRegisteredStateName();
  Test_SM_IsStateRegistered_RejectsNonRegisteredStateName();
  puts(" • ListStates Post-RegisterState");
  // TODO check for list, check if statecount != 0
  puts(" • GetCurrStateName Pre-ChangeStateTo");
  Test_SM_GetCurrStateName_FailsPreRegisterState();
  puts(" • Lifecycle Functions Pre-ChangeStateTo");
  Test_SM_Update_FailsIfNullCurrentState();
  Test_SM_Draw_FailsIfNullCurrentState();
  puts(" • ChangeStateTo");
  Test_SM_ChangeStateTo_AcceptsValidStateFromNull();
  Test_SM_ChangeStateTo_RejectsNullStateName();
  Test_SM_ChangeStateTo_RejectsUnregisteredStateName();

  // TODO Think of how to name this better
  Test_SM_ChangeStateTo_SucceedsChangingFromOneValidStateToAnother();

  // TODO review the below
  Test_SM_ChangeStateTo_CallsExitFunctionOfCurrentState();
  Test_SM_ChangeStateTo_CallsEnterFunctionOfNewState();
  Test_SM_ChangeStateTo_CallsExitAndEnterIfChangingToSameState();
  Test_SM_ChangeStateTo_CallsEnterFunctionWithArgs();

  puts(" • GetCurrStateName Post-ChangeStateTo");
  Test_SM_GetCurrStateName_ReturnsCurrentStateName();

  puts("TESTING LIFICYCLE FUNCTIONS");
  Test_SM_Update_CallsValidUpdateFunction();
  Test_SM_Draw_CallsValidDrawFunction();
  Test_SM_Update_CallsValidUpdateFunctionEvenIfNullUpdate();
  Test_SM_Draw_CallsValidDrawFunctionEvenIfNullDraw();

  puts("\nTESTING SHUTDOWN");
  Test_SM_Shutdown_SucceedsPostInit();

  puts("\nTESTING POST-SHUTDOWN");
  puts("• Shutdown");
  Test_SM_Shutdown_FailsPostShutdown();
  puts("• Init Related");
  Test_SM_IsInitialized_FailsPostShutdown();
  puts("• State Functions");
  Test_SM_RegisterState_FailsPostShutdown();
  Test_SM_ChangeStateTo_FailsPostShutdown();
  Test_SM_GetCurrStateName_FailsPostShutdown();
  // Test_SM_ListStates_FailsPostShutdown();
  puts("• Lifecycle Functions");

  // TODO review the below
  Test_SM_Update_FailsPostShutdown();
  Test_SM_Draw_FailsPostShutdown();

  // TODO review the below
  puts("\nTESTING OTHER TESTS");
  Test_SM_Shutdown_CallsExitFunctionOfCurrentState();
  Test_SM_Shutdown_SkipsExitIfNull();

  puts("\nSTRESS TESTS");
  Test_SM_RegisteringMultipleStatesCausesNoSkips();
  Test_SM_ChangingStatesOftenCausesNoSkips();
  Test_SM_Shutdown_FreeingMultipleStatesCausesNoSkips();

  puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
  return 0;
}
