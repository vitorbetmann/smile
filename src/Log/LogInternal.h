#ifndef LOG_INTERNAL_H
#define LOG_INTERNAL_H

// --------------------------------------------------
// Other defines - Values
// --------------------------------------------------

#define SMILE_CYAN "\033[36m"
#define SMILE_YELLOW "\033[33m"
#define SMILE_RED "\033[31m"
#define SMILE_PURPLE "\033[0;35m"
#define SMILE_GREEN "\033[32m"
#define SMILE_DEFAULT_COLOR "\033[0m"

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_FATAL,
    LOG_USER,
} LogLevel;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

void lgInternalEvent(LogLevel level, const char *module, const char *cause,
                     const char *funcName, const char *conseq);

void lgInternalEventWithArg(LogLevel level, const char *module,
                            const char *cause, const char *arg,
                            const char *funcName, const char *conseq);

void lgInternalEventFmt(LogLevel level, const char *module, const char *fmt,
                        ...);

#endif
