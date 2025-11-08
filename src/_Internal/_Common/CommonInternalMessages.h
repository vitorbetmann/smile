/**
 * @file
 * @brief Definitions for shared log messages.
 *
 * Defines shared function identifiers, causes, and consequences
 * that can be reused across all Smile modules for consistent logging.
 *
 * @author Vitor Betmann
 * @date 2025-10-29
 * @version 1.0.0
 */


#ifndef SMILE_COMMON_INTERNAL_MESSAGES_H
#define SMILE_COMMON_INTERNAL_MESSAGES_H


// —————————————————————————————————————————————————————————————————————————————
// Functions Names
// —————————————————————————————————————————————————————————————————————————————

// Start Functions
#define FN_START  "Start"
#define FN_IS_RUNNING  "IsRunning"
// Stop Functions
#define FN_STOP  "Stop"


// —————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————

// Infos
#define CAUSE_MODULE_STARTED  "Module Started"
#define CAUSE_MODULE_STOPPED  "Module Stopped"
// Warnings
#define CAUSE_ALREADY_RUNNING  "Module Already Running"
// Errors
#define CAUSE_MEM_ALLOC_FAILED  "Memory Allocation Failed"
#define CAUSE_NOT_RUNNING  "Module Not Running"
#define CAUSE_NULL_ARG  "Null Argument"
#define CAUSE_EMPTY_ARG  "Empty Argument"


// —————————————————————————————————————————————————————————————————————————————
// Consequences
// —————————————————————————————————————————————————————————————————————————————

// Success
#define CONSEQ_SUCCESSFUL  "Successful"
// Failure
#define CONSEQ_ABORTED  "Aborted"


#endif // #ifndef SMILE_COMMON_INTERNAL_MESSAGES_H

