#ifndef LOG_H
#define LOG_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef void (*SMILE_FatalHandler)(void);

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Logs a user-level message from a specified location.
 *
 * This is a convenience wrapper around SMILE_Log() that always logs with
 * the USER log type. It is intended for application-level or ad-hoc messages,
 * rather than system diagnostics.
 *
 * @param location A string identifying the origin of the message
 *                 (e.g., a function name or where in the program).
 * @param msg      The format string (printf-style).
 * @param ...      Arguments matching the format string.
 *
 * @return void
 *
 * @author Vitor Betmann
 */
void LOG(const char *location, const char *msg, ...);

/**
 * @brief Sets a custom handler for fatal log events.
 *
 * By default, fatal logs terminate the program via SMILE_DefaultFatalHandler().
 * You can override this behavior by providing your own function. Passing NULL
 * restores the default handler.
 *
 * @param handler Pointer to a function with no parameters and no return value.
 *
 * @return void
 *
 * @author Vitor Betmann
 */
void LOG_SetFatalHandler(SMILE_FatalHandler handler);

#endif