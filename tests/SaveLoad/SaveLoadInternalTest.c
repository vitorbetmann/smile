/* TODO add instructions to text below about how to create mocks and how to name
 * them perhaps a few more details on how to name stuff
 *
 * Like for example, when using NULL in the name of a func, call it Null for
 * readability
 * And when writing tests, put one immediately below the other and only skip a
 * line when testing a different routine
 */

/*
 * Test Naming Convention:
 *
 * Each test function is named following the pattern:
 *
 *   Test_<FunctionName>_<ExpectedBehavior>
 *
 * where:
 *   - Test_SL_: prefix indicating this is a SaveLoad test.
 *   - <FunctionName>: the name of the SaveLoad function or feature under
 * test.
 *   - <ExpectedBehavior>: a short description of what the test checks,
 *                        often stating the expected outcome or condition.
 *
 * Example:
 *   Test_SL_Init_ReturnsFalseIfMallocFails
 *
 * @author Vitor Betmann
 */

#include "SaveLoad.h"
#include "../src/SaveLoad/SaveLoadInternal.h"
#include "../src/SaveLoad/SaveLoadMessages.h"
#include "../src/_Internals/Test/TestInternal.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include "SaveLoadInternalTest.h"

// --------------------------------------------------
// Pre-Init - Internal
// --------------------------------------------------

void Test_SL_Internal_GetDefaultSysDir_ReturnsNullPreInit(void) {
    assert(!SL_Internal_GetDefaultSysDir());
    TEST_PASS("Test_SL_Internal_GetDefaultSysDir_ReturnsNULLPreInit");
}

void Test_SL_Internal_BeginSession_OnSaveModeReturnsFalsePreInit(void) {
    assert(!SL_Internal_BeginSession(WRITE, NULL,"hi"));
    TEST_PASS("Test_SL_Internal_BeginSession_OnSaveModeReturnsFalsePreInit");
}

void Test_SL_Internal_BeginSession_OnLoadModeReturnsFalsePreInit(void) {
    assert(!SL_Internal_BeginSession(LOAD, NULL,"hi"));
    TEST_PASS("Test_SL_Internal_BeginSession_OnLoadModeReturnsFalsePreInit");
}

// --------------------------------------------------
// Finger's crossed!
// --------------------------------------------------

int main() {
    puts("");

    puts("");

    return 0;
}
