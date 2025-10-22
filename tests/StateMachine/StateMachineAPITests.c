#include <assert.h>
#include <stdio.h>

#include "StateMachineAPITests.h"
#include "../include/StateMachine.h"
#include "../src/_Internal/Test/TestInternal.h"

// -----------------------------------------------------------------------------
// Base Tests
// -----------------------------------------------------------------------------

void Test_smStop_SucceedsPostStart(void) {
    assert(smStart());
    assert(smStop());
    tsInternalPass("Test_smStop_SucceedsPostStart");
}

// -----------------------------------------------------------------------------
// Pre-Start
// -----------------------------------------------------------------------------

// Start Related

void Test_smHasStarted_FailsPreStart(void) {
    assert(!smIsRunning());
    tsInternalPass("Test_smHasStarted_FailsPreStart");
}

// State Functions

void Test_smCreateState_FailsPreStart(void) {
    assert(!smCreateState(nullptr, nullptr, nullptr, nullptr, nullptr));
    tsInternalPass("Test_smCreateState_FailsPreStart");
}

void Test_smStateExists_FailsPreStart(void) {
    assert(!smStateExists(nullptr));
    tsInternalPass("Test_smStateExists_FailsPreStart");
}

void Test_smSetState_FailsPreStart(void) {
    assert(!smSetState(nullptr, nullptr));
    tsInternalPass("Test_smSetState_FailsPreStart");
}

void Test_smGetCurrentStateName_FailsPreStart(void) {
    assert(!smGetCurrentStateName());
    tsInternalPass("Test_smGetCurrentStateName_FailsPreStart");
}

void Test_smDeleteState_FailsPreStart(void) {
    assert(!smDeleteState(nullptr));
    tsInternalPass("Test_smDeleteState_FailsPreStart");
}

void Test_smGetStateCount_FailsPreStart(void) {
    assert(smGetStateCount() == -1);
    tsInternalPass("Test_smGetStateCount_FailsPreStart");
}


// Lifecycle Functions

void Test_smUpdate_FailsPreStart(void) {
    assert(!smUpdate(mockDt));
    tsInternalPass("Test_smUpdate_FailsPreStart");
}

void Test_smDraw_FailsPreStart(void) {
    assert(!smDraw());
    tsInternalPass("Test_smDraw_FailsPreStart");
}

// Stop

void Test_smStop_FailsPreStart(void) {
    assert(!smStop());
    tsInternalPass("Test_smStop_FailsPreStart");
}

// -----------------------------------------------------------------------------
// Start
// -----------------------------------------------------------------------------

// Mem Alloc Fail

void Test_smStart_FailsIfCallocFails(void) {
    tsInternalDisable(CALLOC, 1);
    assert(!smStart());
    tsInternalPass("Test_smStart_FailsIfCallocFails");
}

// -----------------------------------------------------------------------------
// Post-Start
// -----------------------------------------------------------------------------

// Start Related

void Test_smStart_IsIdempotentPostStart(void) {
    smStart();
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++) {
        assert(!smStart());
    }
    smStop();
    tsInternalPass("Test_smStart_IsIdempotentPostStart");
}

void Test_smHasStarted_SucceedsPostStart(void) {
    smStart();
    assert(smIsRunning());
    smStop();
    tsInternalPass("Test_smHasStarted_SucceedsPostStart");
}

// State Functions

// -- smCreateState

// -- -- Name Related

void Test_smCreateState_AcceptsValidStateName(void) {
    // TODO
}

void Test_smCreateState_RejectsExistingStateName(void) {
    // TODO
}

void Test_smCreateState_RejectsLongStateName(void) {
    // TODO
}

void Test_smCreateState_RejectsNullStateName(void) {
    smStart();
    assert(!smCreateState(nullptr, nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_RejectsNullStateName");
}

void Test_smCreateState_RejectsEmptyStateName(void) {
    smStart();
    assert(!smCreateState("", nullptr, nullptr, nullptr, nullptr));
    smStop();
    tsInternalPass("Test_smCreateState_RejectsEmptyStateName");
}

void Test_smCreateState_RejectsWhitespaceOnlyStateName(void) {
    // TODO
}

void Test_smCreateState_AcceptsLeadingWhitespaceOnStateName(void) {
    // TODO
}

void Test_smCreateState_AcceptsTrailingWhitespaceOnStateName(void) {
    // TODO
}

void Test_smCreateState_AcceptsLeadingAndTrailingWhitespaceOnStateName(void) {
    // TODO
}

void Test_smCreateState_TrimsLeadingWhitespaceOnStateName(void) {
    // TODO
}

void Test_smCreateState_TrimsTrailingWhitespaceOnStateName(void) {
    // TODO
}

void Test_smCreateState_TrimsLeadingAndTrailingWhitespaceOnStateName(void) {
    // TODO
}

// -- -- State Functions Related

void Test_smCreateState_AcceptsValidNameAndAllNonNullFunctions(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndEnterNullOnly(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndUpdateNullOnly(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndDrawNullOnly(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndExitNullOnly(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdate(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDraw(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullExit(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDraw(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullExit(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullDrawAndNullExit(void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullDraw(
    void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullUpdateAndNullExit(
    void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullEnterAndNullDrawAndNullExit(
    void) {
    // TODO
}

void Test_smCreateState_AcceptsValidNameAndNullUpdateAndNullDrawAndNullExit(
    void) {
    // TODO
}

void Test_smCreateState_RejectsValidNameAndAllNullFunctions(void) {
    // TODO
}

// smStateExists

void Test_smStateExists_AcceptsCreatedStateName(void) {
    // TODO
}

void Test_smStateExists_RejectsNonCreatedStateName(void) {
    // TODO
}

// -- smSetState

void Test_smSetState_AcceptsValidStateFromNull(void) {
    // TODO
}

void Test_smSetState_RejectsNullStateName(void) {
    // TODO
}

void Test_smSetState_RejectsUnCreatedStateName(void) {
    // TODO
}

void Test_smSetState_SucceedsChangingFromOneValidStateToAnother(void) {
    // TODO
}

void Test_smSetState_CallsExitFunctionOfCurrentState(void) {
    // TODO
}

void Test_smSetState_CallsEnterFunctionOfNewState(void) {
    // TODO
}

void Test_smSetState_CallsExitAndEnterIfChangingToSameState(void) {
    // TODO
}

void Test_smSetState_CallsEnterFunctionWithArgs(void) {
    // TODO
}

// -- smGetCurrentStateName

void Test_smGetCurrentStateName_FailsPreCreateState(void) {
    smStart();
    assert(!smGetCurrentStateName());
    smStop();
    tsInternalPass("Test_smGetCurrentStateName_FailsPreCreateState");
}

void Test_smGetCurrentStateName_ReturnsCurrentStateName(void) {
    // TODO
}

// -- smDeleteState

void Test_smDeleteState_FailsPreCreateState(void) {
    // TODO
}

// -- smGetStateCount

void Test_smGetStateCount_ReturnsZeroPostStart(void) {
    smStart();
    assert(smGetStateCount() == 0);
    smStop();
    tsInternalPass("Test_smGetStateCount_ReturnsZeroPostStart");
}

void Test_smGetStateCount_ReturnsCorrectStateCountPostCreateState(void) {
    // TODO
}

void Test_smGetStateCount_ReturnsCorrectStateCountPostDeleteState(void) {
    // TODO
}

// Lifecycle Functions

// -- smUpdate

void Test_smUpdate_FailsIfNullCurrentState(void) {
    smStart();
    assert(!smGetCurrentStateName());
    assert(!smUpdate(mockDt));
    smStop();
    tsInternalPass("Test_smUpdate_FailsIfNullCurrentState");
}

void Test_smUpdate_CallsValidUpdateFunction(void) {
    // TODO
}

void Test_smUpdate_CallsValidUpdateFunctionEvenIfNullUpdate(void) {
    // TODO
}

// -- smDraw

void Test_smDraw_FailsIfNullCurrentState(void) {
    smStart();
    assert(!smGetCurrentStateName());
    assert(!smDraw());
    smStop();
    tsInternalPass("Test_smDraw_FailsIfNullCurrentState");
}

void Test_smDraw_CallsValidDrawFunction(void) {
    // TODO
}

void Test_smDraw_CallsValidDrawFunctionEvenIfNullDraw(void) {
    // TODO
}

// Stop Related

void Test_smStop_CallsExitFunctionOfCurrentState(void) {
    // TODO
}

void Test_smStop_SkipsExitIfNull(void) {
    // TODO
}

// -----------------------------------------------------------------------------
// Post-Stop
// -----------------------------------------------------------------------------

// Start Related

void Test_smHasStarted_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smIsRunning());
    tsInternalPass("Test_smHasStarted_FailsPostStop");
}

// State Functions

void Test_smCreateState_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smCreateState(nullptr, nullptr, nullptr,nullptr,nullptr));
    tsInternalPass("Test_smCreateState_FailsPostStop");
}

void Test_smStateExists_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smStateExists(nullptr));
    tsInternalPass("Test_smStateExists_FailsPostStop");
}

void Test_smSetState_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smSetState(nullptr, nullptr));
    tsInternalPass("Test_smSetState_FailsPostStop");
}

void Test_smGetCurrentStateName_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smGetCurrentStateName());
    tsInternalPass("Test_smGetCurrentStateName_FailsPostStop");
}

void Test_smDeleteState_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smDeleteState(nullptr));
    tsInternalPass("Test_smDeleteState_FailsPostStop");
}

void Test_smGetStateCount_FailsPostStop(void) {
    smStart();
    smStop();
    assert(smGetStateCount() == -1);
    tsInternalPass("Test_smGetStateCount_FailsPostStop");
}

// Lifecycle Functions

void Test_smUpdate_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smUpdate(mockDt));
    tsInternalPass("Test_smUpdate_FailsPostStop");
}

void Test_smDraw_FailsPostStop(void) {
    smStart();
    smStop();
    assert(!smDraw());
    tsInternalPass("Test_smDraw_FailsPostStop");
}

// Stop Related

void Test_smStop_IsIdempotentPostStop(void) {
    smStart();
    smStop();
    for (int i = 0; i < IDEMPOTENT_ITERATIONS; i++) {
        assert(!smStop());
    }
    tsInternalPass("Test_smStop_IsIdempotentPostStop");
}

// --------------------------------------------------
// Stress tests
// --------------------------------------------------

// State Functions

void TestStress_smCreateState_CreatingMultipleStatesCausesNoSkips(void) {
    // TODO
}

void TestStress_smSetState_SettingStatesOftenCausesNoSkips(void) {
    // TODO
}

// Stop Related

void TestStress_smStop_FreeingMultipleStatesCausesNoSkips(void) {
    // TODO
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main() {
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
    Test_smDraw_FailsPreStart();
    puts("• Stop Related");
    Test_smStop_FailsPreStart();

    puts("\nSTART TESTING");
    puts("• Mem Alloc Fail");
    Test_smStart_FailsIfCallocFails();

    puts("\nPOST-START TESTING");
    puts("• Start Related");
    Test_smStart_IsIdempotentPostStart();
    Test_smHasStarted_SucceedsPostStart();
    puts("• State Functions");
    puts(" • smCreateState");
    puts("  • Name Related");
    Test_smCreateState_AcceptsValidStateName();
    Test_smCreateState_RejectsExistingStateName();
    Test_smCreateState_RejectsLongStateName();
    Test_smCreateState_RejectsNullStateName();
    Test_smCreateState_RejectsEmptyStateName();

    // TODO create more tests for the different types of whitespaces
    // Test_smCreateState_RejectsWhitespaceOnlyStateName();
    //
    // Test_smCreateState_AcceptsLeadingWhitespaceOnStateName();
    // Test_smCreateState_AcceptsTrailingWhitespaceOnStateName();
    // Test_smCreateState_AcceptsLeadingAndTrailingWhitespaceOnStateName();
    // Test_smCreateState_TrimsLeadingWhitespaceOnStateName();
    // Test_smCreateState_TrimsTrailingWhitespaceOnStateName();
    // Test_smCreateState_TrimsLeadingAndTrailingWhitespaceOnStateName();
    puts("  • State Functions Related");
    Test_smCreateState_AcceptsValidNameAndAllNonNullFunctions();
    Test_smCreateState_AcceptsValidNameAndEnterNullOnly();
    Test_smCreateState_AcceptsValidNameAndUpdateNullOnly();
    Test_smCreateState_AcceptsValidNameAndDrawNullOnly();
    Test_smCreateState_AcceptsValidNameAndExitNullOnly();
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
    Test_smStateExists_AcceptsCreatedStateName();
    Test_smStateExists_RejectsNonCreatedStateName();
    puts(" • smSetState");
    Test_smSetState_AcceptsValidStateFromNull();
    Test_smSetState_RejectsNullStateName();
    Test_smSetState_RejectsUnCreatedStateName();
    Test_smSetState_SucceedsChangingFromOneValidStateToAnother();
    Test_smSetState_CallsExitFunctionOfCurrentState();
    Test_smSetState_CallsEnterFunctionOfNewState();
    Test_smSetState_CallsExitAndEnterIfChangingToSameState();
    Test_smSetState_CallsEnterFunctionWithArgs();
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
    Test_smUpdate_FailsIfNullCurrentState();
    Test_smUpdate_CallsValidUpdateFunction();
    Test_smUpdate_CallsValidUpdateFunctionEvenIfNullUpdate();
    // TODO test with negative dt?
    puts(" • smDraw");
    Test_smDraw_FailsIfNullCurrentState();
    Test_smDraw_CallsValidDrawFunction();
    Test_smDraw_CallsValidDrawFunctionEvenIfNullDraw();

    puts("\nSTOP TESTING");
    Test_smStop_CallsExitFunctionOfCurrentState();
    Test_smStop_SkipsExitIfNull();
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
