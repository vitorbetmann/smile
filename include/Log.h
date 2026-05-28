#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/** @brief Function pointer type for custom fatal error handlers. */
typedef void (*lgFatalHandler)(void);

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Logs a formatted message to the terminal, ending with a newline.
 *
 * Supports printf-style formatting. Output is written to stderr.
 * Do not pass untrusted input directly as msg; use lgLog("%s", untrustedInput) instead.
 *
 * @param msg Format string for the message to log.
 * @param ... Additional arguments matching the format specifiers in msg.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int lgLog(const char *msg, ...);

/**
 * @brief Sets a custom handler to be called when a fatal event occurs.
 *
 * @param handler Function pointer to the custom fatal handler. Pass NULL or nullptr to restore
 *                the default handler, which logs the event to the terminal and terminates the
 *                program with failure status.
 *
 * @return 0 on success.
 */
int lgSetFatal(lgFatalHandler handler);
