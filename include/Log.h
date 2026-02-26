/**
 * @file
 * @brief Declarations of public data types and functions for the
 *        Log module.
 *
 * @see docs/Log/README.md
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_LOG_H
#define SMILE_LOG_H


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Function pointer type for custom fatal error handlers.
 *
 * @author Vitor Betmann
 */
typedef void (*lgFatalHandler)(void);


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Logs a message to the terminal ending in a new line. Supports
 * printf-style formatting.
 *
 * @param msg Format string for the message to log.
 * @param ... Additional arguments related to the format specifiers in msg.
 *
 * @return Returns `0` on success, or a negative error code on failure.
 *
 * @note Fails if: `msg` is null; time conversion/formatting fails; or write to
 *       `stderr` fails.
 * @note Output is written to stderr.
 * @note Side effects: none beyond writing to `stderr`.
 *
 * @author Vitor Betmann
 */
int lgLog(const char *msg, ...);

/**
 * @brief Sets a custom handler to be called when a fatal event occurs.
 *
 * @param handler Function pointer to the custom fatal handler.
 * If NULL or nullptr, the default handler is set, which terminates the program
 * with failure status after logging the event to the terminal.
 *
 * @return Returns `0` on success.
 *
 * @note Fails if: none.
 * @note It's recommended your custom handler terminates the program.
 *
 * @author Vitor Betmann
 */
int lgSetFatal(lgFatalHandler handler);


#endif // #ifndef SMILE_LOG_H
