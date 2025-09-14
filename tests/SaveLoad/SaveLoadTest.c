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
 *   Test_SM_RegisterState_ReturnsFalseIfNameIsNULL
 *     Tests that SM_RegisterState returns false when the provided name is NULL.
 *
 * @author Vitor Betmann
 */

#include "../include/SaveLoad.h"
#include "../src/SaveLoad/SaveLoadInternal.h"
#include "../src/_Internals/Test/TestInternal.h"
#include "SaveLoadTest.h"
#include "assert.h"

// --------------------------------------------------
// Pre-initialization - Internal
// --------------------------------------------------

void Test_SL_Internal_GetDefaultOSDir_ReturnsNULLBeforeInitialization(void) {}
void Test_SL_Internal_GetGameName_ReturnsNULLBeforeInitialization(void) {}
void Test_SL_Internal_BeginSession_ReturnsFalseBeforeInitialization(void) {}

// --------------------------------------------------
// Pre-initialization - Public
// --------------------------------------------------

void Test_SL_IsInitialized_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_SetGameDir_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_SetGameFile_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_DirExists_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_FileExists_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_BeginSaveSession_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_SaveNext_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_EndSaveSession_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_BeginLoadSession_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_HasNext_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_LoadNext_ReturnsNULLBeforeInitialization(void) {}
void Test_SL_LoadNextTo_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_EndLoadSession_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_DeleteSave_ReturnsFalseBeforeInitialization(void) {}
void Test_SL_Shutdown_ReturnsFalseBeforeInitialization(void) {}

// --------------------------------------------------
// Initialization
// --------------------------------------------------

void Test_SL_Init_ReturnsFalseIfMallocFails(void) {}
void Test_SL_Init_ReturnsTrueAndInitializesTracker(void) {}
void Test_SL_Init_ReturnsFalseIfCalledTwice(void) {}

// --------------------------------------------------
// Post-Initialization - Internal
// --------------------------------------------------
// --------------------------------------------------
// Pre-Registration - Internal
// --------------------------------------------------
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
  Test_SL_Internal_BeginSession_ReturnsFalseBeforeInitialization();
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
  Test_SL_Init_ReturnsFalseIfMallocFails();
  Test_SL_Init_ReturnsTrueAndInitializesTracker();
  Test_SL_Init_ReturnsFalseIfCalledTwice();
  puts("");

  puts("Testing Post-Initialization - Internal");
  puts("");

  puts("Testing Pre-Registration - Internal");
  puts("");

  puts("All Tests Completed Successfully!");
  return 0;
}