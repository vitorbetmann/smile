/**
 * @file
 * @brief Implementation of the Log API Tests.
 *
 * @author Vitor Betmann
 */


// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <assert.h>
#include <stdio.h>

#include "Log.h"
#include "LogInternal.h"
#include "CommonInternal.h"
#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#ifdef NDEBUG
#error "LogAPITest must be compiled without NDEBUG (asserts required)."
#endif


// —————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————

static int fatalHandlerCount;


// —————————————————————————————————————————————————————————————————————————————
// Functions
// —————————————————————————————————————————————————————————————————————————————

static void mockFatalHandler(void)
{
    fatalHandlerCount++;
}


// —————————————————————————————————————————————————————————————————————————————
// Tests
// —————————————————————————————————————————————————————————————————————————————

void Test_lgLog_SucceedsWithValidMessage(void)
{
    assert(lgLog("Hello, %s!", "Smile") == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_lgLog_FailsWithNullMessage(void)
{
    assert(lgLog(nullptr) == CM_RESULT_NULL_ARG);
    tsInternalPass(__func__);
}

void Test_lgSetFatal_SucceedsWithNullHandler(void)
{
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_lgInternalLog_FatalInvokesCustomHandler(void)
{
    fatalHandlerCount = 0;
    assert(lgSetFatal(mockFatalHandler) == CM_RESULT_OK);
    assert(
        lgInternalLog(FATAL, "LogAPITest", "Fatal path exercised", __func__,
            "for verification") == CM_RESULT_OK);
    assert(fatalHandlerCount == 1);
    assert(lgSetFatal(nullptr) == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_lgInternalLog_InfoReturnsSuccess(void)
{
    assert(
        lgInternalLog(INFO, "LogAPITest", "Info path exercised", __func__,
            "for verification") == CM_RESULT_OK);
    tsInternalPass(__func__);
}

void Test_lgInternalLog_WarningReturnsSuccess(void)
{
    assert(
        lgInternalLog(WARNING, "LogAPITest", "Warning path exercised",
            __func__, "for verification") == CM_RESULT_OK);
    tsInternalPass(__func__);
}


// —————————————————————————————————————————————————————————————————————————————
// Main
// —————————————————————————————————————————————————————————————————————————————

int main(void)
{
    puts("\nLOG API TESTING");
    Test_lgLog_SucceedsWithValidMessage();
    Test_lgLog_FailsWithNullMessage();
    Test_lgSetFatal_SucceedsWithNullHandler();
    Test_lgInternalLog_FatalInvokesCustomHandler();
    Test_lgInternalLog_InfoReturnsSuccess();
    Test_lgInternalLog_WarningReturnsSuccess();

    puts("\nTIME TO SMILE! :)\n\tAll Tests Passed!");
    return 0;
}
