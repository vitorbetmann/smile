/**
 * @file
 * @brief Internal declarations of data structures and functions for the
 *        CommonInternal module.
 *
 * @note TODO #XX [Function] for [CommonInternal] - Add cmInternalPathJoin(buf, bufSize, dir, file) for cross-platform
 *                                                  path joining (CM_PATH_MAX, correct separator)
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_COMMON_INTERNAL_H
#define SMILE_COMMON_INTERNAL_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <stddef.h>


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define CM_PATH_MAX 256

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Common result codes shared across Smile modules.
 *
 * @note `CM_RESULT_OK` (`0`) indicates success.
 * @note The following range is exclusive to Common failures: `-1..-99`.
 *
 * @author Vitor Betmann
 */
typedef enum
{
    CM_RESULT_OK = 0,
    CM_RESULT_ALREADY_RUNNING = -1,
    CM_RESULT_NOT_RUNNING = -2,
    CM_RESULT_MEM_ALLOC_FAILED = -3,
    CM_RESULT_NULL_ARG = -4,
    CM_RESULT_EMPTY_ARG = -5,
    CM_RESULT_CLOCK_GETTIME_FAILED = -6,
    CM_RESULT_INVALID_PATH = -7,
    CM_RESULT_INVALID_NAME = -8,
    CM_RESULT_FILE_NOT_FOUND = -9,
    CM_RESULT_FAIL_TO_CREATE_DIR = -10,
    CM_RESULT_FAIL_TO_DELETE_FILE = -11,
    CM_RESULT_FAIL_TO_CREATE_FILE = -12,
} cmInternalResult;

/**
 * @brief Function pointer used to check whether a module is currently running.
 *
 * @return `true` if the module is running, `false` otherwise.
 *
 * @author Vitor Betmann
 */
typedef bool (*cmIsRunningFn)(void);


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Start Related

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
bool cmInternalIsRunning(cmIsRunningFn cmIsRunning, const char *module, const char *fnName);

// Filesystem

bool cmInternalDirExists(const char *path);

int cmInternalValidatePath(const char *path);

int cmInternalCreateDir(const char *path);

bool cmInternalFileExists(const char *filename);

int cmInternalDeleteFile(const char *path);

// int cmInternalCreateFile(const char *path);


#endif
