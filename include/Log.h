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
 * @brief Logs a formatted SMILE-style message to the terminal.
 *
 * Each message includes a timestamp, the SMILE USER tag, the origin,
 * and the user-provided text. Output is written to standard error with
 * coloring for readability. A newline is automatically appended.
 *
 * Example output:
 * [09:10:11] [SMILE USER from main]: Starting my new game.
 *
 * @param origin   A string identifying the source of the message (e.g.,
 *                 function name or point in the program).
 * @param msg      The format string (printf-style).
 * @param ...      Arguments matching the format string.
 *
 * @return void
 *
 * @author Vitor Betmann
 */
void LOG(const char *origin, const char *msg, ...);

/**
 * @brief Sets a custom handler for fatal log events.
 *
 * By default, fatal logs terminate the program immediately via
 * SMILE_DefaultFatalHandler(). You can override this behavior by providing your
 * own function. Passing NULL restores the default handler.
 *
 * @param handler Pointer to a function with no parameters and no return value.
 *
 * @return void
 *
 * @author Vitor Betmann
 */
void LOG_SetFatalHandler(SMILE_FatalHandler handler);

#endif