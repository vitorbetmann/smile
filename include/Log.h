#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Function pointer type for custom fatal error handlers.
 */
typedef void (*lgFatalHandler)(void);

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Logs a message to stderr ending in a newline. Supports printf-style formatting.
 *
 * @param msg Format string for the message to log.
 * @param ... Additional arguments matching the format specifiers in msg.
 * @return 0 on success, or a negative error code on failure.
 */
int lgLog(const char *msg, ...);

/**
 * @brief Sets a custom handler to be called when a fatal event occurs.
 *
 * If handler is null, the default handler is restored, which logs the event
 * and terminates the program with failure status.
 *
 * @param handler Function pointer to the custom fatal handler.
 * @return 0 on success.
 */
int lgSetFatal(lgFatalHandler handler);
