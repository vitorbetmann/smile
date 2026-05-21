#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Log-specific result codes for failures not covered by cmResult.
 *
 * Log uses the range -100..-199 for its module-specific codes.
 */
typedef enum
{
    RES_WRITE_FAIL = -100, /**< Logging output write/flush failed. */
    RES_TIME_FAIL = -101,  /**< Time acquisition/formatting failed. */
} lgInternalResult;

/**
 * @brief Severity levels for internal Smile logging.
 */
typedef enum
{
    USER,  /**< Custom user-generated log. See lgLog(). */
    INFO,  /**< Reports informational Smile events (e.g., start successful). */
    WARN,  /**< Reports non-critical unexpected events (e.g., module started twice). */
    ERROR, /**< Reports game-breaking errors (e.g., memory allocation failure). */
    FATAL, /**< Reports program-breaking errors (e.g., fatal cleanup failure). */
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
 * @param caller Caller identifier — pass __func__, or tool name for top-level tool logs.
 * @param csq    Consequence or follow-up description.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int lgInternalLog(lgInternalLevel lvl, const char *ori, const char *cse, const char *caller,
                  const char *csq);

/**
 * @brief Logs an internal Smile event with an additional context argument.
 *
 * Identical to lgInternalLog() but includes an extra string for context
 * (for example, the name of the offending argument). If lvl is FATAL,
 * the configured fatal handler is invoked after logging.
 *
 * @param lvl    Severity level (INFO, WARN, ERROR, FATAL).
 * @param ori    Name of the module generating the log.
 * @param cse    Cause of the log event.
 * @param arg    Additional context string relevant to the event.
 * @param caller Caller identifier — pass __func__, or tool name for top-level tool logs.
 * @param csq    Consequence or follow-up description.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int lgInternalLogWithArg(lgInternalLevel lvl, const char *ori, const char *cse, const char *arg,
                         const char *caller, const char *csq);
