/**
 * @file
 * @brief Implementation of the Log module.
 *
 * @see Log.h
 * @see LogInternal.h
 *
 * @note TODO #20 [Feature] for [Log] - Add file logging support for standard
 *       and fatal logs
 * @note TODO #21 [Feature] for [Log] - Add runtime and module-specific log
 *       filtering
 *
 * @author Vitor Betmann
 */

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// External
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Module Related
#include "Log.h"
#include "LogInternal.h"
// Support
#include "internal/Common/Common.h"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Outputs messages with a specific log level and origin.
 *
 * Wraps variable argument handling for internal logging functions.
 *
 * @param level Severity level of the log message.
 * @param origin Origin or module name for the log message.
 * @param msg Format string for the message to log.
 * @param ... Additional arguments for the format string.
 *
 * @author Vitor Betmann
 */
static int lgPrivateLog(lgInternalLevel level, const char *origin, const char *msg, ...);

/**
 * @brief Outputs formatted messages.
 *
 * Handles timestamp formatting, color coding, log prefixes, and log level
 * filtering.
 *
 * @param level Severity level of the log message.
 * @param origin Origin or module name for the log message.
 * @param msg Format string for the message to log.
 * @param args Variable argument list corresponding to the format string.
 *
 * @author Vitor Betmann
 */
static int lgPrivateLogV(lgInternalLevel level, const char *origin, const char *msg, va_list args);

/**
 * @brief Determines if logging is enabled for a given level.
 *
 * @param level Severity level to check.
 * @return true if logging is enabled for the level, false otherwise.
 *
 * @author Vitor Betmann
 */
static bool lgPrivateIsLevelEnabled(lgInternalLevel level);

/**
 * @brief Determines the color and prefix for a given log level.
 *
 * @param level Severity level of the log message.
 * @param color Output pointer to the ANSI color string for the level.
 * @param prefix Output pointer to the prefix string for the level.
 *
 * @author Vitor Betmann
 */
static void lgPrivateGetColorAndPrefix(lgInternalLevel level, const char **color, const char **prefix);

/**
 * @brief Default handler for fatal log events.
 *
 * Called when a log with FATAL level is issued and no custom fatal handler
 * is set. Terminates the program with failure status.
 *
 * @author Vitor Betmann
 */
static void lgPrivateFatalHandler(void);

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static lgFatalHandler fatalHandler = lgPrivateFatalHandler;

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int lgLog(const char *msg, ...)
{
    if (!msg)
    {
        return RES_NULL_ARG;
    }

    va_list args;
    va_start(args, msg);
    int result = lgPrivateLogV(USER, "User", msg, args);
    va_end(args);
    return result;
}

int lgSetFatal(lgFatalHandler handler)
{
    if (!handler)
    {
        fatalHandler = lgPrivateFatalHandler;
    }

    fatalHandler = handler;
    return RES_OK;
}

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int lgInternalLog(lgInternalLevel level, const char *origin, const char *cse, const char *fnName, const char *csq)
{
    if (!origin || !cse || !fnName || !csq)
    {
        return RES_NULL_ARG;
    }

    return lgPrivateLog(level, origin, "%s. '%s' %s.", cse, fnName, csq);
}

int lgInternalLogWithArg(lgInternalLevel lvl, const char *origin, const char *cause, const char *arg,
                         const char *fnName, const char *csq)
{
    if (!origin || !cause || !arg || !fnName || !csq)
    {
        return RES_NULL_ARG;
    }

    return lgPrivateLog(lvl, origin, "%s: %s. '%s' %s.", cause, arg, fnName,
                        csq);
}

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Private
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static int lgPrivateLog(lgInternalLevel level, const char *origin, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    int result = lgPrivateLogV(level, origin, msg, args);
    va_end(args);
    return result;
}

static int lgPrivateLogV(lgInternalLevel level, const char *origin, const char *msg, va_list args)
{
    if (!lgPrivateIsLevelEnabled(level))
    {
        return RES_OK;
    }

    const char *color = nullptr;
    const char *prefix = nullptr;
    lgPrivateGetColorAndPrefix(level, &color, &prefix);

    char timeBuf[LOG_TIME_BUFFER_LEN] = "00:00:00";
    const time_t epochTime = time(nullptr);
    struct tm localTime = {0};
    bool hasLocalTime = false;

    if (epochTime != (time_t)-1)
    {
#ifdef _WIN32
        hasLocalTime = localtime_s(&localTime, &epochTime) == 0;
#else
        hasLocalTime = (localtime_r(&epochTime, &localTime) != nullptr);
#endif
    }
    if (!hasLocalTime)
    {
        if (level == FATAL)
        {
            fatalHandler();
        }
        return RES_TIME_FAIL;
    }

    if (strftime(timeBuf, sizeof(timeBuf), LOG_TIME_FMT, &localTime) == 0)
    {
        if (level == FATAL)
        {
            fatalHandler();
        }
        return RES_TIME_FAIL;
    }

    int prefixStatus = fprintf(stderr, "%s%s [Smile %s From %s] - ", color, timeBuf, prefix, origin);
    int messageStatus = vfprintf(stderr, msg, args);
    int suffixStatus = fprintf(stderr, "%s\n", SMILE_WHITE); // Reset colour
    int flushStatus = 0;
    if (level == ERROR || level == FATAL)
    {
        flushStatus = fflush(stderr);
    }

    if (prefixStatus < 0 || messageStatus < 0 || suffixStatus < 0 ||
        flushStatus == EOF)
    {
        if (level == FATAL)
        {
            fatalHandler();
        }
        return RES_WRITE_FAIL;
    }

    if (level == FATAL)
    {
        fatalHandler();
    }
    return RES_OK;
}

static bool lgPrivateIsLevelEnabled(lgInternalLevel level)
{
    switch (level)
    {
    case INFO:
#ifdef SMILE_INFO
        return true;
#else
        return false;
#endif
    case WARN:
#ifdef SMILE_WARN
        return true;
#else
        return false;
#endif
    default:
        return true;
    }
}

static void lgPrivateGetColorAndPrefix(lgInternalLevel level, const char **color, const char **prefix)
{
    switch (level)
    {
    case USER:
        *color = SMILE_GREEN;
        *prefix = "Log";
        return;
    case INFO:
        *color = SMILE_CYAN;
        *prefix = "Info";
        return;
    case WARN:
        *color = SMILE_YELLOW;
        *prefix = "Warning";
        return;
    case ERROR:
        *color = SMILE_RED;
        *prefix = "Error";
        return;
    case FATAL:
        *color = SMILE_PURPLE;
        *prefix = "Fatal";
        return;
    default:
        *color = SMILE_WHITE;
        *prefix = "\"You Shouldn't Be Seeing This!\"";
    }
}

static void lgPrivateFatalHandler(void)
{
    exit(EXIT_FAILURE);
}
