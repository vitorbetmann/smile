// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "Log.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

#define SMILE_CYAN "\033[36m"
#define SMILE_YELLOW "\033[33m"
#define SMILE_RED "\033[31m"
#define SMILE_DEFAULT_COLOR "\033[0m"

// --------------------------------------------------
// Functions
// --------------------------------------------------

void SMILE_Log(LogType type, const char *module, const char *message, ...) {
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

    //   case SMILE_LOG_DEBUG:
    // #ifdef SMILE_DEBUG_ENABLED
    //     color = SMILE_DEFAULT_COLOR;
    //     prefix = "DEBUG";
    //     break;
    // #else
    //     return;
    // #endif
  }

  time_t epochTime = time(NULL);
  struct tm localTime;
  localtime_r(&epochTime, &localTime);
  char timeBuffer[9];
  strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &localTime);

  fprintf(stderr, "%s [%s] [SMILE %s from %s]: ", color, timeBuffer, prefix,
          module);
  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);
  fprintf(stderr, "%s\n", SMILE_DEFAULT_COLOR); // Reset color
}
