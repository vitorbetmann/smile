#ifndef LOG_MESSAGES_H
#define LOG_MESSAGES_H

// --------------------------------------------------
// Defines
// --------------------------------------------------

// Infos
#define LOG_INFO_SHUTDOWN_SUCCESSFUL "Shutdown successful."
#define LOG_INFO_SHUTDOWN_FAILED "Shutdown failed."
#define LOG_INFO_INIT_SUCCESSFUL "Init successful."

// Causes
#define LOG_CAUSE_ALREADY_INITIALIZED "Already initialized."
#define LOG_CAUSE_NOT_INITIALIZED "Not initialized."
#define LOG_CAUSE_NULL_NAME "Name is NULL"
#define LOG_CAUSE_EMPTY_NAME "Name is empty"
#define LOG_CAUSE_MEM_ALLOC_FAILED "Memory allocation failed."
#define LOG_CAUSE_UNKNOWN_ERROR "Unknown internal error"

// Consequences
#define LOG_CONSEQ_INIT_ABORTED "Init aborted."
#define LOG_CONSEQ_SHUTDOWN_ABORTED "Shutdown aborted."

#endif