#ifndef LOG_H
#define LOG_H

// --------------------------------------------------
// Other defines
// --------------------------------------------------

#define SMILE_INFO(module, msg) SMILE_Log(SMILE_LOG_INFO, module, "%s", msg)
#define SMILE_WARN(module, cause, conseq)                                      \
  SMILE_Log(SMILE_LOG_WARNING, module, "%s %s", cause, conseq)
#define SMILE_ERR(module, cause, conseq)                                       \
  SMILE_Log(SMILE_LOG_ERROR, module, "%s %s", cause, conseq)
// #define SMILE_DEBUG_MSG(module, msg)                                           \
//   SMILE_Log(SMILE_LOG_DEBUG, module, "%s", msg)

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef enum {
  SMILE_LOG_INFO,
  SMILE_LOG_WARNING,
  SMILE_LOG_ERROR,
  SMILE_LOG_DEBUG,
} LogType;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

void SMILE_Log(LogType type, const char *module, const char *message, ...);

#endif
