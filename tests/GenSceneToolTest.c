/**
 * @file
 * @brief Implementation of the GenScene Tool Tests.
 *
 * @author Vitor Betmann
 */


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// External
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Module Related
#include "GenSceneInternal.h"
// Support
#include "CommonInternal.h"
#include "Log.h"
#include "Test.h"


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
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "MyScene") == RES_OK);
    assert(strcmp(buf, "MyScene") == 0);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsConvertsSpacesToCamelCase(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "my scene") == RES_OK);
    assert(strcmp(buf, "myScene") == 0);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsWithUnderscoreStart(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "_myScene") == RES_OK);
    assert(strcmp(buf, "_myScene") == 0);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsTrimmingLeadingSpaces(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "  MyScene") == RES_OK);
    assert(strcmp(buf, "MyScene") == 0);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsTrimmingTrailingSpaces(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "MyScene  ") == RES_OK);
    assert(strcmp(buf, "MyScene") == 0);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithNullBuf(void)
{
    assert(gsInternalSanitizeName(nullptr, GS_NAME_MAX, "MyScene") == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithNullName(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, nullptr) == RES_NULL_ARG);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithEmptyName(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "") == RES_EMPTY_ARG);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithWhitespaceOnly(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "   ") == RES_EMPTY_ARG);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithDigitStart(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "1MyScene") == RES_INVALID_ARG);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWithInvalidChar(void)
{
    char buf[GS_NAME_MAX];
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, "My!Scene") == RES_INVALID_ARG);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsWhenOutputTooLong(void)
{
    char buf[3];
    assert(gsInternalSanitizeName(buf, 3, "MyScene") == RES_INVALID_ARG);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_SucceedsAtMaxNameLength(void)
{
    char buf[GS_NAME_MAX];
    char name[GS_NAME_MAX];
    memset(name, 'A', GS_NAME_MAX - 1);
    name[GS_NAME_MAX - 1] = '\0';
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, name) == RES_OK);
    tsPass(__func__);
}

void Test_gsInternalSanitizeName_FailsExceedingMaxNameLength(void)
{
    char buf[GS_NAME_MAX];
    char name[GS_NAME_MAX + 1];
    memset(name, 'A', GS_NAME_MAX);
    name[GS_NAME_MAX] = '\0';
    assert(gsInternalSanitizeName(buf, GS_NAME_MAX, name) == RES_INVALID_ARG);
    tsPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tests - Error Path
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Test_gsInternalRun_FailsWithNoArgs(void)
{
    char *argv[] = {"GenScene"};
    assert(gsInternalRun(1, argv) == RES_EMPTY_ARG);
    tsPass(__func__);
}

void Test_gsInternalRun_SucceedsWithHelpFlag(void)
{
    char *argv[] = {"GenScene", "--help"};
    assert(gsInternalRun(2, argv) == RES_OK);
    tsPass(__func__);
}

void Test_gsInternalRun_SucceedsWithShortHelpFlag(void)
{
    char *argv[] = {"GenScene", "-h"};
    assert(gsInternalRun(2, argv) == RES_OK);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWithNameStartingWithDash(void)
{
    char *argv[] = {"GenScene", "-NotAScene"};
    assert(gsInternalRun(2, argv) == RES_INVALID_ARG);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWithInvalidFlag(void)
{
    char *argv[] = {"GenScene", "MyScene", "-z"};
    assert(gsInternalRun(3, argv) == RES_INVALID_FLAG);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWithNoCallbacks(void)
{
    char *argv[] = {"GenScene", "MyScene", "-ne", "-nu", "-nd", "-nx"};
    assert(gsInternalRun(6, argv) == RES_NO_CALLBACKS);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWithMissingSourceInArg(void)
{
    char *argv[] = {"GenScene", "MyScene", "-si"};
    assert(gsInternalRun(3, argv) == RES_EMPTY_ARG);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWithMissingHeaderInArg(void)
{
    char *argv[] = {"GenScene", "MyScene", "-hi"};
    assert(gsInternalRun(3, argv) == RES_EMPTY_ARG);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWithInvalidSourcePath(void)
{
    char *argv[] = {"GenScene", "MyScene", "-si", "../bad"};
    assert(gsInternalRun(4, argv) == RES_INVALID_PATH);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWithInvalidHeaderPath(void)
{
    char *argv[] = {"GenScene", "MyScene", "-hi", "../bad"};
    assert(gsInternalRun(4, argv) == RES_INVALID_PATH);
    tsPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tests - File Generation
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Test_gsInternalRun_SucceedsAndCreatesBothFiles(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == RES_OK);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    assert(cmFileExists(srcPath));
    assert(cmFileExists(incPath));

    remove(srcPath);
    remove(incPath);
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    tsPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcContainsAllCallbacks(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == RES_OK);

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
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    tsPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcOmitsDisabledCallbacks(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-ne", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(7, argv) == RES_OK);

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
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWhenSrcFileCannotBeCreated(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    tsReset();
    assert(lgSetFatal(gsInternalFatalHandler) == RES_OK);
    tsDisable(FOPEN, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);
    assert(lgSetFatal(nullptr) == RES_OK);

    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    assert(result == RES_CREATE_FILE_FAIL);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWhenIncFileCannotBeCreated(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    tsReset();
    assert(lgSetFatal(gsInternalFatalHandler) == RES_OK);
    tsDisable(FOPEN, 2);
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);
    assert(lgSetFatal(nullptr) == RES_OK);

    char srcPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    remove(srcPath);
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    assert(result == RES_CREATE_FILE_FAIL);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWhenSrcDirCannotBeCreated(void)
{
    tsReset();
    assert(lgSetFatal(gsInternalFatalHandler) == RES_OK);
    gsTestUserConfirms = true;
    tsDisable(MKDIR, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", "gstest_nonexistent_src", "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);
    gsTestUserConfirms = false;
    assert(lgSetFatal(nullptr) == RES_OK);
    assert(result == RES_CREATE_DIR_FAIL);
    tsPass(__func__);
}

void Test_gsInternalRun_FailsWhenIncDirCannotBeCreated(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);

    tsReset();
    assert(lgSetFatal(gsInternalFatalHandler) == RES_OK);
    gsTestUserConfirms = true;
    tsDisable(MKDIR, 1);
    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);
    gsTestUserConfirms = false;
    assert(lgSetFatal(nullptr) == RES_OK);

    cmDeleteDir(srcDir);
    assert(result == RES_CREATE_DIR_FAIL);
    tsPass(__func__);
}

void Test_gsInternalRun_GeneratedHeaderHasCorrectIncludeGuard(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    assert(gsInternalRun(6, argv) == RES_OK);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    assert(fileContains(incPath, "#ifndef TESTSCENE_H"));
    assert(fileContains(incPath, "#define TESTSCENE_H"));
    assert(fileContains(incPath, "#endif"));

    remove(srcPath);
    remove(incPath);
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    tsPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tests - Overwrite / Prompt
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Test_gsInternalRun_AbortsWhenUserDeclinesCreateSrcDir(void)
{
    char *argv[] = {"GenScene", "TestScene", "-si", "gstest_nonexistent_src",
                    "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);
    assert(result == RES_USER_ABORT);
    tsPass(__func__);
}

void Test_gsInternalRun_AbortsWhenUserDeclinesCreateIncDir(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", "gstest_nonexistent_inc"};
    int result = gsInternalRun(6, argv);

    cmDeleteDir(srcDir);
    assert(result == RES_USER_ABORT);
    tsPass(__func__);
}

void Test_gsInternalRun_AbortsWhenUserDeclinesOverwriteSrcFile(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char srcPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    FILE *f = fopen(srcPath, "w");
    assert(f != nullptr);
    fclose(f);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);

    remove(srcPath);
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    assert(result == RES_USER_ABORT);
    tsPass(__func__);
}

void Test_gsInternalRun_AbortsWhenUserDeclinesOverwriteIncFile(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char incPath[CM_PATH_MAX];
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);
    FILE *f = fopen(incPath, "w");
    assert(f != nullptr);
    fclose(f);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);

    remove(incPath);
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    assert(result == RES_USER_ABORT);
    tsPass(__func__);
}

void Test_gsInternalRun_SucceedsWhenUserAcceptsOverwrite(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

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
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);
    assert(result == RES_OK);
    tsPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcHasSectionsWithFlag(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    char *argv[] = {"GenScene", "TestScene", "-as", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(7, argv);

    bool hasSections = fileContains(srcPath, GS_SECTION_DIV);
    remove(srcPath);
    remove(incPath);
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);

    assert(result == RES_OK);
    assert(hasSections);
    tsPass(__func__);
}

void Test_gsInternalRun_GeneratedSrcHasNoSectionsWithoutFlag(void)
{
    char srcDir[] = "gstest_src_XXXXXX";
    char incDir[] = "gstest_inc_XXXXXX";
    assert(tsMkdtemp(srcDir) != nullptr);
    assert(tsMkdtemp(incDir) != nullptr);

    char srcPath[CM_PATH_MAX];
    char incPath[CM_PATH_MAX];
    snprintf(srcPath, sizeof(srcPath), "%s/TestScene.c", srcDir);
    snprintf(incPath, sizeof(incPath), "%s/TestScene.h", incDir);

    char *argv[] = {"GenScene", "TestScene", "-si", srcDir, "-hi", incDir};
    int result = gsInternalRun(6, argv);

    bool noSections = !fileContains(srcPath, GS_SECTION_DIV);
    remove(srcPath);
    remove(incPath);
    cmDeleteDir(srcDir);
    cmDeleteDir(incDir);

    assert(result == RES_OK);
    assert(noSections);
    tsPass(__func__);
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
