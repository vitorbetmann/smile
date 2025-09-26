#ifndef COMMON_MESSAGES_H
#define COMMON_MESSAGES_H

// -----------------------------------------------------------------------------
// Enums
// -----------------------------------------------------------------------------

typedef enum {
    // Start Functions
    START,
    HAS_STARTED,
    // Stop Functions
    STOP
} CommonFunctions;

typedef enum {
    // Infos
    MODULE_STARTED,
    MODULE_STOPPED,
    // Warnings
    ALREADY_STARTED,
    // Errors
    MEM_ALLOC_FAILED,
    NOT_STARTED,
    NULL_ARG,
    EMPTY_ARG,
} CommonCauses;

typedef enum {
    // Success
    SUCCESSFUL,
    // Failure
    ABORTED,
} CommonConsequences;

// -----------------------------------------------------------------------------
// String Arrays
// -----------------------------------------------------------------------------

static char *cmFuncs[] = {
    [START] = "Start",
    [HAS_STARTED] = "HasStarted",
    [STOP] = "Stop",
};

static char *cmCauses[] = {
    // Infos
    [MODULE_STARTED] = "Module Started",
    [MODULE_STOPPED] = "Module Stopped",
    // Warnings
    [ALREADY_STARTED] = "Module Already Started",
    // Errors
    [MEM_ALLOC_FAILED] = "Memory Allocation Failed",
    [NOT_STARTED] = "Module Not Started",
    [NULL_ARG] = "Null Argument",
    [EMPTY_ARG] = "Empty Argument",
};

static char *cmConseqs[] = {
    // Success
    [SUCCESSFUL] = "Successful",
    // Failure
    [ABORTED] = "Aborted",
};

#endif
