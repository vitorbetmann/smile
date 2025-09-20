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

#include "SaveLoad.h"
#include "../src/SaveLoad/SaveLoadInternal.h"
#include "../src/_Internals/Test/TestInternal.h"
#include "SaveLoadTest.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------
// Pre-Init
// --------------------------------------------------

// Init ---------------------------------------------

void Test_SL_IsInitialized_ReturnsFalsePreInit(void) {
  assert(!SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsFalsePreInit");
}

// Game Dir -----------------------------------------

void Test_SL_GetGameDir_ReturnsFalsePreInit(void) {
  assert(!SL_GetGameDir());
  TEST_PASS("Test_SL_GetGameDir_ReturnsFalsePreInit");
}

void Test_SL_GetDefaultDir_ReturnsFalsePreInit(void) {
  assert(!SL_GetDefaultDir());
  TEST_PASS("Test_SL_GetDefaultDir_ReturnsFalsePreInit");
}

void Test_SL_SetGameDir_ReturnsFalsePreInit(void) {
  assert(!SL_SetGameDir(NULL));
  TEST_PASS("Test_SL_SetGameDir_ReturnsFalsePreInit");
}

void Test_SL_DirExists_ReturnsFalsePreInit(void) {
  assert(!SL_DirExists(NULL));
  TEST_PASS("Test_SL_DirExists_ReturnsFalsePreInit");
}

// Game File ----------------------------------------

void Test_SL_GetGameFile_ReturnsFalsePreInit(void) {
  assert(!SL_GetGameFile());
  TEST_PASS("Test_SL_GetGameFile_ReturnsFalsePreInit");
}

void Test_SL_GetGamePath_ReturnsFalsePreInit(void) {
  assert(!SL_GetGamePath());
  TEST_PASS("Test_SL_GetGamePath_ReturnsFalsePreInit");
}

void Test_SL_SetGameFile_ReturnsFalsePreInit(void) {
  assert(!SL_SetGameFile(NULL));
  TEST_PASS("Test_SL_SetGameFile_ReturnsFalsePreInit");
}

void Test_SL_FileExists_ReturnsFalsePreInit(void) {
  assert(!SL_FileExists(NULL));
  TEST_PASS("Test_SL_FileExists_ReturnsFalsePreInit");
}

// Save ---------------------------------------------

void Test_SL_BeginSaveSession_ReturnsFalsePreInit(void) {
  assert(!SL_BeginSaveSession());
  TEST_PASS("Test_SL_BeginSaveSession_ReturnsFalsePreInit");
}

void Test_SL_SaveNext_ReturnsFalsePreInit(void) {
  assert(!SL_SaveNext(NULL));
  TEST_PASS("Test_SL_SaveNext_ReturnsFalsePreInit");
}

void Test_SL_EndSaveSession_ReturnsFalsePreInit(void) {
  assert(!SL_EndSaveSession());
  TEST_PASS("Test_SL_EndSaveSession_ReturnsFalsePreInit");
}

// Load ---------------------------------------------

void Test_SL_BeginLoadSession_ReturnsFalsePreInit(void) {
  assert(!SL_BeginLoadSession());
  TEST_PASS("Test_SL_BeginLoadSession_ReturnsFalsePreInit");
}

void Test_SL_HasNext_ReturnsFalsePreInit(void) {
  assert(!SL_HasNext());
  TEST_PASS("Test_SL_HasNext_ReturnsFalsePreInit");
}

void Test_SL_LoadNext_ReturnsNullPreInit(void) {
  assert(!SL_LoadNext());
  TEST_PASS("Test_SL_LoadNext_ReturnsNULLPreInit");
}

void Test_SL_LoadNextTo_ReturnsFalsePreInit(void) {
  assert(!SL_LoadNextTo(NULL, 0));
  TEST_PASS("Test_SL_LoadNextTo_ReturnsFalsePreInit");
}

void Test_SL_EndLoadSession_ReturnsFalsePreInit(void) {
  assert(!SL_EndLoadSession());
  TEST_PASS("Test_SL_EndLoadSession_ReturnsFalsePreInit");
}

// Delete -------------------------------------------

void Test_SL_DeleteDir_ReturnsFalsePreInit(void) {
  assert(!SL_DeleteDir(NULL));
  TEST_PASS("Test_SL_DeleteDir_ReturnsFalsePreInit");
}

void Test_SL_DeleteFile_ReturnsFalsePreInit(void) {
  assert(!SL_DeleteFile(NULL));
  TEST_PASS("Test_SL_DeleteFile_ReturnsFalsePreInit");
}

// Shutdown -----------------------------------------

void Test_SL_Shutdown_ReturnsFalsePreInit(void) {
  assert(!SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsFalsePreInit");
}

// --------------------------------------------------
// Init
// --------------------------------------------------

// Mem Alloc Fail -----------------------------------

void Test_SL_Init_ReturnsFalseIfCallocFails(void) {
  TEST_SetCanCalloc(false);
  assert(!SL_Init());
  TEST_SetCanCalloc(true);
  TEST_PASS("Test_SL_Init_ReturnsFalseIfCallocFails");
}

void Test_SL_Init_ReturnsFalseIfMallocFails(void) {
  TEST_SetCanMalloc(false);
  assert(!SL_Init());
  TEST_SetCanMalloc(true);
  TEST_PASS("Test_SL_Init_ReturnsFalseIfMallocFails");
}

// Success ------------------------------------------

void Test_SL_Init_ReturnsTrue(void) {
  assert(SL_Init());
  TEST_PASS("Test_SL_Init_ReturnsTrue");
}

// --------------------------------------------------
// Post-Init
// --------------------------------------------------

void Test_SL_IsInitialized_ReturnsTruePostInit(void) {
  assert(SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsTrueAfterInit");
}



#ifdef __APPLE__

// GetDefaultDir succeeds

// GetGameDir fails
// SetGameDir fails with invalid path
// // Perhaps test this more than once
// SetGameDir succeeds
// GetGameDir succeeds

// GetGameFile fails
// SetGameFile fails with invalid path
// // Perhaps test this more than once
// SetGameFile succeeds
// GetGameFile succeeds

// must also test DirExists, FileExists
// change directories and check, change file and check
// // perhaps do the same again in another order
// must also create a TEST_Func to create/delete dirs and files
// // Or call delete dir on all once tests are over

// Also test alternate dir if default dir fails



#endif
#ifdef __linux__
#endif
#ifdef _WIN32
#endif

// --------------------------------------------------
// Shutdown
// --------------------------------------------------

void Test_SL_Shutdown_ReturnsTrueAfterInit(void) {
  assert(SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsTrueAfterInit");
}

// --------------------------------------------------
// Post-Shutdown
// --------------------------------------------------

void Test_SL_Shutdown_ReturnsFalseIfCalledTwice(void) {
  assert(!SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsFalseIfCalledTwice");
}

void Test_SL_IsInitialized_ReturnsFalseAfterShutdown(void) {
  assert(!SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsFalseAfterShutdown");
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
  puts("• Init");
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

  puts("\nTESTINT INIT");
  puts("• Mem Alloc Fail");
  Test_SL_Init_ReturnsFalseIfCallocFails();
  Test_SL_Init_ReturnsFalseIfMallocFails();
  puts("• Success");
  Test_SL_Init_ReturnsTrue();

  puts("\nTESTING POST-INIT");
  Test_SL_IsInitialized_ReturnsTruePostInit();
#ifdef __APPLE__
#endif
#ifdef __linux__
#endif
#ifdef _WIN32
#endif
  puts("");

  puts("\nTESTING SHUTDOWN");
  Test_SL_Shutdown_ReturnsTrueAfterInit();

  puts("\nTESTING POST-SHUTDOWN");
  Test_SL_Shutdown_ReturnsFalseIfCalledTwice();
  Test_SL_IsInitialized_ReturnsFalseAfterShutdown();

  puts("\nAll Tests Completed Successfully!");
  return 0;
}
