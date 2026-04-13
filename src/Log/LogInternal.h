/**
 * @file
 * @brief Declarations of internal data types and functions for the
 *        LogInternal module.
 *
 * @see Log.c
 *
 * @author Vitor Betmann
 */

#ifndef SMILE_LOG_INTERNAL_H
#define SMILE_LOG_INTERNAL_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define SMILE_CYAN "\033[36m"
#define SMILE_YELLOW "\033[33m"
#define SMILE_RED "\033[31m"
#define SMILE_PURPLE "\033[0;35m"
#define SMILE_GREEN "\033[32m"
#define SMILE_WHITE "\033[0m"

#define LOG_TIME_FMT "%H:%M:%S"
#define LOG_TIME_BUFFER_LEN 32


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Log-specific result codes.
 *
 * @note Log-specific failures cover the following range: `-100..-199`.
 *
 * @see  src/internal/Common/Common.h for common result codes
 *
 * @author Vitor Betmann
 */
typedef enum
{
    RES_WRITE_FAIL = -100,
    RES_TIME_FAIL = -101,
} lgInternalResult;

/**
 * @brief LogInternal severity levels used by Smile.
 *
 * Represents the importance of a log message, ranging from user-defined
 * messages to fatal errors.
 *
 * @author Vitor Betmann
 */
typedef enum
{
    USER,
    INFO,
    WARN,
    ERROR,
    FATAL,
} lgInternalLevel;


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Used by Smile modules to log info, warnings, errors, or fatal events.
 *
 * Provides module or tool name, cause, function name, and consequences for context.
 *
 * @param level Severity level of the log (INFO, WARN, etc.).
 * @param origin Name of the module or tool generating the log.
 * @param cse Description of the cause of the log event.
 * @param fnName Name of the function where the log is generated.
 * @param csq Consequences or additional information about the event.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: any input argument is null; time conversion/formatting
 *       fails (`RES_TIME_FAIL`); or write to `stderr` fails
 *       (`RES_WRITE_FAIL`).
 * @note Side effects: when `level` is `FATAL`, the configured fatal handler is
 *       invoked after attempting to log.
 *
 * @author Vitor Betmann
 */
int lgInternalLog(lgInternalLevel level, const char *origin, const char *cse, const char *fnName, const char *csq);

/**
 * @brief Used by Smile modules to log info, warnings, errors, or fatal events
 * with additional context.
 *
 * Similar to lgInternalLog, but includes an extra argument string for
 * additional context.
 *
 * @param lvl Severity level of the log (WARN, ERROR, etc.).
 * @param origin Name of the module generating the log.
 * @param cause Description of the cause of the log event.
 * @param arg Additional context argument relevant to the log event.
 * @param fnName Name of the function where the log is generated.
 * @param csq Consequences or additional information about the event.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: any input argument is null; time conversion/formatting
 *       fails (`RES_TIME_FAIL`); or write to `stderr` fails
 *       (`RES_WRITE_FAIL`).
 * @note Side effects: when `level` is `FATAL`, the configured fatal handler is
 *       invoked after attempting to log.
 *
 * @author Vitor Betmann
 */
int lgInternalLogWithArg(lgInternalLevel lvl, const char *origin, const char *cse, const char *arg, const char *fnName,
                         const char *csq);


#endif
