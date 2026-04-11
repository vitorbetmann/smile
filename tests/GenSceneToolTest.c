/**
 * @file
 * @brief Implementation of the GenScene Tool Tests.
 *
 * @author Vitor Betmann
 */


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CommonInternal.h"
#include "GenSceneInternal.h"
#include "Log.h"
#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#ifdef NDEBUG
#error "GenSceneToolTest must be compiled without NDEBUG (asserts required)."
#endif


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static bool mockPromptYes(const char *prompt)
{
    (void)prompt;
    return true;
}

static bool fileContains(const char *path, const char *substr)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        return false;
    }

    char buf[4096];
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    fclose(f);
    buf[n] = '\0';
    return strstr(buf, substr) != nullptr;
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tests - Name Sanitization
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Test_gsInternalSanitizeName_SucceedsWithSimpleName(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "MyScene") == CM_RESULT_OK);
    assert(strcmp(buf, "MyScene") == 0);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsConvertsSpacesToCamelCase(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "my scene") == CM_RESULT_OK);
    assert(strcmp(buf, "myScene") == 0);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsWithUnderscoreStart(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "_myScene") == CM_RESULT_OK);
    assert(strcmp(buf, "_myScene") == 0);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsTrimmingLeadingSpaces(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "  MyScene") == CM_RESULT_OK);
    assert(strcmp(buf, "MyScene") == 0);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsTrimmingTrailingSpaces(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "MyScene  ") == CM_RESULT_OK);
    assert(strcmp(buf, "MyScene") == 0);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithNullBuf(void)
{
    assert(gsInternalSanitizeName(nullptr, GS_NAME_MAX, "MyScene") == CM_RESULT_NULL_ARG);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithNullName(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, nullptr) == CM_RESULT_NULL_ARG);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithEmptyName(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "") == CM_RESULT_EMPTY_ARG);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithWhitespaceOnly(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "   ") == CM_RESULT_EMPTY_ARG);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithDigitStart(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "1MyScene") == CM_RESULT_INVALID_NAME);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithInvalidChar(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "My!Scene") == CM_RESULT_INVALID_NAME);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWhenOutputTooLong(void)
{
    char buf[3];
    assert(gsInternalSanitizeName(buf, 3, "MyScene") == CM_RESULT_INVALID_NAME);
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tests - Error Path
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Test_gsInternalRun_FailsWithNoArgs(void)
{
    char *argv[] = {"GenScene"};
    assert(gsInternalRun(1, argv) == CM_RESULT_EMPTY_ARG);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_SucceedsWithHelpFlag(void)
{
    char *argv[] = {"GenScene", "--help"};
    assert(gsInternalRun(2, argv) == 0);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_SucceedsWithShortHelpFlag(void)
{
    char *argv[] = {"GenScene", "-h"};
    assert(gsInternalRun(2, argv) == 0);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWithNameStartingWithDash(void)
{
    char *argv[] = {"GenScene", "-NotAScene"};
    assert(gsInternalRun(2, argv) == CM_RESULT_INVALID_NAME);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWithInvalidFlag(void)
{
    char *argv[] = {"GenScene", "MyScene", "-z"};
    assert(gsInternalRun(3, argv) == GS_RESULT_INVALID_FLAG);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWithNoCallbacks(void)
{
    char *argv[] = {"GenScene", "MyScene", "-ne", "-nu", "-nd", "-nx"};
    assert(gsInternalRun(6, argv) == GS_RESULT_NO_CALLBACKS);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWithMissingSourceInArg(void)
{
    char *argv[] = {"GenScene", "MyScene", "-si"};
    assert(gsInternalRun(3, argv) == CM_RESULT_EMPTY_ARG);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWithMissingHeaderInArg(void)
{
    char *argv[] = {"GenScene", "MyScene", "-hi"};
    assert(gsInternalRun(3, argv) == CM_RESULT_EMPTY_ARG);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWithInvalidSourcePath(void)
{
    char *argv[] = {"GenScene", "MyScene", "-si", "../bad"};
    assert(gsInternalRun(4, argv) == CM_RESULT_INVALID_PATH);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWithInvalidHeaderPath(void)
{
    char *argv[] = {"GenScene", "MyScene", "-hi", "../bad"};
    assert(gsInternalRun(4, argv) == CM_RESULT_INVALID_PATH);
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tests - File Generation
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Test_gsInternalRun_SucceedsAndCreatesBothFiles(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == CM_RESULT_OK);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    assert(cmInternalFileExists(srcPath));
    assert(cmInternalFileExists(incPath));

    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcContainsAllCallbacks(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == CM_RESULT_OK);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    assert(fileContains(srcPath, "Enter"));
    assert(fileContains(srcPath, "Update"));
    assert(fileContains(srcPath, "Draw"));
    assert(fileContains(srcPath, "Exit"));

    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcOmitsDisabledCallbacks(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-ne", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(7, argv) == CM_RESULT_OK);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    assert(!fileContains(srcPath, "Enter"));
    assert(fileContains(srcPath, "Update"));

    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWhenSrcFileCannotBeCreated(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    assert(lgSetFatal(gsInternalFatalHandler) == CM_RESULT_OK);
    tsInternalDisable(FOPEN, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == CM_RESULT_FAIL_TO_CREATE_FILE);
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);

    rmdir(srcDir);
    rmdir(incDir);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWhenIncFileCannotBeCreated(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    assert(lgSetFatal(gsInternalFatalHandler) == CM_RESULT_OK);
    tsInternalDisable(FOPEN, 2);
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == CM_RESULT_FAIL_TO_CREATE_FILE);
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);

    char srcPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    remove(srcPath);
    rmdir(srcDir);
    rmdir(incDir);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWhenSrcDirCannotBeCreated(void)
{
    assert(lgSetFatal(gsInternalFatalHandler) == CM_RESULT_OK);
    gsTestPrompt = mockPromptYes;
    tsInternalDisable(MKDIR, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", "gstest_nonexistent_src", "-hi", "gstest_nonexistent_inc"};
    assert(gsInternalRun(6, argv) == CM_RESULT_FAIL_TO_CREATE_DIR);
    gsTestPrompt = nullptr;
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWhenIncDirCannotBeCreated(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);

    assert(lgSetFatal(gsInternalFatalHandler) == CM_RESULT_OK);
    gsTestPrompt = mockPromptYes;
    tsInternalDisable(MKDIR, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", "gstest_nonexistent_inc"};
    assert(gsInternalRun(6, argv) == CM_RESULT_FAIL_TO_CREATE_DIR);
    gsTestPrompt = nullptr;
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);

    rmdir(srcDir);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_GeneratedHeaderHasCorrectIncludeGuard(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == CM_RESULT_OK);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    assert(fileContains(incPath, "#ifndef TESTSCENE_H"));

    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Main
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int main(void)
{
    puts("\nNAME SANITIZATION TESTING");
    Test_gsInternalSanitizeName_SucceedsWithSimpleName();
    Test_gsInternalSanitizeName_SucceedsConvertsSpacesToCamelCase();
    Test_gsInternalSanitizeName_SucceedsWithUnderscoreStart();
    Test_gsInternalSanitizeName_SucceedsTrimmingLeadingSpaces();
    Test_gsInternalSanitizeName_SucceedsTrimmingTrailingSpaces();
    Test_gsInternalSanitizeName_FailsWithNullBuf();
    Test_gsInternalSanitizeName_FailsWithNullName();
    Test_gsInternalSanitizeName_FailsWithEmptyName();
    Test_gsInternalSanitizeName_FailsWithWhitespaceOnly();
    Test_gsInternalSanitizeName_FailsWithDigitStart();
    Test_gsInternalSanitizeName_FailsWithInvalidChar();
    Test_gsInternalSanitizeName_FailsWhenOutputTooLong();
    puts("\nERROR PATH TESTING");
    Test_gsInternalRun_FailsWithNoArgs();
    Test_gsInternalRun_SucceedsWithHelpFlag();
    Test_gsInternalRun_SucceedsWithShortHelpFlag();
    Test_gsInternalRun_FailsWithNameStartingWithDash();
    Test_gsInternalRun_FailsWithInvalidFlag();
    Test_gsInternalRun_FailsWithNoCallbacks();
    Test_gsInternalRun_FailsWithMissingSourceInArg();
    Test_gsInternalRun_FailsWithMissingHeaderInArg();
    Test_gsInternalRun_FailsWithInvalidSourcePath();
    Test_gsInternalRun_FailsWithInvalidHeaderPath();
    puts("\nFILE GENERATION TESTING");
    Test_gsInternalRun_FailsWhenSrcFileCannotBeCreated();
    Test_gsInternalRun_FailsWhenIncFileCannotBeCreated();
    Test_gsInternalRun_FailsWhenSrcDirCannotBeCreated();
    Test_gsInternalRun_FailsWhenIncDirCannotBeCreated();
    Test_gsInternalRun_SucceedsAndCreatesBothFiles();
    Test_gsInternalRun_GeneratedSrcContainsAllCallbacks();
    Test_gsInternalRun_GeneratedSrcOmitsDisabledCallbacks();
    Test_gsInternalRun_GeneratedHeaderHasCorrectIncludeGuard();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
    return 0;
}
