/**
 * @file
 * @brief Public declarations of data types and functions for the Log module.
 *
 * Defines the public API for writing printf-style messages to the terminal
 * and configuring fatal error handling.
 *
 * @note Internal data structures and helper functions are defined in
 *       LogInternal.h.
 *
 * @author Vitor Betmann
 * @date 2025-10-29
 * @version 1.0.0
 */

#ifndef SMILE_LOG_H
#define SMILE_LOG_H

// —————————————————————————————————————————————————————————————————————————————
// Data types
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
 * @note Output is written to stderr.
 *
 * @author Vitor Betmann
 */
void lgLog(const char *msg, ...);

/**
 * @brief Sets a custom handler to be called when a fatal event occurs.
 *
 * @param handler Function pointer to the custom fatal handler.
 * If NULL or nullptr, the default handler is set, which terminates the program
 * with failure status after logging the event to the terminal.
 *
 * @note It's recommended your custom handler terminates the program.
 *
 * @author Vitor Betmann
 */
void lgSetFatal(lgFatalHandler handler);

#endif // #ifndef SMILE_LOG_H
