#ifndef LOG_INTERNAL_H
#define LOG_INTERNAL_H

// --------------------------------------------------
// Other defines - vars
// --------------------------------------------------

#define SMILE_CYAN "\033[36m"
#define SMILE_YELLOW "\033[33m"
#define SMILE_RED "\033[31m"
#define SMILE_PURPLE "\033[0;35m"
#define SMILE_GREEN "\033[32m"
#define SMILE_DEFAULT_COLOR "\033[0m"

// --------------------------------------------------
// Other defines - funcs
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

#define SMILE_FATAL(module, cause, conseq)                                     \
  SMILE_Log(SMILE_LOG_FATAL, module, "%s %s", cause, conseq)
#define SMILE_FATAL_WITH_NAME(module, cause, name, conseq)                     \
  SMILE_Log(SMILE_LOG_FATAL, module, "%s '%s'. %s", cause, name, conseq)
#define SMILE_FATAL_FMT(module, fmt, ...)                                      \
  SMILE_Log(SMILE_LOG_FATAL, module, fmt, __VA_ARGS__)

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef enum {
  SMILE_LOG_INFO,
  SMILE_LOG_WARNING,
  SMILE_LOG_ERROR,
  SMILE_LOG_DEBUG,
  SMILE_LOG_FATAL,
  SMILE_LOG_USER,
} LogType;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Logs a message with a specified type and module name.
 *
 * Formats and outputs a log message to stderr with timestamp, color coding,
 * and a prefix based on the log type. Supports printf-style format strings.
 *
 * @param type   The log message type (INFO, WARNING, ERROR, FATAL, USER).
 * @param module The name of the originating module.
 * @param msg      The format string (printf-style).
 * @param ...    Arguments matching the format string.
 *
 * @return void
 */
void SMILE_Log(LogType type, const char *module, const char *msg, ...);

#endif
