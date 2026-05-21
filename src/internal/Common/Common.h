#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Common result codes shared across Smile modules.
 *
 * RES_OK (0) indicates success. Negative values indicate failure.
 * The range -1..-99 is reserved for Common; modules define their own ranges
 * outside that window.
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
    RES_DEL_FILE_FAIL = -11,
    RES_CREATE_FILE_FAIL = -12,
    RES_DIR_NOT_FOUND = -13,
    RES_DEL_DIR_FAIL = -14,
} cmResult;

/**
 * @brief Function pointer used to check whether a module is currently running.
 */
typedef bool (*cmIsRunningFn)(void);

// Functions ———————————————————————————————————————————————————————————————————————————————————————

// Start Related

/**
 * @brief Guard that verifies a module is running before a public API continues.
 *
 * Logs an error if the module is not running.
 *
 * @param cmIsRunning Callback that checks whether the module is running.
 * @param module      Name of the module performing the check (for logging).
 * @param fnName      Name of the calling function (for logging).
 * @return true if the module is running, false otherwise.
 */
bool cmIsRunning(cmIsRunningFn cmIsRunning, const char *module, const char *fnName);

// Filesystem

/**
 * @brief Checks whether a directory exists at the given path.
 *
 * @param path Directory path to check.
 * @return true if a directory exists at path, false otherwise.
 */
bool cmDirExists(const char *path);

/**
 * @brief Validates that a path is safe and within acceptable bounds.
 *
 * Rejects absolute paths, paths exceeding CM_PATH_MAX, and paths containing
 * bare ".." segments.
 *
 * @param path Path string to validate.
 * @return RES_OK on success, or a negative error code on failure.
 */
int cmValidatePath(const char *path);

/**
 * @brief Recursively creates a directory and all necessary parent directories.
 *
 * Validates the path via cmValidatePath(). Treats an already-existing directory
 * as success.
 *
 * @param path Relative path of the directory to create.
 * @return RES_OK on success, or a negative error code on failure.
 */
int cmCreateDir(const char *path);

/**
 * @brief Checks whether a file exists and is readable at the given path.
 *
 * @param filename File path to check.
 * @return true if the file exists and can be opened for reading, false otherwise.
 */
bool cmFileExists(const char *filename);

/**
 * @brief Deletes a file at the specified path.
 *
 * Validates the path, confirms the file exists, then removes it.
 *
 * @param path Relative path of the file to delete.
 * @return RES_OK on success, or a negative error code on failure.
 */
int cmDeleteFile(const char *path);

/**
 * @brief Deletes an empty directory at the specified path.
 *
 * Validates the path, confirms the directory exists, then removes it.
 * The directory must be empty.
 *
 * @param path Relative path of the directory to delete.
 * @return RES_OK on success, or a negative error code on failure.
 */
int cmDeleteDir(const char *path);

// Variables - Constants ———————————————————————————————————————————————————————————————————————————

/** @brief Maximum allowed path length in bytes, including the null terminator. */
extern const int CM_PATH_MAX;