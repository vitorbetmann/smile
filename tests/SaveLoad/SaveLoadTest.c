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

void Test_SL_DeleteFile_ReturnsFalsePreInit(void) {
  assert(!SL_DeleteFile(NULL));
  TEST_PASS("Test_SL_DeleteFile_ReturnsFalsePreInit");
}

void Test_SL_DeleteDir_ReturnsFalsePreInit(void) {
  assert(!SL_DeleteDir(NULL));
  TEST_PASS("Test_SL_DeleteDir_ReturnsFalsePreInit");
}

// Shutdown -----------------------------------------

void Test_SL_Shutdown_ReturnsFalsePreInit(void) {
  assert(!SL_Shutdown());
  TEST_PASS("Test_SL_Shutdown_ReturnsFalsePreInit");
}

// --------------------------------------------------
// Init - Fail Mem Alloc
// --------------------------------------------------

void Test_SL_Init_ReturnsFalseIfCallocFails(void) {
  TEST_SetCanCalloc(false);
  assert(!SL_Init(MOCK_GAME_NAME));
  TEST_SetCanCalloc(true);
  TEST_PASS("Test_SL_Init_ReturnsFalseIfCallocFails");
}

void Test_SL_Init_ReturnsFalseIfMallocFails(void) {
  TEST_SetCanMalloc(false);
  assert(!SL_Init(MOCK_GAME_NAME));
  TEST_SetCanMalloc(true);
  TEST_PASS("Test_SL_Init_ReturnsFalseIfMallocFails");
}

// --------------------------------------------------
// Init - NULL Args
// --------------------------------------------------

void Test_SL_Init_ReturnsTrueWithAllNullArgs(void) {
  assert(SL_Init(MOCK_GAME_NAME));
  TEST_PASS("Test_SL_Init_ReturnsTrueOnFirstCall");
}

void Test_SL_Init_ReturnsFalseIfCalledTwice(void) {
  assert(!SL_Init(MOCK_GAME_NAME));
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
  // TODO Create a func for this... From here1
  const char *homeDir = getenv("HOME");
  size_t homeDirLen = strlen(homeDir);
  size_t sysDirLen = strlen(DEFAULT_SYS_DIR);
  char smileDir[] = "smile/";
  int smileLen = strlen(smileDir);
  size_t gameNameLen = strlen(MOCK_GAME_NAME);
  size_t bufferLen = homeDirLen + sysDirLen + smileLen + gameNameLen + 2;

  char *buffer = TEST_Malloc(bufferLen);
  if (!buffer) {
    exit(EXIT_FAILURE);
  }
  snprintf(buffer, bufferLen, "%s%s%s%s/", homeDir, DEFAULT_SYS_DIR, smileDir,
           MOCK_GAME_NAME);
  // to here1

  assert(TEST_COMP_NAME(SL_GetGameDir(), buffer));
  TEST_PASS("Test_SL_GetGameDir_ReturnsDefaultSystemDirectoryOnApple");
}

void Test_SL_GetGameDir_ReturnsAltSysDirOnApple(void) {
  // TODO enable alt sys dir
  // assert(TEST_COMP_NAME(SL_GetGameDir(), "./"));
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
  assert(SL_Init(MOCK_GAME_NAME));
  SL_Shutdown();
  TEST_PASS("Test_SL_Init_ReturnsTrueWithNULLFile");
}

void Test_SL_Init_ReturnsTrueWithNULLDir(void) {
  assert(SL_Init(MOCK_GAME_NAME));
  SL_Shutdown();
  TEST_PASS("Test_SL_Init_ReturnsTrueWithNULLDir");
}

void Test_SL_Init_ReturnsTrueWithNoNULLArgs(void) {
  assert(SL_Init(MOCK_GAME_NAME));
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
  puts("\nTESTINT PRE-INIT");
  puts("• IsInitialized");
  Test_SL_IsInitialized_ReturnsFalsePreInit();
  puts("• Game Dir");
  Test_SL_GetGameDir_ReturnsFalsePreInit();
  Test_SL_DirExists_ReturnsFalsePreInit();
  Test_SL_SetGameDir_ReturnsFalsePreInit();
  puts("• Game File");
  Test_SL_GetGameFile_ReturnsFalsePreInit();
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
  Test_SL_DeleteFile_ReturnsFalsePreInit();
  Test_SL_DeleteDir_ReturnsFalsePreInit();
  puts("• Shutdown");
  Test_SL_Shutdown_ReturnsFalsePreInit();

  puts("\nTESTINT INIT");
  puts("• Fail Mem Alloc");
  Test_SL_Init_ReturnsFalseIfCallocFails();
  Test_SL_Init_ReturnsFalseIfMallocFails();
  puts("• Null Args");
  Test_SL_Init_ReturnsTrueWithAllNullArgs();
  Test_SL_Init_ReturnsFalseIfCalledTwice();

  puts("• TESTING POST-INIT");
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

  puts("Testing Post-Shutdown");
  Test_SL_Shutdown_ReturnsFalseIfCalledTwice();
  Test_SL_IsInitialized_ReturnsFalseAfterShutdown();
  puts("");

  puts("Testing Init - No NULL Args");
  puts("");

  puts("All Tests Completed Successfully!");
  return 0;
}
