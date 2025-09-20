// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "Log.h"
#include "LogInternal.h"
#include <__stdarg_va_list.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Core logging function (va_list variant).
 *
 * This function implements all log formatting and output. It is the
 * workhorse that both SMILE_Log() and LOG() delegate to.
 *
 * @param type   The log type (INFO, WARNING, ERROR, FATAL, USER).
 * @param module The name of the module or location string.
 * @param msg    The format string (printf-style).
 * @param args   A va_list containing arguments for the format string.
 *
 * @return void
 */
static void SMILE_LogV(LogType type, const char *module, const char *msg,
                       va_list args);

/**
 * @brief Internal default fatal handler.
 *
 * This is used when no custom fatal handler is installed via
 * LOG_SetFatalHandler(). It simply terminates the process with EXIT_FAILURE.
 *
 * @note This function is intended for internal use only and is not
 *       exposed in the public API.
 *
 * @return void
 */
static void SMILE_DefaultFatalHandler(void);

// --------------------------------------------------
// Variables
// --------------------------------------------------

SMILE_FatalHandler fatalHandler = SMILE_DefaultFatalHandler;

// --------------------------------------------------
// Functions - Public
// --------------------------------------------------

void LOG(const char *origin, const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  SMILE_LogV(SMILE_LOG_USER, origin, msg, args);
  va_end(args);
}

void LOG_SetFatalHandler(SMILE_FatalHandler handler) {
  if (!handler) {
    fatalHandler = SMILE_DefaultFatalHandler;
    return;
  }

  fatalHandler = handler;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

void SMILE_Log(LogType type, const char *module, const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  SMILE_LogV(type, module, msg, args);
  va_end(args);
}

// --------------------------------------------------
// Functions - Private
// --------------------------------------------------

static void SMILE_LogV(LogType type, const char *module, const char *msg,
                       va_list args) {
  const char *color;
  const char *prefix;

  // Determine color based on type
  switch (type) {
  case SMILE_LOG_INFO:
#ifdef SMILE_INFO_ENABLED
    color = SMILE_CYAN;
    prefix = "INFO";
    break;
#else
    return;
#endif

  case SMILE_LOG_WARNING:
#ifdef SMILE_WARNINGS_ENABLED
    color = SMILE_YELLOW;
    prefix = "WARNING";
    break;
#else
    return;
#endif

  case SMILE_LOG_ERROR:
    color = SMILE_RED;
    prefix = "ERROR";
    break;

    // case SMILE_LOG_DEBUG:
    // #ifdef SMILE_DEBUG_ENABLED
    //     color = SMILE_DEFAULT_COLOR;
    //     prefix = "DEBUG";
    // break;
    // #else
    //     return;
    // #endif

  case SMILE_LOG_FATAL:
    color = SMILE_PURPLE;
    prefix = "FATAL";
    break;

  case SMILE_LOG_USER:
    color = SMILE_GREEN;
    prefix = "USER";
    break;

  default:
    color = SMILE_DEFAULT_COLOR;
    prefix = "LOG";
    break;
  }

  time_t epochTime = time(NULL);
  struct tm localTime;
  localtime_r(&epochTime, &localTime);
  char timeBuffer[9];
  strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &localTime);

  fprintf(stderr, "%s [%s] [SMILE %s from %s]: ", color, timeBuffer, prefix,
          module);
  vfprintf(stderr, msg, args);
  fprintf(stderr, "%s\n", SMILE_DEFAULT_COLOR); // Reset color

  if (type == SMILE_LOG_FATAL) {
    fatalHandler();
  }
}

static void SMILE_DefaultFatalHandler(void) { exit(EXIT_FAILURE); }