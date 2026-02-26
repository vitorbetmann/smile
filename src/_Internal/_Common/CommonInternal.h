/**
 * @file
 * @brief Internal declarations of data structures and functions for the
 *        CommonInternal module.
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_COMMON_INTERNAL_H
#define SMILE_COMMON_INTERNAL_H


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

typedef enum {
    // Success
    CM_RESULT_OK = 0,
    // Common failures: -1..-99
    CM_RESULT_ALREADY_RUNNING = -1,
    CM_RESULT_NOT_RUNNING = -2,
    CM_RESULT_MEM_ALLOC_FAILED = -3,
    CM_RESULT_NULL_ARG = -4,
    CM_RESULT_EMPTY_ARG = -5,
    CM_RESULT_CLOCK_GETTIME_FAILED = -6,
} cmResult;

/**
 * @brief Function pointer used to check whether a module is currently running.
 *
 * @return `true` if the module is running, `false` otherwise.
 *
 * @author Vitor Betmann
 */
typedef bool (*cmIsRunningFn)(void);


// —————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Guard function that verifies that a module is running before a public
 *        API continues, logging an error if it is not.
 *
 * @param cmIsRunning Callback that checks whether the module is running.
 * @param module  Name of the module performing the check (for logging).
 * @param fnName  Name of the calling function (for logging).
 *
 * @return `true` if the module is running, false otherwise.
 *
 * @author Vitor Betmann
 */
bool cmInternalIsRunning(cmIsRunningFn cmIsRunning, const char *module,
                         const char *fnName);


#endif // #ifndef SMILE_COMMON_INTERNAL_H
