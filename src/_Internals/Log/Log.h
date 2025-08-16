#ifndef LOG_H
#define LOG_H

// --------------------------------------------------
// Other defines
// --------------------------------------------------

#define SMILE_INFO(module, info) SMILE_Log(SMILE_LOG_INFO, module, "%s", info)
#define SMILE_INFO_WITH_NAME(module, info, name)                               \
  SMILE_Log(SMILE_LOG_INFO, module, "%s '%s'.", info, name)
#define SMILE_INFO_FMT(module, fmt, ...)                                       \
  SMILE_Log(SMILE_LOG_INFO, module, fmt, __VA_ARGS__)

#define SMILE_WARN(module, cause, conseq)                                      \
  SMILE_Log(SMILE_LOG_WARNING, module, "%s %s", cause, conseq)
#define SMILE_WARN_WITH_NAME(module, cause, name, conseq)                      \
  SMILE_Log(SMILE_LOG_WARNING, module, "%s '%s'. %s", cause, name, conseq)
#define SMILE_WARN_FMT(module, fmt, ...)                                       \
  SMILE_Log(SMILE_LOG_WARNING, module, fmt, __VA_ARGS__)

#define SMILE_ERR(module, cause, conseq)                                       \
  SMILE_Log(SMILE_LOG_ERROR, module, "%s %s", cause, conseq)
#define SMILE_ERR_WITH_NAME(module, cause, name, conseq)                       \
  SMILE_Log(SMILE_LOG_ERROR, module, "%s '%s'. %s", cause, name, conseq)
#define SMILE_ERR_FMT(module, fmt, ...)                                        \
  SMILE_Log(SMILE_LOG_ERROR, module, fmt, __VA_ARGS__)

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
