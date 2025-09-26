#ifndef LOG_INTERNAL_H
#define LOG_INTERNAL_H

// -----------------------------------------------------------------------------
// Data types
// -----------------------------------------------------------------------------
typedef enum {
    LOG_USER,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
} LogLevel;

// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

void lgInternalLog(LogLevel level, const char *module, const char *cause,
                   const char *fnName, const char *conseq);

void lgInternalLogWithArg(LogLevel level, const char *module,
                          const char *cause, const char *arg,
                          const char *fnName, const char *conseq);

#endif
