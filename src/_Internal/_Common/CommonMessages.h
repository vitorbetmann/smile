#ifndef COMMON_MESSAGES_H
#define COMMON_MESSAGES_H

// -----------------------------------------------------------------------------
// Functions Names
// -----------------------------------------------------------------------------

#define FN_START  "Start"
#define FN_HAS_STARTED  "HasStarted"
#define FN_STOP  "Stop"

// -----------------------------------------------------------------------------
// Causes
// -----------------------------------------------------------------------------

// Infos
#define CAUSE_MODULE_STARTED  "Module Started"
#define CAUSE_MODULE_STOPPED  "Module Stopped"
// Warnings
#define CAUSE_ALREADY_STARTED  "Module Already Started"
// Errors
#define CAUSE_MEM_ALLOC_FAILED  "Memory Allocation Failed"
#define CAUSE_NOT_STARTED  "Module Not Started"
#define CAUSE_NULL_ARG  "Null Argument"
#define CAUSE_EMPTY_ARG  "Empty Argument"

// -----------------------------------------------------------------------------
// Consequences
// -----------------------------------------------------------------------------

// Success
#define CONSEQ_SUCCESSFUL  "Successful"
// Failure
#define CONSEQ_ABORTED  "Aborted"

#endif
