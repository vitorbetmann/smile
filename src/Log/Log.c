// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "include/Log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "LogInternal.h"


// --------------------------------------------------
// Defines
// --------------------------------------------------

#define SMILE_CYAN "\033[36m"
#define SMILE_YELLOW "\033[33m"
#define SMILE_RED "\033[31m"
#define SMILE_PURPLE "\033[0;35m"
#define SMILE_GREEN "\033[32m"
#define SMILE_DEFAULT_COLOR "\033[0m"

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

static void lgEvent(LogLevel level, const char *module, const char *msg, ...);

static void lgEventV(LogLevel level, const char *module, const char *fmt,
                     va_list args);

static void lgInternalFatalHandler(void);

// --------------------------------------------------
// Variables
// --------------------------------------------------

lgFatalHandler fatalHandler = lgInternalFatalHandler;

// --------------------------------------------------
// Functions - Public
// --------------------------------------------------

void lgLog(const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  lgEventV(LOG_USER, "User", msg, args);
  va_end(args);
}

void lgSetFatalHandler(lgFatalHandler handler) {
  if (!handler) {
    fatalHandler = lgInternalFatalHandler;
    return;
  }

  fatalHandler = handler;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

void lgInternalLog(LogLevel level, const char *module, const char *cause,
                   const char *fnName, const char *conseq) {
  lgEvent(level, module, "%s. '%s' %s.", cause, fnName, conseq);
}

void lgInternalLogWithArg(LogLevel level, const char *module,
                          const char *cause, const char *arg,
                          const char *fnName, const char *conseq) {
  lgEvent(level, module, "%s: %s. '%s' %s.", cause, arg, fnName, conseq);
}

void lgInternalEventFmt(LogLevel level, const char *module, const char *msg,
                        ...) {
  va_list args;
  va_start(args, msg);
  lgEventV(level, module, msg, args);
  va_end(args);
}

// --------------------------------------------------
// Functions - Private
// --------------------------------------------------

void lgEvent(LogLevel level, const char *module, const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  lgEventV(level, module, msg, args);
  va_end(args);
}

static void lgEventV(const LogLevel level, const char *module, const char *fmt,
                     const va_list args) {
  const char *color;
  const char *prefix;

  // Determine color based on type
  switch (level) {
    case LOG_INFO:
#ifdef SMILE_INFO_ENABLED
      color = SMILE_CYAN;
      prefix = "INFO";
      break;
#else
      return;
#endif

    case LOG_WARNING:
#ifdef SMILE_WARNINGS_ENABLED
      color = SMILE_YELLOW;
      prefix = "WARNING";
      break;
#else
      return;
#endif

    case LOG_ERROR:
      color = SMILE_RED;
      prefix = "ERROR";
      break;

    case LOG_FATAL:
      color = SMILE_PURPLE;
      prefix = "FATAL";
      break;

    case LOG_USER:
      color = SMILE_GREEN;
      prefix = "Log";
      break;

    default:
      color = SMILE_DEFAULT_COLOR;
      prefix = "Log";
      break;
  }

  const time_t epochTime = time(nullptr);
  struct tm localTime;
  localtime_r(&epochTime, &localTime);
  char timeBuf[9];
  strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &localTime);

  fprintf(stderr, "%s[%s Smile %s from %s] - ", color, timeBuf, prefix,
          module);
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "%s\n", SMILE_DEFAULT_COLOR); // Reset color

  if (level == LOG_FATAL) {
    fatalHandler();
  }
}

static void lgInternalFatalHandler(void) {
  exit(EXIT_FAILURE);
}
