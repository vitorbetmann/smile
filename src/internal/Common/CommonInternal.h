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
    RES_OK = 0,
    RES_ALREADY_RUNNING = -1,
    RES_NOT_RUNNING = -2,
    RES_MEM_ALLOC_FAIL = -3,
    RES_NULL_ARG = -4,
    RES_EMPTY_ARG = -5,
    RES_CLOCK_GETTIME_FAIL = -6,
    RES_INVALID_PATH = -7,
    RES_INVALID_ARG = -8,
    RES_FILE_NOT_FOUND = -9,
    RES_CREATE_DIR_FAIL = -10,
    RES_DELETE_FILE_FAIL = -11,
    RES_CREATE_FILE_FAIL = -12,
    RES_DIR_NOT_FOUND = -13,
    RES_DELETE_DIR_FAIL = -14,
} cmResult;

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
bool cmIsRunning(cmIsRunningFn cmIsRunning, const char *module, const char *fnName);

// Filesystem

/**
 * @brief Checks whether a directory exists at the given path.
 *
 * @param path The directory path to check.
 *
 * @return `true` if a directory exists at @p path, `false` otherwise.
 *
 * @author Vitor Betmann
 */
bool cmDirExists(const char *path);

/**
 * @brief Validates that a path is safe and within acceptable bounds.
 *
 * Rejects absolute paths, paths exceeding `CM_PATH_MAX`, and paths containing
 * bare `..` segments to prevent directory traversal.
 *
 * @param path The path string to validate.
 *
 * @return `CM_RESULT_OK` on success, or a negative error code on failure.
 *
 * @note Fails if: @p path is NULL (`CM_RESULT_NULL_ARG`); @p path is empty
 *       (`CM_RESULT_EMPTY_ARG`); or the path is absolute, exceeds `CM_PATH_MAX`
 *       characters, or contains a bare `..` segment (`CM_RESULT_INVALID_PATH`).
 *
 * @author Vitor Betmann
 */
int cmValidatePath(const char *path);

/**
 * @brief Recursively creates a directory and all necessary parent directories.
 *
 * Validates the path via `cmValidatePath`, then creates each missing
 * path segment in order. Treats an already-existing directory as success.
 *
 * @param path Relative path of the directory to create.
 *
 * @return `CM_RESULT_OK` on success, or a negative error code on failure.
 *
 * @note Fails if: the path is invalid (see `cmInternalValidatePath`); or a
 *       directory segment cannot be created (`CM_RESULT_FAIL_TO_CREATE_DIR`).
 *
 * @author Vitor Betmann
 */
int cmCreateDir(const char *path);

/**
 * @brief Checks whether a file exists and is readable at the given path.
 *
 * @param filename The file path to check.
 *
 * @return `true` if the file exists and can be opened for reading,
 *         `false` otherwise.
 *
 * @author Vitor Betmann
 */
bool cmFileExists(const char *filename);

/**
 * @brief Deletes a file at the specified path.
 *
 * Validates the path, confirms the file exists, then removes it.
 *
 * @param path Relative path of the file to delete.
 *
 * @return `CM_RESULT_OK` on success, or a negative error code on failure.
 *
 * @note Fails if: the path is invalid (see `cmInternalValidatePath`); the file
 *       does not exist (`CM_RESULT_FILE_NOT_FOUND`); or deletion fails
 *       (`CM_RESULT_FAIL_TO_DELETE_FILE`).
 * @note Side effects: permanently removes the file from the filesystem.
 *
 * @author Vitor Betmann
 */
int cmDeleteFile(const char *path);

/**
 * @brief Deletes an empty directory at the specified path.
 *
 * Validates the path, confirms the directory exists, then removes it.
 *
 * @param path Relative path of the directory to delete.
 *
 * @return `CM_RESULT_OK` on success, or a negative error code on failure.
 *
 * @note Fails if: the path is invalid (see `cmInternalValidatePath`); the
 *       directory does not exist (`CM_RESULT_DIR_NOT_FOUND`); or deletion fails
 *       (`CM_RESULT_FAIL_TO_DELETE_DIR`).
 * @note The directory must be empty; non-empty directories will fail.
 * @note Side effects: permanently removes the directory from the filesystem.
 *
 * @author Vitor Betmann
 */
int cmDeleteDir(const char *path);


#endif
