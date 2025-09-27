/**
 * @file LogInternal.c
 * @brief Implementation of the SMILE logging system.
 *
 * @author Vitor Betmann
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/Log.h"
#include "LogInternal.h"

// -----------------------------------------------------------------------------
// Defines
// -----------------------------------------------------------------------------

#define LOG_CYAN "\033[36m"
#define LOG_YELLOW "\033[33m"
#define LOG_RED "\033[31m"
#define LOG_PURPLE "\033[0;35m"
#define LOG_GREEN "\033[32m"
#define LOG_WHITE "\033[0m"

#define LOG_TIME_FMT "%H:%M:%S"
#define LOG_TIME_BUFFER_LEN 32

// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

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
static void lgHelperLog(LogLevel level, const char *origin, const char *msg,
                        ...);

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
static void lgHelperLogV(LogLevel level, const char *origin, const char *msg,
                         va_list args);

/**
 * @brief Determines if logging is enabled for a given level.
 *
 * @param level Severity level to check.
 * @return true if logging is enabled for the level, false otherwise.
 *
 * @author Vitor Betmann
 */
static bool lgHelperIsLevelEnabled(LogLevel level);

/**
 * @brief Determines the color and prefix for a given log level.
 *
 * @param level Severity level of the log message.
 * @param color Output pointer to the ANSI color string for the level.
 * @param prefix Output pointer to the prefix string for the level.
 *
 * @author Vitor Betmann
 */
static void lgHelperGetColorAndPrefix(LogLevel level, const char **color,
                                      const char **prefix);

/**
 * @brief Default handler for fatal log events.
 *
 * Called when a log with LOG_FATAL level is issued and no custom fatal handler
 * is set. Terminates the program with failure status.
 *
 * @author Vitor Betmann
 */
static void lgHelperFatalHandler(void);

// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

lgFatalHandler fatalHandler = lgHelperFatalHandler;

// -----------------------------------------------------------------------------
// Functions - Public
// -----------------------------------------------------------------------------

void lgLog(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    lgHelperLogV(LOG_USER, "User", msg, args);
    va_end(args);
}

void lgSetFatal(lgFatalHandler handler) {
    if (!handler) {
        fatalHandler = lgHelperFatalHandler;
        return;
    }

    fatalHandler = handler;
}

// -----------------------------------------------------------------------------
// Functions - Internal
// -----------------------------------------------------------------------------

void lgInternalLog(LogLevel level, const char *module, const char *cause,
                   const char *fnName, const char *conseq) {
    lgHelperLog(level, module, "%s. '%s' %s.", cause, fnName, conseq);
}

void lgInternalLogWithArg(LogLevel level, const char *module,
                          const char *cause, const char *arg,
                          const char *fnName, const char *conseq) {
    lgHelperLog(level, module, "%s: %s. '%s' %s.", cause, arg, fnName, conseq);
}

// -----------------------------------------------------------------------------
// Functions - Helper
// -----------------------------------------------------------------------------

static void lgHelperLog(LogLevel level, const char *origin, const char *msg,
                        ...) {
    va_list args;
    va_start(args, msg);
    lgHelperLogV(level, origin, msg, args);
    va_end(args);
}

void lgHelperLogV(LogLevel level, const char *origin, const char *msg,
                  va_list args) {
    if (!lgHelperIsLevelEnabled(level)) {
        return;
    }

    const char *color = nullptr;
    const char *prefix = nullptr;
    lgHelperGetColorAndPrefix(level, &color, &prefix);

    const time_t epochTime = time(nullptr);
    struct tm localTime;
    localtime_r(&epochTime, &localTime);
    char timeBuf[LOG_TIME_BUFFER_LEN];
    strftime(timeBuf, sizeof(timeBuf), LOG_TIME_FMT, &localTime);

    fprintf(stderr, "%s%s [Smile %s From %s] - ", color, timeBuf, prefix,
            origin);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", LOG_WHITE); // Reset color

    if (level == LOG_FATAL) {
        fatalHandler();
    }
}

static bool lgHelperIsLevelEnabled(LogLevel level) {
    switch (level) {
        case LOG_INFO:
#ifdef SMILE_INFO
            return true;
#else
            return false;
#endif
        case LOG_WARNING:
#ifdef SMILE_WARNINGS
            return true;
#else
            return false;
#endif
        default:
            return true;
    }
}

static void lgHelperGetColorAndPrefix(LogLevel level, const char **color,
                                      const char **prefix) {
    switch (level) {
        case LOG_USER:
            *color = LOG_GREEN;
            *prefix = "LOG";
            return;
        case LOG_INFO:
            *color = LOG_CYAN;
            *prefix = "INFO";
            return;
        case LOG_WARNING:
            *color = LOG_YELLOW;
            *prefix = "WARNING";
            return;
        case LOG_ERROR:
            *color = LOG_RED;
            *prefix = "ERROR";
            return;
        case LOG_FATAL:
            *color = LOG_PURPLE;
            *prefix = "FATAL";
            return;
        default:
            *color = LOG_WHITE;
            *prefix = "\"You Shouldn't Be Seeing This!\"";
    }
}

static void lgHelperFatalHandler(void) {
    exit(EXIT_FAILURE);
}