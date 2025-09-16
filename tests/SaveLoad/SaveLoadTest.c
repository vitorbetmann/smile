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
// Pre-Init - Internal
// --------------------------------------------------

void Test_SL_Internal_GetDefaultSysDir_ReturnsNULLBeforeInit(void) {
  assert(!SL_Internal_GetDefaultSysDir());
  TEST_PASS("Test_SL_Internal_GetDefaultSysDir_ReturnsNULLBeforeInit");
}
void Test_SL_Internal_BeginSession_OnSaveModeReturnsFalseBeforeInit(void) {
  assert(!SL_Internal_BeginSession(SAVE, NULL,
                                   LOG_CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED));
  TEST_PASS("Test_SL_Internal_BeginSession_OnSaveModeReturnsFalseBeforeInit");
}
void Test_SL_Internal_BeginSession_OnLoadModeReturnsFalseBeforeInit(void) {
  assert(!SL_Internal_BeginSession(LOAD, NULL,
                                   LOG_CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED));
  TEST_PASS("Test_SL_Internal_BeginSession_OnLoadModeReturnsFalseBeforeInit");
}

// --------------------------------------------------
// Pre-Init - Public
// --------------------------------------------------

void Test_SL_IsInitialized_ReturnsFalseBeforeInit(void) {
  assert(!SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsFalseBeforeInit");
}
void Test_SL_SetGameDir_ReturnsFalseBeforeInit(void) {
  assert(!SL_SetGameDir(NULL));
  TEST_PASS("Test_SL_SetGameDir_ReturnsFalseBeforeInit");
}
void Test_SL_SetGameFile_ReturnsFalseBeforeInit(void) {
  assert(!SL_SetGameFile(NULL));
  TEST_PASS("Test_SL_SetGameFile_ReturnsFalseBeforeInit");
}
void Test_SL_DirExists_ReturnsFalseBeforeInit(void) {
  assert(!SL_DirExists(NULL));
  TEST_PASS("Test_SL_DirExists_ReturnsFalseBeforeInit");
}
void Test_SL_FileExists_ReturnsFalseBeforeInit(void) {
  assert(!SL_FileExists(NULL));
  TEST_PASS("Test_SL_FileExists_ReturnsFalseBeforeInit");
}
void Test_SL_BeginSaveSession_ReturnsFalseBeforeInit(void) {
  assert(!SL_BeginSaveSession(NULL));
  TEST_PASS("Test_SL_BeginSaveSession_ReturnsFalseBeforeInit");
}
void Test_SL_SaveNext_ReturnsFalseBeforeInit(void) {
  assert(!SL_SaveNext(NULL));
  TEST_PASS("Test_SL_SaveNext_ReturnsFalseBeforeInit");
}
void Test_SL_EndSaveSession_ReturnsFalseBeforeInit(void) {
  assert(!SL_EndSaveSession());
  TEST_PASS("Test_SL_EndSaveSession_ReturnsFalseBeforeInit");
}
void Test_SL_BeginLoadSession_ReturnsFalseBeforeInit(void) {
  assert(!SL_BeginLoadSession(NULL));
  TEST_PASS("Test_SL_BeginLoadSession_ReturnsFalseBeforeInit");
}
void Test_SL_HasNext_ReturnsFalseBeforeInit(void) {
  assert(!SL_HasNext());
  TEST_PASS("Test_SL_HasNext_ReturnsFalseBeforeInit");
}
void Test_SL_LoadNext_ReturnsNULLBeforeInit(void) {
  assert(!SL_LoadNext());
  TEST_PASS("Test_SL_LoadNext_ReturnsNULLBeforeInit");
}
void Test_SL_LoadNextTo_ReturnsFalseBeforeInit(void) {
  assert(!SL_LoadNextTo(NULL, 0));
  TEST_PASS("Test_SL_LoadNextTo_ReturnsFalseBeforeInit");
}
void Test_SL_EndLoadSession_ReturnsFalseBeforeInit(void) {
  assert(!SL_EndLoadSession());
  TEST_PASS("Test_SL_EndLoadSession_ReturnsFalseBeforeInit");
}
void Test_SL_DeleteSave_ReturnsFalseBeforeInit(void) {
  assert(!SL_DeleteSave());
  TEST_PASS("Test_SL_DeleteSave_ReturnsFalseBeforeInit");
}
void Test_SL_Shutdown_ReturnsFalseBeforeInit(void) {
  assert(!SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsFalseBeforeInit");
}

// --------------------------------------------------
// Init - Fail Mem Alloc
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

// --------------------------------------------------
// Init - NULL Args
// --------------------------------------------------

void Test_SL_Init_ReturnsTrueWithAllNULLArgs(void) {
  assert(SL_Init(NULL, NULL));
  TEST_PASS("Test_SL_Init_ReturnsTrueOnFirstCall");
}
void Test_SL_Init_ReturnsFalseIfCalledTwice(void) {
  assert(!SL_Init(NULL, NULL));
  TEST_PASS("Test_SL_Init_ReturnsFalseIfCalledTwice");
}

// --------------------------------------------------
// Post-Init - Public
// --------------------------------------------------

void Test_SL_IsInitialized_ReturnsTrueAfterInit(void) {
  assert(SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsTrueAfterInit");
}
void Test_SL_GetGameDir_ReturnsDefaultSysDirOnApple(void) {
  char *buffer = SL_GetGameDir();
  assert(TEST_COMP_NAME(buffer, "./"));
  free(buffer);
  buffer = NULL;
  TEST_PASS("Test_SL_GetGameDir_ReturnsDefaultSystemDirectoryOnApple");
}
void Test_SL_GetGameDir_ReturnsAltSysDirOnApple(void) {
  // TODO enable alt sys dir
  char *buffer = SL_GetGameDir();
  assert(TEST_COMP_NAME(buffer, "./"));
  free(buffer);
  buffer = NULL;
  TEST_PASS("Test_SL_GetGameDir_ReturnsAltSysDirOnApple");
}
void Test_SL_GetGameDir_ReturnsDefaultSysDirOnLinux(void) {
  char *buffer = SL_GetGameDir();
  assert(TEST_COMP_NAME(buffer, "./"));
  free(buffer);
  buffer = NULL;
  TEST_PASS("Test_SL_GetGameDir_ReturnsDefaultSysDirOnLinux");
}
void Test_SL_GetGameDir_ReturnsAltSysDirOnLinux(void) {
  // TODO enable alt sys dir
  char *buffer = SL_GetGameDir();
  assert(TEST_COMP_NAME(buffer, "./"));
  free(buffer);
  buffer = NULL;
  TEST_PASS("Test_SL_GetGameDir_ReturnsAltSysDirOnLinux");
}
void Test_SL_GetGameDir_ReturnsDefaultSysDirOnWindows(void) {
  char *buffer = SL_GetGameDir();
  assert(TEST_COMP_NAME(buffer, "./"));
  free(buffer);
  buffer = NULL;
  TEST_PASS("Test_SL_GetGameDir_ReturnsDefaultSysDirOnWindows");
}
void Test_SL_GetGameDir_ReturnsAltSysDirOnWindows(void) {
  // TODO enable alt sys dir
  char *buffer = SL_GetGameDir();
  assert(TEST_COMP_NAME(buffer, "./"));
  free(buffer);
  buffer = NULL;
  TEST_PASS("Test_SL_GetGameDir_ReturnsAltSysDirOnWindows");
}

// bool SL_SetGameDir(char *dir);
// char *SL_GetGameDir(void) { return NULL; } again

// char *SL_GetGameFile(void);
// bool SL_SetGameFile(char *file) { return false; }
// char *SL_GetGameFile(void); again

// bool SL_DirExists(char *dir) { return false; }
// bool SL_DirExists(char *dir) { return false; }

// bool SL_FileExists(char *file) { return false; }
// bool SL_FileExists(char *file) { return false; }

// --------------------------------------------------
// Shutdown
// --------------------------------------------------

void Test_SL_Shutdown_ReturnsTrueAfterInit(void) {
  assert(SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsTrueAfterInit");
}

// --------------------------------------------------
// Post-Shutdown access - Public
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
// Init -  No NULL Args
// --------------------------------------------------

void Test_SL_Init_ReturnsTrueWithNULLFile(void) {
  assert(SL_Init(NULL, NULL));
  SL_Shutdown();
  TEST_PASS("Test_SL_Init_ReturnsTrueWithNULLFile");
}
void Test_SL_Init_ReturnsTrueWithNULLDir(void) {
  assert(SL_Init(NULL, NULL));
  SL_Shutdown();
  TEST_PASS("Test_SL_Init_ReturnsTrueWithNULLDir");
}
void Test_SL_Init_ReturnsTrueWithNoNULLArgs(void) {
  assert(SL_Init(NULL, NULL));
  SL_Shutdown();
  TEST_PASS("Test_SL_Init_ReturnsTrueWithNoNULLArgs");
}

// --------------------------------------------------
// Stress tests
// --------------------------------------------------

// --------------------------------------------------
// Finger's crossed!
// --------------------------------------------------

int main() {
  puts("");
  puts("Testing Pre-Init - Internal");
  Test_SL_Internal_GetDefaultSysDir_ReturnsNULLBeforeInit();
  Test_SL_Internal_BeginSession_OnSaveModeReturnsFalseBeforeInit();
  Test_SL_Internal_BeginSession_OnLoadModeReturnsFalseBeforeInit();
  puts("");

  puts("Testing Pre-Init - Public");
  Test_SL_IsInitialized_ReturnsFalseBeforeInit();
  Test_SL_SetGameDir_ReturnsFalseBeforeInit();
  Test_SL_SetGameFile_ReturnsFalseBeforeInit();
  Test_SL_DirExists_ReturnsFalseBeforeInit();
  Test_SL_FileExists_ReturnsFalseBeforeInit();
  Test_SL_BeginSaveSession_ReturnsFalseBeforeInit();
  Test_SL_SaveNext_ReturnsFalseBeforeInit();
  Test_SL_EndSaveSession_ReturnsFalseBeforeInit();
  Test_SL_BeginLoadSession_ReturnsFalseBeforeInit();
  Test_SL_HasNext_ReturnsFalseBeforeInit();
  Test_SL_LoadNext_ReturnsNULLBeforeInit();
  Test_SL_LoadNextTo_ReturnsFalseBeforeInit();
  Test_SL_EndLoadSession_ReturnsFalseBeforeInit();
  Test_SL_DeleteSave_ReturnsFalseBeforeInit();
  Test_SL_Shutdown_ReturnsFalseBeforeInit();
  puts("");

  puts("Testing Init - Fail Mem Alloc");
  Test_SL_Init_ReturnsFalseIfCallocFails();
  Test_SL_Init_ReturnsFalseIfMallocFails();
  puts("");

  puts("Testing Init - NULL Args");
  Test_SL_Init_ReturnsTrueWithAllNULLArgs();
  Test_SL_Init_ReturnsFalseIfCalledTwice();
  puts("");

  puts("Testing Post-Init - Public");
  Test_SL_IsInitialized_ReturnsTrueAfterInit();
#ifdef __APPLE__
  Test_SL_GetGameDir_ReturnsDefaultSysDirOnApple();
  Test_SL_GetGameDir_ReturnsAltSysDirOnApple();
#endif
#ifdef _WIN32
  Test_SL_GetGameDir_ReturnsDefaultSysDirOnLinux();
  Test_SL_GetGameDir_ReturnsAltSysDirOnLinux();
#endif
#ifdef __linux__
  Test_SL_GetGameDir_ReturnsDefaultSysDirOnWindows();
  Test_SL_GetGameDir_ReturnsAltSysDirOnWindows();
#endif
  puts("");

  puts("Testing Shutdown");
  Test_SL_Shutdown_ReturnsTrueAfterInit();
  puts("");

  puts("Testing Post-Shutdown - Public");
  Test_SL_Shutdown_ReturnsFalseIfCalledTwice();
  Test_SL_IsInitialized_ReturnsFalseAfterShutdown();
  puts("");

  puts("Testing Init - No NULL Args");
  puts("");

  puts("All Tests Completed Successfully!");
  return 0;
}