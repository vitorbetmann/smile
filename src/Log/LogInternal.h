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
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Log-specific result codes.
 *
 * @note Log-specific failures cover the following range: `-100..-199`.
 *
 * @see  src/_Internal/_Common/CommonInternal.h for common result codes
 *
 * @author Vitor Betmann
 */
typedef enum {
    LG_RESULT_WRITE_FAILED = -100,
    LG_RESULT_TIME_FAILED = -101,
} lgResult;

/**
 * @brief LogInternal severity levels used by Smile.
 *
 * Represents the importance of a log message, ranging from user-defined
 * messages to fatal errors.
 *
 * @author Vitor Betmann
 */
typedef enum {
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Used by Smile modules to log info, warnings, errors, or fatal events.
 *
 * Provides module name, cause, function name, and consequences for context.
 *
 * @param level Severity level of the log (LOG_INFO, LOG_WARNING, etc.).
 * @param module Name of the module generating the log.
 * @param cause Description of the cause of the log event.
 * @param fnName Name of the function where the log is generated.
 * @param conseq Consequences or additional information about the event.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: any input argument is null; time conversion/formatting
 *       fails (`LG_RESULT_TIME_FAILED`); or write to `stderr` fails
 *       (`LG_RESULT_WRITE_FAILED`).
 * @note Side effects: when `level` is `FATAL`, the configured fatal handler is
 *       invoked after attempting to log.
 *
 * @author Vitor Betmann
 */
int lgInternalLog(InternalLevel level, const char *module, const char *cause,
                  const char *fnName, const char *conseq);

/**
 * @brief Used by Smile modules to log info, warnings, errors, or fatal events
 * with additional context.
 *
 * Similar to lgInternalLog, but includes an extra argument string for
 * additional context.
 *
 * @param level Severity level of the log (LOG_WARNING, LOG_ERROR, etc.).
 * @param module Name of the module generating the log.
 * @param cause Description of the cause of the log event.
 * @param arg Additional context argument relevant to the log event.
 * @param fnName Name of the function where the log is generated.
 * @param conseq Consequences or additional information about the event.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: any input argument is null; time conversion/formatting
 *       fails (`LG_RESULT_TIME_FAILED`); or write to `stderr` fails
 *       (`LG_RESULT_WRITE_FAILED`).
 * @note Side effects: when `level` is `FATAL`, the configured fatal handler is
 *       invoked after attempting to log.
 *
 * @author Vitor Betmann
 */
int lgInternalLogWithArg(InternalLevel level, const char *module,
                         const char *cause, const char *arg,
                         const char *fnName, const char *conseq);


#endif // #ifndef SMILE_LOG_INTERNAL_H
