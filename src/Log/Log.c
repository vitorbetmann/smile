/**
 * @file
 * @brief Implementation of the StateMachine module.
 *
 * @see StateMachine.h
 * @see StateMachineInternal.h
 * @see StateMachineMessages.h
 *
 * @bug No known bugs.
 *
 * @note TODO #20 [Feature] for [Log] - Add file logging support for standard
 *       and fatal logs
 * @note TODO #21 [Feature] for [Log] - Add runtime and module-specific log
 *       filtering
 *
 * @author Vitor Betmann
 * @date 2025-10-29
 * @version 1.0.0
 */

// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/Log.h"
#include "LogInternal.h"

// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#define SMILE_CYAN "\033[36m"
#define SMILE_YELLOW "\033[33m"
#define SMILE_RED "\033[31m"
#define SMILE_PURPLE "\033[0;35m"
#define SMILE_GREEN "\033[32m"
#define SMILE_WHITE "\033[0m"

#define LOG_TIME_FMT "%H:%M:%S"
#define LOG_TIME_BUFFER_LEN 32

// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

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
static void lgPrivateLog(InternalLevel level, const char *origin,
                         const char *msg, ...);

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
static void lgPrivateLogV(InternalLevel level, const char *origin,
                          const char *msg, va_list args);

/**
 * @brief Determines if logging is enabled for a given level.
 *
 * @param level Severity level to check.
 * @return true if logging is enabled for the level, false otherwise.
 *
 * @author Vitor Betmann
 */
static bool lgPrivateIsLevelEnabled(InternalLevel level);

/**
 * @brief Determines the color and prefix for a given log level.
 *
 * @param level Severity level of the log message.
 * @param color Output pointer to the ANSI color string for the level.
 * @param prefix Output pointer to the prefix string for the level.
 *
 * @author Vitor Betmann
 */
static void lgPrivateGetColorAndPrefix(InternalLevel level, const char **color,
                                       const char **prefix);

/**
 * @brief Default handler for fatal log events.
 *
 * Called when a log with LOG_FATAL level is issued and no custom fatal handler
 * is set. Terminates the program with failure status.
 *
 * @author Vitor Betmann
 */
static void lgPrivateFatalHandler(void);

// —————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————

lgFatalHandler fatalHandler = lgPrivateFatalHandler;

// —————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————

void lgLog(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    lgPrivateLogV(USER, "User", msg, args);
    va_end(args);
}

void lgSetFatal(lgFatalHandler handler)
{
    if (!handler)
    {
        fatalHandler = lgPrivateFatalHandler;
        return;
    }

    fatalHandler = handler;
}

// —————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————

void lgInternalLog(InternalLevel level, const char *module, const char *cause,
                   const char *fnName, const char *conseq)
{
    lgPrivateLog(level, module, "%s. '%s' %s.", cause, fnName, conseq);
}

void lgInternalLogWithArg(InternalLevel level, const char *module,
                          const char *cause, const char *arg,
                          const char *fnName, const char *conseq)
{
    lgPrivateLog(level, module, "%s: %s. '%s' %s.", cause, arg, fnName, conseq);
}

// —————————————————————————————————————————————————————————————————————————————
// Functions - Private
// —————————————————————————————————————————————————————————————————————————————

static void lgPrivateLog(InternalLevel level, const char *origin,
                         const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    lgPrivateLogV(level, origin, msg, args);
    va_end(args);
}

void lgPrivateLogV(InternalLevel level, const char *origin, const char *msg,
                   va_list args)
{
    if (!lgPrivateIsLevelEnabled(level))
    {
        return;
    }

    const char *color = nullptr;
    const char *prefix = nullptr;
    lgPrivateGetColorAndPrefix(level, &color, &prefix);

    const time_t EPOCH_TIME = time(nullptr);
    struct tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &EPOCH_TIME);
#else
    localtime_r(&EPOCH_TIME, &localTime);
#endif
    char timeBuf[LOG_TIME_BUFFER_LEN];
    strftime(timeBuf, sizeof(timeBuf), LOG_TIME_FMT, &localTime);

    fprintf(stderr, "%s%s [Smile %s From %s] - ", color, timeBuf, prefix,
            origin);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", SMILE_WHITE); // Reset color

    if (level == FATAL)
    {
        fatalHandler();
    }
}

static bool lgPrivateIsLevelEnabled(InternalLevel level)
{
    switch (level)
    {
        case INFO:
#ifdef SMILE_LOG_INFO
            return true;
#else
            return false;
#endif
        case WARNING:
#ifdef SMILE_LOG_WARNING
            return true;
#else
            return false;
#endif
        default:
            return true;
    }
}

static void lgPrivateGetColorAndPrefix(InternalLevel level, const char **color,
                                       const char **prefix)
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
        case WARNING:
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
