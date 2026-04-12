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

static bool fileContains(const char *path, const char *needle)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return false;

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    if (len <= 0)
    {
        fclose(f);
        return false;
    }

    char *buf = malloc((size_t)len + 1);
    if (!buf)
    {
        fclose(f);
        return false;
    }

    size_t nread = fread(buf, 1, (size_t)len, f);
    buf[nread] = '\0';
    fclose(f);

    bool found = strstr(buf, needle) != nullptr;
    free(buf);
    return found;
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

void Test_gsInternalSanitizeName_SucceedsAtMaxNameLength(void)
{
    char buf[GS_NAME_MAX];
    char name[GS_NAME_MAX];
    memset(name, 'A', GS_NAME_MAX - 1);
    name[GS_NAME_MAX - 1] = '\0';
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, name) == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_gsInternalSanitizeName_FailsExceedingMaxNameLength(void)
{
    char buf[GS_NAME_MAX];
    char name[GS_NAME_MAX + 1];
    memset(name, 'A', GS_NAME_MAX);
    name[GS_NAME_MAX] = '\0';
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, name) == CM_RESULT_INVALID_NAME);
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
    assert(gsInternalRun(2, argv) == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_SucceedsWithShortHelpFlag(void)
{
    char *argv[] = {"GenScene", "-h"};
    assert(gsInternalRun(2, argv) == CM_RESULT_OK);
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
    assert(fileContains(srcPath, "Draw"));
    assert(fileContains(srcPath, "Exit"));

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
    int result = gsInternalRun(6, argv);
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);

    rmdir(srcDir);
    rmdir(incDir);
    assert(result == CM_RESULT_FAIL_TO_CREATE_FILE);
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
    int result = gsInternalRun(6, argv);
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);

    char srcPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    remove(srcPath);
    rmdir(srcDir);
    rmdir(incDir);
    assert(result == CM_RESULT_FAIL_TO_CREATE_FILE);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWhenSrcDirCannotBeCreated(void)
{
    assert(lgSetFatal(gsInternalFatalHandler) == CM_RESULT_OK);
    gsTestUserConfirms = true;
    tsInternalDisable(MKDIR, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", "gstest_nonexistent_src", "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);
    gsTestUserConfirms = false;
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);
    assert(result == CM_RESULT_FAIL_TO_CREATE_DIR);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_FailsWhenIncDirCannotBeCreated(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);

    assert(lgSetFatal(gsInternalFatalHandler) == CM_RESULT_OK);
    gsTestUserConfirms = true;
    tsInternalDisable(MKDIR, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);
    gsTestUserConfirms = false;
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);

    rmdir(srcDir);
    assert(result == CM_RESULT_FAIL_TO_CREATE_DIR);
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
    assert(fileContains(incPath, "#define TESTSCENE_H"));
    assert(fileContains(incPath, "#endif"));

    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tests - Overwrite / Prompt
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Test_gsInternalRun_AbortsWhenUserDeclinesCreateSrcDir(void)
{
    char *argv[] = {"GenScene", "TestScene", "-si", "gstest_nonexistent_src",
                    "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);
    assert(result == GS_RESULT_USER_ABORTED);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_AbortsWhenUserDeclinesCreateIncDir(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);

    rmdir(srcDir);
    assert(result == GS_RESULT_USER_ABORTED);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_AbortsWhenUserDeclinesOverwriteSrcFile(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char srcPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    FILE *f = fopen(srcPath, "w");
    assert(f != nullptr);
    fclose(f);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);

    remove(srcPath);
    rmdir(srcDir);
    rmdir(incDir);
    assert(result == GS_RESULT_USER_ABORTED);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_AbortsWhenUserDeclinesOverwriteIncFile(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char incPath[CM_PATH_MAX];
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);
    FILE *f = fopen(incPath, "w");
    assert(f != nullptr);
    fclose(f);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);

    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);
    assert(result == GS_RESULT_USER_ABORTED);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_SucceedsWhenUserAcceptsOverwrite(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);
    FILE *f = fopen(srcPath, "w");
    assert(f != nullptr);
    fclose(f);
    f = fopen(incPath, "w");
    assert(f != nullptr);
    fclose(f);

    gsTestUserConfirms = true;
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);
    gsTestUserConfirms = false;

    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);
    assert(result == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcHasSectionsWithFlag(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    char *argv[] = {"GenScene", "TestScene", "-as", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(7, argv);

    bool hasSections = fileContains(srcPath, GS_SECTION_DIV);
    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);

    assert(result == CM_RESULT_OK);
    assert(hasSections);
    tsInternalPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcHasNoSectionsWithoutFlag(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(mkdtemp(srcDir) != nullptr);
    assert(mkdtemp(incDir) != nullptr);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);

    bool noSections = !fileContains(srcPath, GS_SECTION_DIV);
    remove(srcPath);
    remove(incPath);
    rmdir(srcDir);
    rmdir(incDir);

    assert(result == CM_RESULT_OK);
    assert(noSections);
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
    Test_gsInternalSanitizeName_SucceedsAtMaxNameLength();
    Test_gsInternalSanitizeName_FailsExceedingMaxNameLength();
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
    puts("\nOVERWRITE/PROMPT TESTING");
    Test_gsInternalRun_AbortsWhenUserDeclinesCreateSrcDir();
    Test_gsInternalRun_AbortsWhenUserDeclinesCreateIncDir();
    Test_gsInternalRun_AbortsWhenUserDeclinesOverwriteSrcFile();
    Test_gsInternalRun_AbortsWhenUserDeclinesOverwriteIncFile();
    Test_gsInternalRun_SucceedsWhenUserAcceptsOverwrite();
    Test_gsInternalRun_GeneratedSrcHasSectionsWithFlag();
    Test_gsInternalRun_GeneratedSrcHasNoSectionsWithoutFlag();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
    return 0;
}
