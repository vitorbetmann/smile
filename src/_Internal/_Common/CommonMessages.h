#ifndef COMMON_MESSAGES_H
#define COMMON_MESSAGES_H

// --------------------------------------------------
// FUNCTIONS NAMES
// --------------------------------------------------

#define INIT "Init"
#define IS_INITIALIZED "IsInitialized"

#define SHUTDOWN "Shutdown"

// --------------------------------------------------
// STATUS - Prefix
// --------------------------------------------------

#define EMPTY "Empty"

// --------------------------------------------------
// STATUS - Suffix
// --------------------------------------------------

#define SUCCESSFUL "successful"
#define ABORTED "aborted"
#define INITIALIZED "initialized"
#define FAILED "failed"

// --------------------------------------------------
// CAUSE NO ARGS
// --------------------------------------------------

#define CAUSE_ALREADY_INITIALIZED "Already initialized."
#define CAUSE_NOT_INITIALIZED "Not initialized."
#define CAUSE_MEM_ALLOC_FAILED "Memory allocation failed."

// --------------------------------------------------
// CAUSE W/ ARGS
// --------------------------------------------------

#define CAUSE_WITH_ARGS_NULL_ARG "Null Argument:"
#define CAUSE_WITH_ARGS_EMPTY_ARG "Empty Argument:"

#endif
