#ifndef LOG_H
#define LOG_H

// --------------------------------------------------
// Defines
// --------------------------------------------------

#define SMILE_INFO(module, message, ...)                                       \
  SMILE_Log(SMILE_LOG_INFO, module, message, ##__VA_ARGS__)
#define SMILE_WARN(module, message, ...)                                       \
  SMILE_Log(SMILE_LOG_WARNING, module, message, ##__VA_ARGS__)
#define SMILE_ERR(module, message, ...)                                        \
  SMILE_Log(SMILE_LOG_ERROR, module, message, ##__VA_ARGS__)
#define SMILE_DEBUG(module, message, ...)                                      \
  SMILE_Log(SMILE_LOG_DEBUG, module, message, ##__VA_ARGS__)

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