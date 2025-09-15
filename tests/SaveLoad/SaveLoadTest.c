// TODO add instructions to text below about how to create mocks and how to name
// them perhaps a few more details on how to name stuff
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
#include "SaveLoadTest.h"
#include "assert.h"

// --------------------------------------------------
// Pre-initialization - Internal
// --------------------------------------------------

void Test_SL_Internal_GetDefaultOSDir_ReturnsNULLBeforeInitialization(void) {
  assert(!SL_Internal_GetDefaultOSDir());
  TEST_PASS("Test_SL_Internal_GetDefaultOSDir_ReturnsNULLBeforeInitialization");
}
void Test_SL_Internal_GetGameName_ReturnsNULLBeforeInitialization(void) {
  assert(!SL_Internal_GetGameName());
  TEST_PASS("Test_SL_Internal_GetGameName_ReturnsNULLBeforeInitialization");
}
void Test_SL_Internal_BeginSession_OnSaveModeReturnsFalseBeforeInitialization(
    void) {
  assert(!SL_Internal_BeginSession(SAVE, NULL,
                                   LOG_CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED));
  TEST_PASS("Test_SL_Internal_BeginSession_"
            "OnSaveModeReturnsFalseBeforeInitialization");
}
void Test_SL_Internal_BeginSession_OnLoadModeReturnsFalseBeforeInitialization(
    void) {
  assert(!SL_Internal_BeginSession(LOAD, NULL,
                                   LOG_CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED));
  TEST_PASS("Test_SL_Internal_BeginSession_"
            "OnLoadModeReturnsFalseBeforeInitialization");
}

// --------------------------------------------------
// Pre-initialization - Public
// --------------------------------------------------

void Test_SL_IsInitialized_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsFalseBeforeInitialization");
}
void Test_SL_SetGameDir_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_SetGameDir(NULL));
  TEST_PASS("Test_SL_SetGameDir_ReturnsFalseBeforeInitialization");
}
void Test_SL_SetGameFile_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_SetGameFile(NULL));
  TEST_PASS("Test_SL_SetGameFile_ReturnsFalseBeforeInitialization");
}
void Test_SL_DirExists_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_DirExists(NULL));
  TEST_PASS("Test_SL_DirExists_ReturnsFalseBeforeInitialization");
}
void Test_SL_FileExists_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_FileExists(NULL));
  TEST_PASS("Test_SL_FileExists_ReturnsFalseBeforeInitialization");
}
void Test_SL_BeginSaveSession_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_BeginSaveSession(NULL));
  TEST_PASS("Test_SL_BeginSaveSession_ReturnsFalseBeforeInitialization");
}
void Test_SL_SaveNext_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_SaveNext(NULL));
  TEST_PASS("Test_SL_SaveNext_ReturnsFalseBeforeInitialization");
}
void Test_SL_EndSaveSession_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_EndSaveSession());
  TEST_PASS("Test_SL_EndSaveSession_ReturnsFalseBeforeInitialization");
}
void Test_SL_BeginLoadSession_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_BeginLoadSession(NULL));
  TEST_PASS("Test_SL_BeginLoadSession_ReturnsFalseBeforeInitialization");
}
void Test_SL_HasNext_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_HasNext());
  TEST_PASS("Test_SL_HasNext_ReturnsFalseBeforeInitialization");
}
void Test_SL_LoadNext_ReturnsNULLBeforeInitialization(void) {
  assert(!SL_LoadNext());
  TEST_PASS("Test_SL_LoadNext_ReturnsNULLBeforeInitialization");
}
void Test_SL_LoadNextTo_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_LoadNextTo(NULL, 0));
  TEST_PASS("Test_SL_LoadNextTo_ReturnsFalseBeforeInitialization");
}
void Test_SL_EndLoadSession_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_EndLoadSession());
  TEST_PASS("Test_SL_EndLoadSession_ReturnsFalseBeforeInitialization");
}
void Test_SL_DeleteSave_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_DeleteSave());
  TEST_PASS("Test_SL_DeleteSave_ReturnsFalseBeforeInitialization");
}
void Test_SL_Shutdown_ReturnsFalseBeforeInitialization(void) {
  assert(!SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsFalseBeforeInitialization");
}

// --------------------------------------------------
// Initialization
// --------------------------------------------------

void Test_SL_Init_ReturnsFalseIfCallocFails(void) {
  TEST_SetCanCalloc(false);
  assert(!SL_Init(NULL, NULL));
  TEST_SetCanCalloc(true);
  TEST_PASS("Test_SL_Init_ReturnsFalseIfCallocFails");
}
void Test_SL_Init_ReturnsFalseIfMallocFails(void) {
  TEST_SetCanMalloc(false);
  assert(!SL_Init(NULL, NULL));
  TEST_SetCanMalloc(true);
  TEST_PASS("Test_SL_Init_ReturnsFalseIfMallocFails");
}
void Test_SL_Init_ReturnsTrueOnFirstCall(void) {
  assert(SL_Init(NULL, NULL));
  TEST_PASS("Test_SL_Init_ReturnsTrueOnFirstCall");
}
void Test_SL_Init_ReturnsFalseIfCalledTwice(void) {
  assert(!SL_Init(NULL, NULL));
  TEST_PASS("Test_SL_Init_ReturnsFalseIfCalledTwice");
}

// --------------------------------------------------
// Post-Initialization - Internal
// --------------------------------------------------
// --------------------------------------------------
// Post-Initialization - Public
// --------------------------------------------------

void Test_SL_IsInitialized_ReturnsTrueAfterInitialization(void) {
  assert(SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsTrueAfterInitialization");
}

// --------------------------------------------------
// State Registration
// --------------------------------------------------
// --------------------------------------------------
// Post-Registration - Internal
// --------------------------------------------------
// --------------------------------------------------
// Pre-transitions
// --------------------------------------------------
// --------------------------------------------------
// Transitions
// --------------------------------------------------
// --------------------------------------------------
// Getters
// --------------------------------------------------
// --------------------------------------------------
// Checks
// --------------------------------------------------
// --------------------------------------------------
// Update and Draw
// --------------------------------------------------
// --------------------------------------------------
// Shutdown
// --------------------------------------------------
// --------------------------------------------------
// Post-Shutdown access - Public
// --------------------------------------------------
// --------------------------------------------------
// Post-Shutdown access - Internal
// --------------------------------------------------
// --------------------------------------------------
// Stress tests
// --------------------------------------------------

// --------------------------------------------------
// Finger's crossed!
// --------------------------------------------------

int main() {
  puts("");
  puts("Testing Pre-Initialization - Internal");
  Test_SL_Internal_GetDefaultOSDir_ReturnsNULLBeforeInitialization();
  Test_SL_Internal_GetGameName_ReturnsNULLBeforeInitialization();
  Test_SL_Internal_BeginSession_OnSaveModeReturnsFalseBeforeInitialization();
  Test_SL_Internal_BeginSession_OnLoadModeReturnsFalseBeforeInitialization();
  puts("");

  puts("Testing Pre-Initialization - Public");
  Test_SL_IsInitialized_ReturnsFalseBeforeInitialization();
  Test_SL_SetGameDir_ReturnsFalseBeforeInitialization();
  Test_SL_SetGameFile_ReturnsFalseBeforeInitialization();
  Test_SL_DirExists_ReturnsFalseBeforeInitialization();
  Test_SL_FileExists_ReturnsFalseBeforeInitialization();
  Test_SL_BeginSaveSession_ReturnsFalseBeforeInitialization();
  Test_SL_SaveNext_ReturnsFalseBeforeInitialization();
  Test_SL_EndSaveSession_ReturnsFalseBeforeInitialization();
  Test_SL_BeginLoadSession_ReturnsFalseBeforeInitialization();
  Test_SL_HasNext_ReturnsFalseBeforeInitialization();
  Test_SL_LoadNext_ReturnsNULLBeforeInitialization();
  Test_SL_LoadNextTo_ReturnsFalseBeforeInitialization();
  Test_SL_EndLoadSession_ReturnsFalseBeforeInitialization();
  Test_SL_DeleteSave_ReturnsFalseBeforeInitialization();
  Test_SL_Shutdown_ReturnsFalseBeforeInitialization();
  puts("");

  puts("Testing Initialization");
  Test_SL_Init_ReturnsFalseIfCallocFails();
  Test_SL_Init_ReturnsFalseIfMallocFails();
  Test_SL_Init_ReturnsTrueOnFirstCall();
  Test_SL_Init_ReturnsFalseIfCalledTwice();
  puts("");

  puts("Testing Post-Initialization - Internal");
  puts("");

  puts("Testing Post-Initialization - Public");
  puts("");

  puts("All Tests Completed Successfully!");
  return 0;
}