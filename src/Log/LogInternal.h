/**
 * @file LogInternal.h
 * @brief  LogInternal provides interface for Smile modules.
 *
 * Contains internal logging functionality and severity levels
 * for warnings, errors, and fatal events inside Smile core modules.
 *
 * @author Vitor Betmann
 */

#ifndef SMILE_LOG_INTERNAL_H
#define SMILE_LOG_INTERNAL_H

// -----------------------------------------------------------------------------
// Data types
// -----------------------------------------------------------------------------

/**
 * @brief LogInternal severity levels used by Smile.
 *
 * Represents the importance of a log message, ranging from user-defined
 * messages to fatal errors.
 *
 * @author Vitor Betmann
 */
typedef enum {
    LOG_USER,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
} LogLevel;

// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

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
 * @author Vitor Betmann
 */
void lgInternalLog(LogLevel level, const char *module, const char *cause,
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
 * @author Vitor Betmann
 */
void lgInternalLogWithArg(LogLevel level, const char *module, const char *cause,
                          const char *arg, const char *fnName,
                          const char *conseq);

#endif
