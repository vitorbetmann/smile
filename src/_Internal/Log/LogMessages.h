#ifndef LOG_MESSAGES_H
#define LOG_MESSAGES_H

// --------------------------------------------------
// Function Names
// --------------------------------------------------

#define INIT "Init"
#define IS_INITIALIZED "IsInitialized"

#define SHUTDOWN "Shutdown"

// --------------------------------------------------
// Status
// --------------------------------------------------

#define SUCCESSFUL "successful"
#define ABORTED "aborted"

// --------------------------------------------------
// Cause No Args
// --------------------------------------------------

#define CAUSE_ALREADY_INITIALIZED "Already initialized."
#define CAUSE_NOT_INITIALIZED "Not initialized."
#define CAUSE_MEM_ALLOC_FAILED "Memory allocation failed."

// --------------------------------------------------
// Cause W/ Args
// --------------------------------------------------

#define CAUSE_WITH_ARGS_NULL_ARG "Null Argument:"
#define CAUSE_WITH_ARGS_EMPTY_ARG "Empty Argument:"

#endif
