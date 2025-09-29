/*
 * TestInternal Naming Convention:
 *
 * Each test function is named following the pattern:
 *
 *   Test_<FunctionName>_<ExpectedBehavior>
 *
 * where:
 *   - Test_sm: prefix indicating this is a StateMachine test.
 *   - <FunctionName>: the name of the StateMachine function or feature under
 * test.
 *   - <ExpectedBehavior>: a short description of what the test checks,
 *                        often stating the expected outcome or condition.
 *
 * Example:
 *   Test_smStart_FailsIfMallocFails
 *
 * @author Vitor Betmann
 */

#include <assert.h>
#include <stdio.h>

#include "StateMachineTest.h"
#include "../../include/StateMachine.h"
#include "../../src/_Internal/TestInternal/TestInternal.h"

// --------------------------------------------------
// Pre-Start
// --------------------------------------------------

// Start Related -------------------------------------

void Test_smHasStarted_FailsPreStart(void) {
    assert(!smHasStarted());
    tsInternalPass("Test_smHasStarted_FailsPreStart");
}

// State Functions ----------------------------------

void Test_smRegisterState_FailsPreStart(void) {
    assert(
        !smCreateState("setToFail", mockEnter, mockUpdate, mockDraw, mockExit));
    tsInternalPass("Test_smRegisterState_FailsPreStart");
}

void Test_smSetState_FailsPreStart(void) {
    assert(!smSetState("testPreStart", NULL));
    tsInternalPass("Test_smSetState_FailsPreStart");
}

void Test_smGetCurrentStateName_FailsPreStart(void) {
    assert(!smGetCurrentStateName());
    tsInternalPass("Test_smGetCurrentStateName_FailsPreStart");
}

void Test_smGetStateCount_FailsPreStart(void) {
    assert(smGetStateCount() == -1);
    tsInternalPass("Test_smGetStateCount_FailsPreStart");
}

// Lifecycle Functions ------------------------------

void Test_smUpdate_FailsPreStart(void) {
    assert(!smUpdate(mockDT));
    tsInternalPass("Test_smUpdate_FailsPreStart");
}

void Test_smDraw_FailsPreStart(void) {
    assert(!smDraw());
    tsInternalPass("Test_smDraw_FailsPreStart");
}

// Stop -----------------------------------------

void Test_smStop_FailsPreStart(void) {
    assert(!smStop());
    tsInternalPass("Test_smStop_FailsPreStart");
}

// --------------------------------------------------
// Start
// --------------------------------------------------

// Mem Alloc Fail -----------------------------------

void Test_smStart_FailsIfCallocFails(void) {
    tsInternalDisable(CALLOC, 1);
    assert(!smStart());
    tsInternalPass("Test_smStart_FailsIfCallocFails");
}

// Success ------------------------------------------

void Test_smStart_Succeeds(void) {
    assert(smStart());
    tsInternalPass("Test_smStart_Succeeds");
}

// --------------------------------------------------
// Post-Start
// --------------------------------------------------

// Start ---------------------------------------------

void Test_smStart_FailsPostStart(void) {
    assert(!smStart());
    tsInternalPass("Test_smStart_FailsPostStart");
}

// HasStarted ------------------------------------

void Test_smHasStarted_SucceedsPostStart(void) {
    assert(smHasStarted());
    tsInternalPass("Test_smHasStarted_SucceedsPostStart");
}

// State Functions ----------------------------------

// -- GetStateCount Pre-RegisterState ---------------

void Test_smGetStateCount_ReturnsZeroPostStart(void) {
    assert(smGetStateCount() == 0);
    tsInternalPass("Test_smGetStateCount_ReturnsZeroPostStart");
}

// -- RegisterState ---------------------------------

void Test_smRegisterState_RejectsNullStateName(void) {
    assert(!smCreateState(NULL, NULL, NULL, NULL, NULL));
    tsInternalPass("Test_smRegisterState_RejectsNullStateName");
}

void Test_smRegisterState_RejectsEmptyStateName(void) {
    assert(!smCreateState("", NULL, NULL, NULL, NULL));
    tsInternalPass("Test_smRegisterState_RejectsEmptyStateName");
}

void Test_smRegisterState_AcceptsAllNonNullArgs(void) {
    assert(
        smCreateState("testNoNULL", mockEnter, mockUpdate, mockDraw, mockExit));
    tsInternalPass("Test_smRegisterState_AcceptsAllNonNullArgs");
}

void Test_smRegisterState_RejectsExistingStateName(void) {
    assert(!smCreateState("testNoNULL", mockEnter, NULL, NULL, NULL));
    tsInternalPass("Test_smRegisterState_RejectsExistingStateName");
}

void Test_smRegisterState_AcceptsNullEnterAndUpdate(void) {
    assert(
        smCreateState("testNULLEnterAndUpdate", NULL, NULL, mockDraw,
            mockExit));
    tsInternalPass("Test_smRegisterState_AcceptsNullEnterAndUpdate");
}

void Test_smRegisterState_AcceptsNullDrawAndExit(void) {
    assert(
        smCreateState("testNULLDrawAndExit", mockEnter, mockUpdate, NULL,
            NULL));
    tsInternalPass("Test_smRegisterState_AcceptsNullDrawAndExit");
}

void Test_smRegisterState_RejectsAllNullArgs(void) {
    assert(!smCreateState("testAllNULL", NULL, NULL, NULL, NULL));
    tsInternalPass("Test_smRegisterState_RejectsAllNullArgs");
}

// StateExists --------------------------------

void Test_smStateExists_AcceptsRegisteredStateName(void) {
    assert(smStateExists("testNoNULL"));
    tsInternalPass("Test_smStateExists_AcceptsRegisteredStateName");
}

void Test_smStateExists_RejectsNonRegisteredStateName(void) {
    assert(!smStateExists("testUnregistered"));
    tsInternalPass("Test_smStateExists_RejectsNonRegisteredStateName");
}

// -- GetStateCount Post-RegisterState --------------

void Test_smGetStateCount_ReturnsCorrectStateCountPostRegisterState(void) {
    assert(smGetStateCount() == 3);
    tsInternalPass(
        "Test_smGetStateCount_ReturnsCorrectStateCountPostRegisterState");
}

// -- GetCurrentStateName Pre-SetState ------------

void Test_smGetCurrentStateName_FailsPreRegisterState(void) {
    assert(!smGetCurrentStateName());
    tsInternalPass("Test_smGetCurrentStateName_FailsPreRegisterState");
}

// -- Lifecycle Functions Pre-SetState ---------

void Test_smUpdate_FailsIfNullCurrentState(void) {
    assert(!smUpdate(mockDT));
    tsInternalPass("Test_smUpdate_FailsIfNullCurrentState");
}

void Test_smDraw_FailsIfNullCurrentState(void) {
    assert(!smDraw());
    tsInternalPass("Test_smDraw_FailsIfNullCurrentState");
}

// SetState ------------------------------------

void Test_smSetState_AcceptsValidStateFromNull(void) {
    assert(smSetState("testNoNULL", NULL));
    puts(
        "\t[PASS] Test_smSetState_SucceedsChangingFromNullToValidState");
}

void Test_smSetState_RejectsNullStateName(void) {
    assert(!smSetState(NULL, NULL));
    tsInternalPass("Test_smSetState_FailsIfNullName");
}

void Test_smSetState_RejectsUnregisteredStateName(void) {
    assert(!smSetState("testUnregistered", NULL));
    tsInternalPass("Test_smSetState_FailsForUnregisteredState");
}

void Test_smSetState_SucceedsChangingFromOneValidStateToAnother(void) {
    assert(smSetState("testNULLEnterAndUpdate", NULL));
    tsInternalPass(
        "Test_smSetState_SucceedsChangingFromOneValidStateToAnother");
}

void Test_smSetState_CallsExitFunctionOfCurrentState(void) {
    md.hasExited = false;
    smSetState("testNULLDrawAndExit", NULL);
    assert(md.hasExited);
    tsInternalPass("Test_smSetState_CallsExitFunctionOfCurrentState");
}

void Test_smSetState_CallsEnterFunctionOfNewState(void) {
    md.hasEntered = false;
    smSetState("testNoNULL", NULL);
    assert(md.hasEntered);
    tsInternalPass("Test_smSetState_CallsEnterFunctionOfNewState");
}

void Test_smSetState_CallsExitAndEnterIfChangingToSameState(void) {
    md.hasEntered = false;
    md.hasExited = false;
    smSetState("testNoNULL", NULL);
    assert(md.hasEntered && md.hasExited);
    tsInternalPass("Test_smSetState_CallsExitAndEnterIfChangingToSameState");
}

void Test_smSetState_CallsEnterFunctionWithArgs(void) {
    MockStateArgs temp = {.flag = true};
    smSetState("testNoNULL", &temp);
    assert(md.hasEnteredArgs);
    tsInternalPass("Test_smSetState_CallsEnterFunctionWithArgs");
}

// -- GetCurrentStateName Post-SetState -----------

void Test_smGetCurrentStateName_ReturnsCurrentStateName(void) {
    assert(strcmp(smGetCurrentStateName(), "testNoNULL") == 0);
    tsInternalPass("Test_smGetCurrentStateName_ReturnsCurrentStateName");
}

// --------------------------------------------------
// Lifecycle Functions
// --------------------------------------------------

void Test_smUpdate_CallsValidUpdateFunction(void) {
    smUpdate(mockDT);
    assert(md.hasUpdated);
    tsInternalPass("Test_smUpdate_CallsValidUpdateFunction");
}

void Test_smDraw_CallsValidDrawFunction(void) {
    smDraw();
    assert(md.hasDrawn);
    tsInternalPass("Test_smDraw_CallsValidDrawFunction");
}

void Test_smUpdate_CallsValidUpdateFunctionEvenIfNullUpdate(void) {
    smSetState("testNULLEnterAndUpdate", NULL);
    smUpdate(mockDT);
    assert(md.hasUpdated);
    tsInternalPass("Test_smUpdate_CallsValidUpdateFunctionEvenIfNullUpdate");
}

void Test_smDraw_CallsValidDrawFunctionEvenIfNullDraw(void) {
    smSetState("testNULLDrawAndExit", NULL);
    smDraw();
    assert(md.hasDrawn);
    tsInternalPass("Test_smDraw_CallsValidDrawFunctionEvenIfNullDraw");
}

// --------------------------------------------------
// Stop
// --------------------------------------------------

void Test_smStop_SucceedsPostStart(void) {
    assert(smStop());
    tsInternalPass("Test_smStop_SucceedsIfFirstCallPostStart");
}

// --------------------------------------------------
// Post-Stop
// --------------------------------------------------

// Stop -----------------------------------------

void Test_smStop_FailsPostStop(void) {
    assert(!smStop());
    tsInternalPass("Test_smStop_FailsIfCalledTwicePostStop");
}

// HasStarted ------------------------------------

void Test_smHasStarted_FailsPostStop(void) {
    assert(!smHasStarted());
    tsInternalPass("Test_smHasStarted_FailsPostStop");
}

// State Functions ----------------------------------

void Test_smRegisterState_FailsPostStop(void) {
    assert(!smCreateState("setToFail", mockEnter, mockUpdate, mockDraw,
        mockExit));
    tsInternalPass("Test_smRegisterState_FailsPostStop");
}

void Test_smSetState_FailsPostStop(void) {
    assert(!smSetState("testPreStart", NULL));
    tsInternalPass("Test_smSetState_FailsPostStop");
}

void Test_smGetCurrentStateName_FailsPostStop(void) {
    assert(!smGetCurrentStateName());
    tsInternalPass("Test_smGetCurrentStateName_FailsPostStop");
}

void Test_smGetStateCount_FailsPostStop(void) {
    // TODO
}

// Lifecycle Functions ------------------------------

void Test_smUpdate_FailsPostStop(void) {
    // TODO
}

void Test_smDraw_FailsPostStop(void) {
    // TODO
}

void Test_smStop_CallsExitFunctionOfCurrentState(void) {
    // TODO
}

void Test_smStop_SkipsExitIfNull(void) {
    // TODO
}

// --------------------------------------------------
// Stress tests
// --------------------------------------------------

void Test_smRegisteringMultipleStatesCausesNoSkips(void) {
    // smStart();
    // for (int i = 0; i < MULTIPLE_STATES; i++) {
    //   char buffer[16];
    //   snprintf(buffer, sizeof(buffer), "%d", i);
    //   smCreateState(buffer, mockEnter, mockUpdate, mockDraw, mockExit);
    // }
    // assert(smTest_GetStateCount() == MULTIPLE_STATES);
    // printf("\t[PASS] Test_smRegisteringMultipleStatesCausesNoSkips: %d states "
    //        "registered\n",
    //        MULTIPLE_STATES);
}

void Test_smChangingStatesOftenCausesNoSkips(void) {
    // md.enteredTimes = 0;
    // md.exitedTimes = 0;
    // for (int i = 0; i < MULTIPLE_STATES; i++) {
    //   char buffer[16];
    //   snprintf(buffer, sizeof(buffer), "%d", i);
    //   smSetState(buffer, NULL);
    // }
    //
    // assert(md.enteredTimes == MULTIPLE_STATES &&
    //   md.exitedTimes == MULTIPLE_STATES - 1);
    //
    // printf(
    //   "\t[PASS] Test_smChangingStatesOften_CausesNoSkips: %d state changes\n",
    //   MULTIPLE_STATES);
}

void Test_smStop_FreeingMultipleStatesCausesNoSkips(void) {
    // smStop();
    // assert(smTest_GetStateCount() == 0);
    // TEST_Pass("Test_smStop_FreeingMultipleStatesCausesNoSkips");
}

// --------------------------------------------------
// Finger's crossed!
// --------------------------------------------------

int main() {
    puts("\nTESTING PRE-Start");
    puts("• Start Related");
    Test_smHasStarted_FailsPreStart();
    puts("• State Functions");
    Test_smRegisterState_FailsPreStart();
    Test_smSetState_FailsPreStart();
    Test_smGetCurrentStateName_FailsPreStart();

    // TODO Think of how to name this better
    Test_smGetStateCount_FailsPreStart();

    puts("• Lifecycle Functions");
    Test_smUpdate_FailsPreStart();
    Test_smDraw_FailsPreStart();
    puts("• Stop");
    Test_smStop_FailsPreStart();

    puts("\nTESTING Start");
    puts("• Mem Alloc Fail");
    Test_smStart_FailsIfCallocFails();
    puts("• Success");
    Test_smStart_Succeeds();

    puts("\nTESTING POST-Start");
    puts("• Start Related");
    Test_smStart_FailsPostStart();
    Test_smHasStarted_SucceedsPostStart();
    puts("• State Functions");
    puts(" • GetStateCount Pre-RegisterState");

    // TODO Think of how to name this better
    Test_smGetStateCount_ReturnsZeroPostStart();
    puts(" • RegisterState");

    // TODO review the below
    Test_smRegisterState_RejectsNullStateName();
    Test_smRegisterState_RejectsEmptyStateName();
    Test_smRegisterState_AcceptsAllNonNullArgs();
    Test_smRegisterState_RejectsExistingStateName();

    // TODO review the below
    Test_smRegisterState_AcceptsNullEnterAndUpdate(); // TODO TestInternal more
    Test_smRegisterState_AcceptsNullDrawAndExit(); // TODO TestInternal more

    Test_smRegisterState_RejectsAllNullArgs();
    puts("• StateExists");
    Test_smStateExists_AcceptsRegisteredStateName();
    Test_smStateExists_RejectsNonRegisteredStateName();
    puts(" • GetStateCount Post-RegisterState");
    Test_smGetStateCount_ReturnsCorrectStateCountPostRegisterState();
    puts(" • GetCurrentStateName Pre-SetState");
    Test_smGetCurrentStateName_FailsPreRegisterState();
    puts(" • Lifecycle Functions Pre-SetState");
    Test_smUpdate_FailsIfNullCurrentState();
    Test_smDraw_FailsIfNullCurrentState();
    puts(" • SetState");
    Test_smSetState_AcceptsValidStateFromNull();
    Test_smSetState_RejectsNullStateName();
    Test_smSetState_RejectsUnregisteredStateName();

    // TODO Think of how to name this better
    Test_smSetState_SucceedsChangingFromOneValidStateToAnother();

    // TODO review the below
    Test_smSetState_CallsExitFunctionOfCurrentState();
    Test_smSetState_CallsEnterFunctionOfNewState();
    Test_smSetState_CallsExitAndEnterIfChangingToSameState();
    Test_smSetState_CallsEnterFunctionWithArgs();

    puts(" • GetCurrentStateName Post-SetState");
    Test_smGetCurrentStateName_ReturnsCurrentStateName();

    puts("TESTING LIFICYCLE FUNCTIONS");
    Test_smUpdate_CallsValidUpdateFunction();
    Test_smDraw_CallsValidDrawFunction();
    Test_smUpdate_CallsValidUpdateFunctionEvenIfNullUpdate();
    Test_smDraw_CallsValidDrawFunctionEvenIfNullDraw();

    puts("\nTESTING Stop");
    Test_smStop_SucceedsPostStart();

    puts("\nTESTING POST-Stop");
    puts("• Stop");
    Test_smStop_FailsPostStop();
    puts("• Start Related");
    Test_smHasStarted_FailsPostStop();
    puts("• State Functions");
    Test_smRegisterState_FailsPostStop();
    Test_smSetState_FailsPostStop();
    Test_smGetCurrentStateName_FailsPostStop();
    Test_smGetStateCount_FailsPostStop();
    puts("• Lifecycle Functions");

    // TODO review the below
    Test_smUpdate_FailsPostStop();
    Test_smDraw_FailsPostStop();

    // TODO review the below
    puts("\nTESTING OTHER TESTS");
    Test_smStop_CallsExitFunctionOfCurrentState();
    Test_smStop_SkipsExitIfNull();

    puts("\nSTRESS TESTS");
    Test_smRegisteringMultipleStatesCausesNoSkips();
    Test_smChangingStatesOftenCausesNoSkips();
    Test_smStop_FreeingMultipleStatesCausesNoSkips();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
    return 0;
}
