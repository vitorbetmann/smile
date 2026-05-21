#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Log-specific result codes for failures not covered by cmResult.
 *
 * Range -100..-199 is reserved for Log.
 */
typedef enum
{
    RES_WRITE_FAIL = -100,
    RES_TIME_FAIL = -101,
} lgInternalResult;

/**
 * @brief Severity levels for internal Smile logging.
 */
typedef enum
{
    USER,
    INFO,
    WARN,
    ERROR,
    FATAL,
} lgInternalLevel;

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Logs an internal Smile event with module, cause, and consequence context.
 *
 * If lvl is FATAL, the configured fatal handler is invoked after logging.
 *
 * @param lvl    Severity level (INFO, WARN, ERROR, FATAL).
 * @param ori    Name of the module generating the log.
 * @param cse    Cause of the log event.
 * @param caller Calling function name — pass __func__.
 * @param csq    Consequence or follow-up description.
 * @return 0 on success, or a negative error code on failure.
 */
int lgInternalLog(lgInternalLevel lvl, const char *ori, const char *cse, const char *caller,
                  const char *csq);

/**
 * @brief Logs an internal Smile event with an additional context argument.
 *
 * Identical to lgInternalLog() but includes an extra string for context
 * (for example, the name of the offending argument).
 *
 * @param lvl    Severity level (INFO, WARN, ERROR, FATAL).
 * @param ori    Name of the module generating the log.
 * @param cse    Cause of the log event.
 * @param arg    Additional context string relevant to the event.
 * @param caller Calling function name — pass __func__.
 * @param csq    Consequence or follow-up description.
 * @return 0 on success, or a negative error code on failure.
 */
int lgInternalLogWithArg(lgInternalLevel lvl, const char *ori, const char *cse, const char *arg,
                         const char *caller, const char *csq);