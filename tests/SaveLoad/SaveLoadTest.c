// /* TODO add instructions to text below about how to create mocks and how to name
//  * them perhaps a few more details on how to name stuff
//  *
//  * Like for example, when using NULL in the name of a func, call it Null for
//  * readability
//  * And when writing tests, put one immediately below the other and only skip a
//  * line when testing a different routine
//  *
//  * and the puts' in main, how to write them as well (e.g. all caps for new sections,
//  * start with a '•' for subsections
//  */
//
// /*
//  * Test Naming Convention:
//  *
//  * Each test function is named following the pattern:
//  *
//  *   Test_<FunctionName>_<ExpectedBehavior>
//  *
//  * where:
//  *   - Test_SL_: prefix indicating this is a SaveLoad test.
//  *   - <FunctionName>: the name of the SaveLoad function or feature under
//  * test.
//  *   - <ExpectedBehavior>: a short description of what the test checks,
//  *                        often stating the expected outcome or condition.
//  *
//  * Example:
//  *   Test_SL_Init_FailsIfMallocFails
//  *
//  * @author Vitor Betmann
//  */
//
// #include "SaveLoadTest.h"
// #include "SaveLoad.h"
// #include "../src/SaveLoad/SaveLoadInternal.h"
// #include "../src/_Internal/Test/TestInternal.h"
// #include <assert.h>
// #include <string.h>
//
// // --------------------------------------------------
// // Pre-Init
// // --------------------------------------------------
//
// // Init Related -------------------------------------
//
// void Test_SL_IsInitialized_FailsPreInit(void) {
//   assert(!SL_IsInitialized());
//   TEST_Pass("Test_SL_IsInitialized_FailsPreInit");
// }
//
// // Game Dir -----------------------------------------
//
// void Test_SL_GetGameDir_FailsPreInit(void) {
//   assert(!SL_GetGameDir());
//   TEST_Pass("Test_SL_GetGameDir_FailsPreInit");
// }
//
// void Test_SL_GetDefaultDir_FailsPreInit(void) {
//   assert(!SL_GetDefaultDir());
//   TEST_Pass("Test_SL_GetDefaultDir_FailsPreInit");
// }
//
// void Test_SL_SetGameDir_FailsPreInit(void) {
//   assert(!SL_SetGameDir(NULL));
//   TEST_Pass("Test_SL_SetGameDir_FailsPreInit");
// }
//
// void Test_SL_DirExists_FailsPreInit(void) {
//   assert(!SL_DirExists(NULL));
//   TEST_Pass("Test_SL_DirExists_FailsPreInit");
// }
//
// // Game File ----------------------------------------
//
// void Test_SL_GetGameFile_FailsPreInit(void) {
//   assert(!SL_GetGameFile());
//   TEST_Pass("Test_SL_GetGameFile_FailsPreInit");
// }
//
// void Test_SL_GetGamePath_FailsPreInit(void) {
//   assert(!SL_GetGamePath());
//   TEST_Pass("Test_SL_GetGamePath_FailsPreInit");
// }
//
// void Test_SL_SetGameFile_FailsPreInit(void) {
//   assert(!SL_SetGameFile(NULL));
//   TEST_Pass("Test_SL_SetGameFile_FailsPreInit");
// }
//
// void Test_SL_FileExists_FailsPreInit(void) {
//   assert(!SL_FileExists(NULL));
//   TEST_Pass("Test_SL_FileExists_FailsPreInit");
// }
//
// // Save ---------------------------------------------
//
// void Test_SL_BeginSaveSession_FailsPreInit(void) {
//   assert(!SL_BeginSaveSession());
//   TEST_Pass("Test_SL_BeginSaveSession_FailsPreInit");
// }
//
// void Test_SL_SaveNext_FailsPreInit(void) {
//   assert(!SL_SaveNext(NULL));
//   TEST_Pass("Test_SL_SaveNext_FailsPreInit");
// }
//
// void Test_SL_EndSaveSession_FailsPreInit(void) {
//   assert(!SL_EndSaveSession());
//   TEST_Pass("Test_SL_EndSaveSession_FailsPreInit");
// }
//
// // Load ---------------------------------------------
//
// void Test_SL_BeginLoadSession_FailsPreInit(void) {
//   assert(!SL_BeginLoadSession());
//   TEST_Pass("Test_SL_BeginLoadSession_FailsPreInit");
// }
//
// void Test_SL_HasNext_FailsPreInit(void) {
//   assert(!SL_HasNext());
//   TEST_Pass("Test_SL_HasNext_FailsPreInit");
// }
//
// void Test_SL_LoadNext_FailsPreInit(void) {
//   assert(!SL_LoadNext());
//   TEST_Pass("Test_SL_LoadNext_FailsPreInit");
// }
//
// void Test_SL_LoadNextTo_FailsPreInit(void) {
//   assert(!SL_LoadNextTo(NULL, 0));
//   TEST_Pass("Test_SL_LoadNextTo_FailsPreInit");
// }
//
// void Test_SL_EndLoadSession_FailsPreInit(void) {
//   assert(!SL_EndLoadSession());
//   TEST_Pass("Test_SL_EndLoadSession_FailsPreInit");
// }
//
// // Delete -------------------------------------------
//
// void Test_SL_DeleteDir_FailsPreInit(void) {
//   assert(!SL_DeleteDir(NULL));
//   TEST_Pass("Test_SL_DeleteDir_FailsPreInit");
// }
//
// void Test_SL_DeleteFile_FailsPreInit(void) {
//   assert(!SL_DeleteFile(NULL));
//   TEST_Pass("Test_SL_DeleteFile_FailsPreInit");
// }
//
// // Shutdown -----------------------------------------
//
// void Test_SL_Shutdown_FailsPreInit(void) {
//   assert(!SL_Shutdown());
//   TEST_Pass("Test_SL_Shutdown_FailsPreInit");
// }
//
// // --------------------------------------------------
// // Init
// // --------------------------------------------------
//
// // Mem Alloc Fail -----------------------------------
//
// void Test_SL_Init_FailsIfCallocFails(void) {
//   TEST_Disable(CALLOC, 1);
//   assert(!SL_Init());
//   TEST_Pass("Test_SL_Init_FailsIfCallocFails");
// }
//
// void Test_SL_Init_FailsIfMallocFails(void) {
//   TEST_Disable(MALLOC, 1);
//   assert(!SL_Init());
//   TEST_Pass("Test_SL_Init_FailsIfMallocFails");
// }
//
// // Success ------------------------------------------
//
// void Test_SL_Init_Succeeds(void) {
//   assert(SL_Init());
//   TEST_Pass("Test_SL_Init_Succeeds");
// }
//
// // --------------------------------------------------
// // Post-Init
// // --------------------------------------------------
//
// // Init Related -------------------------------------
//
// void Test_SL_IsInitialized_SucceedsPostInit(void) {
//   assert(SL_IsInitialized());
//   TEST_Pass("Test_SL_IsInitialized_SucceedsAfterInit");
// }
//
// // Game Dir -----------------------------------------
//
// void Test_SL_GetDefaultDir_ReturnsPathToSmileDir(void) {
//   const char *home = getenv("HOME");
//   const size_t totalLen = strlen(home) + strlen(DEFAULT_SYS_DIR) + strlen(SMILE_DIR) + 1;
//   char *smilePath = malloc(totalLen);
//   snprintf(smilePath, totalLen, "%s%s%s", home, DEFAULT_SYS_DIR, SMILE_DIR);
//
//   assert(strcmp(SL_GetDefaultDir(), smilePath) == 0);
//   free(smilePath);
//   TEST_Pass("Test_SL_GetDefaultDir_ReturnsPathToSmileDir");
// }
//
// void Test_SL_GetGameDir_FailsPreSetGameDir(void) {
//   assert(!SL_GetGameDir());
//   TEST_Pass("Test_SL_GetGameDir_FailsPreSetGameDir");
// }
//
// void Test_SL_SetGameDir_FailsWithNullPath(void) {
//   // assert(!SL_SetGameDir(NULL));
//   TEST_Pass("Test_SL_SetGameDir_FailsWithNullPath");
// }
//
// void Test_SL_SetGameDir_FailsWithPathChangingToParent(void) {
//   // assert(!SL_SetGameDir("../MockPath"));
//   TEST_Pass("Test_SL_SetGameDir_FailsWithPathChangingToParent");
// }
//
// void Test_SL_SetGameDir_FailsWithInvalidPath2(void) {
//   // assert(!SL_SetGameDir("MockPath"));
//   TEST_Pass("Test_SL_SetGameDir_FailsWithInvalidPath2");
// }
//
// void Test_SL_SetGameDir_FailsWithPathLongerThanPathMax(void) {
//   // assert(!SL_SetGameDir(NULL));
//   TEST_Pass("Test_SL_SetGameDir_FailsWithPathLongerThanPathMax");
// }
//
// void Test_SL_DirExists_FailsForNonExistingPath(void) {
//   // assert(false)
//   TEST_Pass("Test_SL_DirExists_FailsForNonExistingPath");
// }
//
// void Test_SL_DirExists_FailsForPathLongerThanPathMax(void) {
//   // assert(false)
//   TEST_Pass("Test_SL_DirExists_FailsForPathLongerThanPathMax");
// }
//
// // -- Unsanitized Path ------------------------------
//
// void Test_SL_SetGameDir_SucceedsWithNonEmptyUnsanitizedValidPath(void) {
//   // assert(!SL_SetGameDir("ValidPath"));
//   TEST_Pass("Test_SL_SetGameDir_SucceedsWithNonEmptyUnsanitizedValidPath");
// }
//
// void Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptyUnsanitizedDefaultDir(void) {
//   // assert(strcmp(SL_GetGameDir(), "some name") == 0);
//   TEST_Pass("Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptyDefaultDir");
// }
//
// void Test_SL_DirExists_SucceedsForExistingUnsanitizedPath(void) {
//   // assert(false)
//   TEST_Pass("Test_SL_DirExists_SucceedsForExistingPath");
// }
//
// // -- Sanitized Path --------------------------------
//
// void Test_SL_SetGameDir_SucceedsWithNonEmptySanitizedValidPath(void) {
//   // assert(!SL_SetGameDir("ValidPath"));
//   TEST_Pass("Test_SL_SetGameDir_SucceedsWithNonEmptySanitizedValidPath");
// }
//
// void Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptySanitizedDefaultDir(void) {
//   // assert(strcmp(SL_GetGameDir(), "some name") == 0);
//   TEST_Pass("Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptySanitizedDefaultDir");
// }
//
// void Test_SL_DirExists_SucceedsForExistingSanitizedPath(void) {
//   // assert(false)
//   TEST_Pass("Test_SL_DirExists_SucceedsForExistingSanitizedPath");
// }
//
// // -- Empty Path ------------------------------------
//
// void Test_SL_SetGameDir_SucceedsWithEmptyPath(void) {
//   // assert(!SL_SetGameDir(""));
//   TEST_Pass("Test_SL_SetGameDir_SucceedsWithValidPath");
// }
//
// void Test_SL_GetGameDir_ReturnsCorrectPathWithEmptyDefaultDir(void) {
//   // assert(strcmp(SL_GetGameDir(), "some name") == 0);
//   TEST_Pass("Test_SL_GetGameDir_ReturnsCorrectPathWithEmptyDefaultDir");
// }
//
// void Test_SL_DirExists_SucceedsForEmptyPathIfEmptyDefaultDir(void) {
//   // assert(false)
//   TEST_Pass("Test_SL_DirExists_SucceedsWithEmptyPathIfEmptyDefaultDir");
// }
//
// // Game File ----------------------------------------
//
// // GetGameFile fails
// // SetGameFile fails with invalid path
// // // Perhaps test this more than once
// // SetGameFile succeeds
// // GetGameFile succeeds
// // must also test FileExists
//
// // Test set game dir after full path is set and check if correct
// // Test set game file after full path is set and check if correct
// // // perhaps do the same again in another order
//
// // must also create a TEST_Func to create/delete dirs and files
// // // Or call delete dir on all once tests are over
//
// // Also test alternate dir if default dir fails
//
//
// // --------------------------------------------------
// // Shutdown
// // --------------------------------------------------
//
// void Test_SL_Shutdown_SucceedsAfterInit(void) {
//   assert(SL_Shutdown());
//   TEST_Pass("Test_SL_Shutdown_SucceedsAfterInit");
// }
//
// // --------------------------------------------------
// // Post-Shutdown
// // --------------------------------------------------
//
// void Test_SL_Shutdown_FailsPostShutdown(void) {
//   assert(!SL_Shutdown());
//   TEST_Pass("Test_SL_Shutdown_FailsIfCalledTwice");
// }
//
// void Test_SL_IsInitialized_FailsPostShutdown(void) {
//   assert(!SL_IsInitialized());
//   TEST_Pass("Test_SL_IsInitialized_FailsAfterShutdown");
// }
//
// // --------------------------------------------------
// // Init With
// // --------------------------------------------------
//
// // Test init with, get dir, get file... all getters, basically
//
// // --------------------------------------------------
// // Save
// // --------------------------------------------------
//
// // --------------------------------------------------
// // Load
// // --------------------------------------------------
//
// // --------------------------------------------------
// // Delete
// // --------------------------------------------------
//
// // --------------------------------------------------
// // Stress tests
// // --------------------------------------------------
//
// // --------------------------------------------------
// // Finger's crossed!
// // --------------------------------------------------
//
// int main() {
//   puts("\nTESTING PRE-INIT");
//   puts("• Init Related");
//   Test_SL_IsInitialized_FailsPreInit();
//   puts("• Game Dir");
//   Test_SL_GetGameDir_FailsPreInit();
//   Test_SL_GetDefaultDir_FailsPreInit();
//   Test_SL_DirExists_FailsPreInit();
//   Test_SL_SetGameDir_FailsPreInit();
//   puts("• Game File");
//   Test_SL_GetGameFile_FailsPreInit();
//   Test_SL_GetGamePath_FailsPreInit();
//   Test_SL_SetGameFile_FailsPreInit();
//   Test_SL_FileExists_FailsPreInit();
//   puts("• Save");
//   Test_SL_BeginSaveSession_FailsPreInit();
//   Test_SL_SaveNext_FailsPreInit();
//   Test_SL_EndSaveSession_FailsPreInit();
//   puts("• Load");
//   Test_SL_BeginLoadSession_FailsPreInit();
//   Test_SL_HasNext_FailsPreInit();
//   Test_SL_LoadNext_FailsPreInit();
//   Test_SL_LoadNextTo_FailsPreInit();
//   Test_SL_EndLoadSession_FailsPreInit();
//   puts("• Delete");
//   Test_SL_DeleteDir_FailsPreInit();
//   Test_SL_DeleteFile_FailsPreInit();
//   puts("• Shutdown");
//   Test_SL_Shutdown_FailsPreInit();
//
//   puts("\nTESTING INIT");
//   puts("• Mem Alloc Fail");
//   Test_SL_Init_FailsIfCallocFails();
//   Test_SL_Init_FailsIfMallocFails();
//   puts("• Success");
//   Test_SL_Init_Succeeds();
//
//   puts("\nTESTING POST-INIT");
//   puts("• Init Related");
//   // Test_SL_Init_FailsPostInit();
//   Test_SL_IsInitialized_SucceedsPostInit();
//   puts("• Game Dir");
//   Test_SL_GetDefaultDir_ReturnsPathToSmileDir();
//   Test_SL_GetGameDir_FailsPreSetGameDir();
//   Test_SL_SetGameDir_FailsWithNullPath();
//   Test_SL_SetGameDir_FailsWithPathChangingToParent();
//   Test_SL_SetGameDir_FailsWithInvalidPath2();
//   Test_SL_SetGameDir_FailsWithPathLongerThanPathMax();
//   Test_SL_DirExists_FailsForNonExistingPath();
//   Test_SL_DirExists_FailsForPathLongerThanPathMax();
//   puts("  • Unsanitized Path");
//   Test_SL_SetGameDir_SucceedsWithNonEmptyUnsanitizedValidPath();
//   Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptyUnsanitizedDefaultDir();
//   Test_SL_DirExists_SucceedsForExistingUnsanitizedPath();
//   puts("  • Sanitized Path");
//   Test_SL_SetGameDir_SucceedsWithNonEmptySanitizedValidPath();
//   Test_SL_GetGameDir_ReturnsCorrectPathWithNonEmptySanitizedDefaultDir();
//   Test_SL_DirExists_SucceedsForExistingSanitizedPath();
//   puts("  • Empty Path");
//   Test_SL_SetGameDir_SucceedsWithEmptyPath();
//   Test_SL_GetGameDir_ReturnsCorrectPathWithEmptyDefaultDir();
//   Test_SL_DirExists_SucceedsForEmptyPathIfEmptyDefaultDir();
//   puts("• Game File");
//
//   puts("\nTESTING SHUTDOWN");
//   Test_SL_Shutdown_SucceedsAfterInit();
//
//   puts("\nTESTING POST-SHUTDOWN");
//   Test_SL_Shutdown_FailsPostShutdown();
//   Test_SL_IsInitialized_FailsPostShutdown();
//
//   puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
//   return 0;
// }

int main() {
}
