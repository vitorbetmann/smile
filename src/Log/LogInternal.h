#ifndef LOG_INTERNAL_H
#define LOG_INTERNAL_H

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_USER,
    LOG_FATAL,
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
