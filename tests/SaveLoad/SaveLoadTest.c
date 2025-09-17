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
#include "SaveLoadTest.h"
#include "assert.h"

// --------------------------------------------------
// Pre-Init - Internal
// --------------------------------------------------

void Test_SL_Internal_GetDefaultSysDir_ReturnsNullPreInit(void) {
  assert(!SL_Internal_GetDefaultSysDir());
  TEST_PASS("Test_SL_Internal_GetDefaultSysDir_ReturnsNULLPreInit");
}
void Test_SL_Internal_BeginSession_OnSaveModeReturnsFalsePreInit(void) {
  assert(!SL_Internal_BeginSession(SAVE, NULL,
                                   LOG_CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED));
  TEST_PASS("Test_SL_Internal_BeginSession_OnSaveModeReturnsFalsePreInit");
}
void Test_SL_Internal_BeginSession_OnLoadModeReturnsFalsePreInit(void) {
  assert(!SL_Internal_BeginSession(LOAD, NULL,
                                   LOG_CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED));
  TEST_PASS("Test_SL_Internal_BeginSession_OnLoadModeReturnsFalsePreInit");
}

// --------------------------------------------------
// Pre-Init - Public
// --------------------------------------------------

void Test_SL_IsInitialized_ReturnsFalsePreInit(void) {
  assert(!SL_IsInitialized());
  TEST_PASS("Test_SL_IsInitialized_ReturnsFalsePreInit");
}
void Test_SL_SetGameDir_ReturnsFalsePreInit(void) {
  assert(!SL_SetGameDir(NULL));
  TEST_PASS("Test_SL_SetGameDir_ReturnsFalsePreInit");
}
void Test_SL_SetGameFile_ReturnsFalsePreInit(void) {
  assert(!SL_SetGameFile(NULL));
  TEST_PASS("Test_SL_SetGameFile_ReturnsFalsePreInit");
}
void Test_SL_DirExists_ReturnsFalsePreInit(void) {
  assert(!SL_DirExists(NULL));
  TEST_PASS("Test_SL_DirExists_ReturnsFalsePreInit");
}
void Test_SL_FileExists_ReturnsFalsePreInit(void) {
  assert(!SL_FileExists(NULL));
  TEST_PASS("Test_SL_FileExists_ReturnsFalsePreInit");
}
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
void Test_SL_BeginLoadSession_ReturnsFalsePreInit(void) {
  assert(!SL_BeginLoadSession(NULL));
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
void Test_SL_DeleteSave_ReturnsFalsePreInit(void) {
  assert(!SL_DeleteSave());
  TEST_PASS("Test_SL_DeleteSave_ReturnsFalsePreInit");
}
void Test_SL_Shutdown_ReturnsFalsePreInit(void) {
  assert(!SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsFalsePreInit");
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

void Test_SL_Init_ReturnsTrueWithAllNullArgs(void) {
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
#ifdef __APPLE__
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
void Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnApple(void) {
  // bool SL_SetGameDir(char *dir);
  TEST_PASS("Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnApple");
}
void Test_SL_SetGameDir_ReturnsTrueForValidDirOnApple(void) {
  // bool SL_SetGameDir(char *dir);
  TEST_PASS("Test_SL_SetGameDir_ReturnsTrueForValidDirOnApple");
}
void Test_SL_GetGameDir_ReturnsCorrectDirAfterSetGameDirOnApple(void) {
  // do something
  TEST_PASS("Test_SL_GetGameDir_ReturnsCorrectDirAfterSetGameDirOnApple");
}
#endif
#ifdef __linux__
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
void Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnLinux(void) {
  // bool SL_SetGameDir(char *dir);
  TEST_PASS("Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnLinux");
}
void Test_SL_SetGameDir_ReturnsTrueForValidDirOnLinux(void) {
  // bool SL_SetGameDir(char *dir);
  TEST_PASS("Test_SL_SetGameDir_ReturnsTrueForValidDirOnLinux");
}
void Test_SL_GetGameDir_ReturnsCorrectDirAfterSetGameDirOnLinux(void) {
  // do something
  TEST_PASS("Test_SL_GetGameDir_ReturnsCorrectDirAfterSetGameDirOnLinux");
}
#endif
#ifdef _WIN32
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
void Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnWindows(void) {
  // bool SL_SetGameDir(char *dir);
  TEST_PASS("Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnWindows");
}
void Test_SL_SetGameDir_ReturnsTrueForValidDirOnWindows(void) {
  // bool SL_SetGameDir(char *dir);
  TEST_PASS("Test_SL_SetGameDir_ReturnsTrueForValidDirOnWindows");
}
void Test_SL_GetGameDir_ReturnsCorrectDirAfterSetGameDirOnWindows(void) {
  // do something
  TEST_PASS("Test_SL_GetGameDir_ReturnsCorrectDirAfterSetGameDirOnWindows");
}
#endif

// char *SL_GetGameFile(void);
// bool SL_SetGameFile(char *file) { return false; }
// char *SL_GetGameFile(void); again

// bool SL_DirExists(char *dir) { return false; } valid dir
// bool SL_DirExists(char *dir) { return false; } invalid dir

// bool SL_FileExists(char *file) { return false; } valid file
// bool SL_FileExists(char *file) { return false; } invalid file

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
  Test_SL_Internal_GetDefaultSysDir_ReturnsNullPreInit();
  Test_SL_Internal_BeginSession_OnSaveModeReturnsFalsePreInit();
  Test_SL_Internal_BeginSession_OnLoadModeReturnsFalsePreInit();
  puts("");

  puts("Testing Pre-Init - Public");
  Test_SL_IsInitialized_ReturnsFalsePreInit();
  Test_SL_SetGameDir_ReturnsFalsePreInit();
  Test_SL_SetGameFile_ReturnsFalsePreInit();
  Test_SL_DirExists_ReturnsFalsePreInit();
  Test_SL_FileExists_ReturnsFalsePreInit();
  Test_SL_BeginSaveSession_ReturnsFalsePreInit();
  Test_SL_SaveNext_ReturnsFalsePreInit();
  Test_SL_EndSaveSession_ReturnsFalsePreInit();
  Test_SL_BeginLoadSession_ReturnsFalsePreInit();
  Test_SL_HasNext_ReturnsFalsePreInit();
  Test_SL_LoadNext_ReturnsNullPreInit();
  Test_SL_LoadNextTo_ReturnsFalsePreInit();
  Test_SL_EndLoadSession_ReturnsFalsePreInit();
  Test_SL_DeleteSave_ReturnsFalsePreInit();
  Test_SL_Shutdown_ReturnsFalsePreInit();
  puts("");

  puts("Testing Init - Fail Mem Alloc");
  Test_SL_Init_ReturnsFalseIfCallocFails();
  Test_SL_Init_ReturnsFalseIfMallocFails();
  puts("");

  puts("Testing Init - NULL Args");
  Test_SL_Init_ReturnsTrueWithAllNullArgs();
  Test_SL_Init_ReturnsFalseIfCalledTwice();
  puts("");

  puts("Testing Post-Init - Public");
  Test_SL_IsInitialized_ReturnsTrueAfterInit();
#ifdef __APPLE__
  Test_SL_GetGameDir_ReturnsDefaultSysDirOnApple();
  Test_SL_GetGameDir_ReturnsAltSysDirOnApple();
  Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnApple();
  Test_SL_SetGameDir_ReturnsTrueForValidDirOnApple();
#endif
#ifdef __linux__
  Test_SL_GetGameDir_ReturnsDefaultSysDirOnLinux();
  Test_SL_GetGameDir_ReturnsAltSysDirOnLinux();
  Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnLinux();
  Test_SL_SetGameDir_ReturnsTrueForValidDirOnLinux();
#endif
#ifdef _WIN32
  Test_SL_GetGameDir_ReturnsDefaultSysDirOnWindows();
  Test_SL_GetGameDir_ReturnsAltSysDirOnWindows();
  Test_SL_SetGameDir_ReturnsFalseForInvalidDirOnWindows();
  Test_SL_SetGameDir_ReturnsTrueForValidDirOnWindows();
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