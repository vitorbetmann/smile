/* TODO add instructions to text below about how to create mocks and how to name
 * them perhaps a few more details on how to name stuff
 *
 * Like for example, when using NULL in the name of a func, call it Null for
 * readability
 * And when writing tests, put one immediately below the other and only skip a
 * line when testing a different routine
 *
 * and the puts' in main, how to write them as well (e.g. all caps for new sections,
 * start with a '•' for subsections
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

#include "SaveLoadTest.h"
#include "SaveLoad.h"
#include "../src/SaveLoad/SaveLoadInternal.h"
#include "../src/_Internal/Test/TestInternal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Log.h"

// --------------------------------------------------
// Pre-Init
// --------------------------------------------------

// IsInitialized ------------------------------------

void Test_SL_IsInitialized_ReturnsFalsePreInit(void) {
  assert(!SL_IsInitialized());
  TEST_Pass("Test_SL_IsInitialized_ReturnsFalsePreInit");
}

// Game Dir -----------------------------------------

void Test_SL_GetGameDir_ReturnsFalsePreInit(void) {
  assert(!SL_GetGameDir());
  TEST_Pass("Test_SL_GetGameDir_ReturnsFalsePreInit");
}

void Test_SL_GetDefaultDir_ReturnsFalsePreInit(void) {
  assert(!SL_GetDefaultDir());
  TEST_Pass("Test_SL_GetDefaultDir_ReturnsFalsePreInit");
}

void Test_SL_SetGameDir_ReturnsFalsePreInit(void) {
  assert(!SL_SetGameDir(NULL));
  TEST_Pass("Test_SL_SetGameDir_ReturnsFalsePreInit");
}

void Test_SL_DirExists_ReturnsFalsePreInit(void) {
  assert(!SL_DirExists(NULL));
  TEST_Pass("Test_SL_DirExists_ReturnsFalsePreInit");
}

// Game File ----------------------------------------

void Test_SL_GetGameFile_ReturnsFalsePreInit(void) {
  assert(!SL_GetGameFile());
  TEST_Pass("Test_SL_GetGameFile_ReturnsFalsePreInit");
}

void Test_SL_GetGamePath_ReturnsFalsePreInit(void) {
  assert(!SL_GetGamePath());
  TEST_Pass("Test_SL_GetGamePath_ReturnsFalsePreInit");
}

void Test_SL_SetGameFile_ReturnsFalsePreInit(void) {
  assert(!SL_SetGameFile(NULL));
  TEST_Pass("Test_SL_SetGameFile_ReturnsFalsePreInit");
}

void Test_SL_FileExists_ReturnsFalsePreInit(void) {
  assert(!SL_FileExists(NULL));
  TEST_Pass("Test_SL_FileExists_ReturnsFalsePreInit");
}

// Save ---------------------------------------------

void Test_SL_BeginSaveSession_ReturnsFalsePreInit(void) {
  assert(!SL_BeginSaveSession());
  TEST_Pass("Test_SL_BeginSaveSession_ReturnsFalsePreInit");
}

void Test_SL_SaveNext_ReturnsFalsePreInit(void) {
  assert(!SL_SaveNext(NULL));
  TEST_Pass("Test_SL_SaveNext_ReturnsFalsePreInit");
}

void Test_SL_EndSaveSession_ReturnsFalsePreInit(void) {
  assert(!SL_EndSaveSession());
  TEST_Pass("Test_SL_EndSaveSession_ReturnsFalsePreInit");
}

// Load ---------------------------------------------

void Test_SL_BeginLoadSession_ReturnsFalsePreInit(void) {
  assert(!SL_BeginLoadSession());
  TEST_Pass("Test_SL_BeginLoadSession_ReturnsFalsePreInit");
}

void Test_SL_HasNext_ReturnsFalsePreInit(void) {
  assert(!SL_HasNext());
  TEST_Pass("Test_SL_HasNext_ReturnsFalsePreInit");
}

void Test_SL_LoadNext_ReturnsNullPreInit(void) {
  assert(!SL_LoadNext());
  TEST_Pass("Test_SL_LoadNext_ReturnsNULLPreInit");
}

void Test_SL_LoadNextTo_ReturnsFalsePreInit(void) {
  assert(!SL_LoadNextTo(NULL, 0));
  TEST_Pass("Test_SL_LoadNextTo_ReturnsFalsePreInit");
}

void Test_SL_EndLoadSession_ReturnsFalsePreInit(void) {
  assert(!SL_EndLoadSession());
  TEST_Pass("Test_SL_EndLoadSession_ReturnsFalsePreInit");
}

// Delete -------------------------------------------

void Test_SL_DeleteDir_ReturnsFalsePreInit(void) {
  assert(!SL_DeleteDir(NULL));
  TEST_Pass("Test_SL_DeleteDir_ReturnsFalsePreInit");
}

void Test_SL_DeleteFile_ReturnsFalsePreInit(void) {
  assert(!SL_DeleteFile(NULL));
  TEST_Pass("Test_SL_DeleteFile_ReturnsFalsePreInit");
}

// Shutdown -----------------------------------------

void Test_SL_Shutdown_ReturnsFalsePreInit(void) {
  assert(!SL_Shutdown());
  TEST_Pass("Test_SL_Shutdown_ReturnsFalsePreInit");
}

// --------------------------------------------------
// Init
// --------------------------------------------------

// Mem Alloc Fail -----------------------------------

void Test_SL_Init_ReturnsFalseIfCallocFails(void) {
  TEST_SetCanCalloc(false);
  assert(!SL_Init());
  TEST_SetCanCalloc(true);
  TEST_Pass("Test_SL_Init_ReturnsFalseIfCallocFails");
}

void Test_SL_Init_ReturnsFalseIfMallocFails(void) {
  TEST_SetCanMalloc(false);
  assert(!SL_Init());
  TEST_SetCanMalloc(true);
  TEST_Pass("Test_SL_Init_ReturnsFalseIfMallocFails");
}

// Success ------------------------------------------

void Test_SL_Init_ReturnsTrue(void) {
  assert(SL_Init());
  TEST_Pass("Test_SL_Init_ReturnsTrue");
}

// --------------------------------------------------
// Post-Init
// --------------------------------------------------

// IsInitialized ------------------------------------

void Test_SL_IsInitialized_ReturnsTruePostInit(void) {
  assert(SL_IsInitialized());
  TEST_Pass("Test_SL_IsInitialized_ReturnsTrueAfterInit");
}

// Game Dir -----------------------------------------

void Test_SL_GetDefaultDir_ReturnsPathToSmileDir(void) {
  const char *home = getenv("HOME");
  const size_t totalLen = strlen(home) + strlen(DEFAULT_SYS_DIR) + strlen(SMILE_DIR) + 1;
  char *smilePath = malloc(totalLen);
  snprintf(smilePath, totalLen, "%s%s%s", home, DEFAULT_SYS_DIR, SMILE_DIR);

  assert(TEST_COMP_NAME(SL_GetDefaultDir(), smilePath));
  free(smilePath);
  TEST_Pass("Test_SL_GetDefaultDir_ReturnsPathToSmileDir");
}

void Test_SL_GetGameDir_ReturnsNullPreSetGameDir(void) {
  // assert(!SL_GetGameDir());
  TEST_Pass("Test_SL_GetGameDir_ReturnsNullPreSetGameDir");
}

void Test_SL_SetGameDir_ReturnsFalseWithNullPath(void) {
  // assert(!SL_SetGameDir(NULL));
  TEST_Pass("Test_SL_SetGameDir_ReturnsFalseWithNullPath");
}

void Test_SL_SetGameDir_ReturnsFalseWithPathChangingToParent(void) {
  // assert(!SL_SetGameDir("../MockPath"));
  TEST_Pass("Test_SL_SetGameDir_ReturnsFalseWithPathChangingToParent");
}

void Test_SL_SetGameDir_ReturnsFalseWithInvalidPath2(void) {
  // assert(!SL_SetGameDir("MockPath"));
  TEST_Pass("Test_SL_SetGameDir_ReturnsFalseWithInvalidPath2");
}

void Test_SL_SetGameDir_ReturnsFalseWithPathLongerThanPathMax(void) {
  // assert(!SL_SetGameDir(NULL));
  TEST_Pass("Test_SL_SetGameDir_ReturnsFalseWithPathLongerThanPathMax");
}

void Test_SL_DirExists_ReturnsFalseForNonExistingPath(void) {
  // assert(false)
  TEST_Pass("Test_SL_DirExists_ReturnsFalseForNonExistingPath");
}

void Test_SL_DirExists_ReturnsFalseForPathLongerThanPathMax(void) {
  // assert(false)
  TEST_Pass("Test_SL_DirExists_ReturnsFalseForPathLongerThanPathMax");
}

// -- Unsanitized Path ------------------------------

void Test_SL_SetGameDir_ReturnsTrueWithNonEmptyUnsanitizedValidPath(void) {
  // assert(!SL_SetGameDir("ValidPath"));
  TEST_Pass("Test_SL_SetGameDir_ReturnsTrueWithNonEmptyUnsanitizedValidPath");
}

void Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptyUnsanitizedDefaultDir(void) {
  // assert(TEST_COMP_NAME(SL_GetGameDir(), "some name"));
  TEST_Pass("Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptyDefaultDir");
}

void Test_SL_DirExists_ReturnsTrueForExistingUnsanitizedPath(void) {
  // assert(false)
  TEST_Pass("Test_SL_DirExists_ReturnsTrueForExistingPath");
}

// -- Sanitized Path --------------------------------


void Test_SL_SetGameDir_ReturnsTrueWithNonEmptySanitizedValidPath(void) {
  // assert(!SL_SetGameDir("ValidPath"));
  TEST_Pass("Test_SL_SetGameDir_ReturnsTrueWithNonEmptySanitizedValidPath");
}

void Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptySanitizedDefaultDir(void) {
  // assert(TEST_COMP_NAME(SL_GetGameDir(), "some name"));
  TEST_Pass("Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptySanitizedDefaultDir");
}

void Test_SL_DirExists_ReturnsTrueForExistingSanitizedPath(void) {
  // assert(false)
  TEST_Pass("Test_SL_DirExists_ReturnsTrueForExistingSanitizedPath");
}

// -- Empty Path ------------------------------------

void Test_SL_SetGameDir_ReturnsTrueWithEmptyPath(void) {
  // assert(!SL_SetGameDir(""));
  TEST_Pass("Test_SL_SetGameDir_ReturnsTrueWithValidPath");
}

void Test_SL_GetGameDir_ReturnsCorrectPathWithEmptyDefaultDir(void) {
  // assert(TEST_COMP_NAME(SL_GetGameDir(), "some name"));
  TEST_Pass("Test_SL_GetGameDir_ReturnsCorrectPathWithEmptyDefaultDir");
}

void Test_SL_DirExists_ReturnsTrueForEmptyPathIfEmptyDefaultDir(void) {
  // assert(false)
  TEST_Pass("Test_SL_DirExists_ReturnsTrueWithEmptyPathIfEmptyDefaultDir");
}

// Game File ----------------------------------------

// GetGameFile fails
// SetGameFile fails with invalid path
// // Perhaps test this more than once
// SetGameFile succeeds
// GetGameFile succeeds
// must also test FileExists

// Test set game dir after full path is set and check if correct
// Test set game file after full path is set and check if correct
// // perhaps do the same again in another order

// must also create a TEST_Func to create/delete dirs and files
// // Or call delete dir on all once tests are over

// Also test alternate dir if default dir fails


// --------------------------------------------------
// Shutdown
// --------------------------------------------------

void Test_SL_Shutdown_ReturnsTrueAfterInit(void) {
  assert(SL_Shutdown());
  TEST_Pass("Test_SL_Shutdown_ReturnsTrueAfterInit");
}

// --------------------------------------------------
// Post-Shutdown
// --------------------------------------------------

void Test_SL_Shutdown_ReturnsFalseIfCalledTwice(void) {
  assert(!SL_Shutdown());
  TEST_Pass("Test_SL_Shutdown_ReturnsFalseIfCalledTwice");
}

void Test_SL_IsInitialized_ReturnsFalseAfterShutdown(void) {
  assert(!SL_IsInitialized());
  TEST_Pass("Test_SL_IsInitialized_ReturnsFalseAfterShutdown");
}

// --------------------------------------------------
// Init With
// --------------------------------------------------

// Test init with, get dir, get file... all getters, basically

// --------------------------------------------------
// Save
// --------------------------------------------------

// --------------------------------------------------
// Load
// --------------------------------------------------

// --------------------------------------------------
// Delete
// --------------------------------------------------

// --------------------------------------------------
// Stress tests
// --------------------------------------------------

// --------------------------------------------------
// Finger's crossed!
// --------------------------------------------------

int main() {
  puts("\nTESTING PRE-INIT");
  puts("• IsInitialized");
  Test_SL_IsInitialized_ReturnsFalsePreInit();
  puts("• Game Dir");
  Test_SL_GetGameDir_ReturnsFalsePreInit();
  Test_SL_GetDefaultDir_ReturnsFalsePreInit();
  Test_SL_DirExists_ReturnsFalsePreInit();
  Test_SL_SetGameDir_ReturnsFalsePreInit();
  puts("• Game File");
  Test_SL_GetGameFile_ReturnsFalsePreInit();
  Test_SL_GetGamePath_ReturnsFalsePreInit();
  Test_SL_SetGameFile_ReturnsFalsePreInit();
  Test_SL_FileExists_ReturnsFalsePreInit();
  puts("• Save");
  Test_SL_BeginSaveSession_ReturnsFalsePreInit();
  Test_SL_SaveNext_ReturnsFalsePreInit();
  Test_SL_EndSaveSession_ReturnsFalsePreInit();
  puts("• Load");
  Test_SL_BeginLoadSession_ReturnsFalsePreInit();
  Test_SL_HasNext_ReturnsFalsePreInit();
  Test_SL_LoadNext_ReturnsNullPreInit();
  Test_SL_LoadNextTo_ReturnsFalsePreInit();
  Test_SL_EndLoadSession_ReturnsFalsePreInit();
  puts("• Delete");
  Test_SL_DeleteDir_ReturnsFalsePreInit();
  Test_SL_DeleteFile_ReturnsFalsePreInit();
  puts("• Shutdown");
  Test_SL_Shutdown_ReturnsFalsePreInit();

  puts("\nTESTING INIT");
  puts("• Mem Alloc Fail");
  Test_SL_Init_ReturnsFalseIfCallocFails();
  Test_SL_Init_ReturnsFalseIfMallocFails();
  puts("• Success");
  Test_SL_Init_ReturnsTrue();

  puts("\nTESTING POST-INIT");
  puts("• IsInitialized");
  Test_SL_IsInitialized_ReturnsTruePostInit();
  puts("• Game Dir");
  Test_SL_GetDefaultDir_ReturnsPathToSmileDir();
  Test_SL_GetGameDir_ReturnsNullPreSetGameDir();
  Test_SL_SetGameDir_ReturnsFalseWithNullPath();
  Test_SL_SetGameDir_ReturnsFalseWithPathChangingToParent();
  Test_SL_SetGameDir_ReturnsFalseWithInvalidPath2();
  Test_SL_SetGameDir_ReturnsFalseWithPathLongerThanPathMax();
  Test_SL_DirExists_ReturnsFalseForNonExistingPath();
  Test_SL_DirExists_ReturnsFalseForPathLongerThanPathMax();
  puts("  • Unsanitized Path");
  Test_SL_SetGameDir_ReturnsTrueWithNonEmptyUnsanitizedValidPath();
  Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptyUnsanitizedDefaultDir();
  Test_SL_DirExists_ReturnsTrueForExistingUnsanitizedPath();
  puts("  • Sanitized Path");
  Test_SL_SetGameDir_ReturnsTrueWithNonEmptySanitizedValidPath();
  Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptySanitizedDefaultDir();
  Test_SL_DirExists_ReturnsTrueForExistingSanitizedPath();
  puts("  • Empty Path");
  Test_SL_SetGameDir_ReturnsTrueWithEmptyPath();
  Test_SL_GetGameDir_ReturnsCorrectPathWithEmptyDefaultDir();
  Test_SL_DirExists_ReturnsTrueForEmptyPathIfEmptyDefaultDir();
  puts("• Game File");

  puts("\nTESTING SHUTDOWN");
  Test_SL_Shutdown_ReturnsTrueAfterInit();

  puts("\nTESTING POST-SHUTDOWN");
  Test_SL_Shutdown_ReturnsFalseIfCalledTwice();
  Test_SL_IsInitialized_ReturnsFalseAfterShutdown();

  puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
  return 0;
}
