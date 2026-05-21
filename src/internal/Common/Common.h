#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Common result codes shared across Smile modules; range -1..-99 is exclusive to Common.
 */
typedef enum
{
    RES_OK = 0,                  /**< Operation completed successfully. */
    RES_ALREADY_RUNNING = -1,    /**< Module is already running. */
    RES_NOT_RUNNING = -2,        /**< Module is not running. */
    RES_MEM_ALLOC_FAIL = -3,     /**< Memory allocation failed. */
    RES_NULL_ARG = -4,           /**< A required pointer argument is null. */
    RES_EMPTY_ARG = -5,          /**< A required string argument is empty. */
    RES_CLOCK_GETTIME_FAIL = -6, /**< System clock query failed. */
    RES_INVALID_PATH = -7,       /**< Path is absolute, too long, or contains .. segments. */
    RES_INVALID_ARG = -8,        /**< Argument is invalid (e.g., malformed name). */
    RES_FILE_NOT_FOUND = -9,     /**< File does not exist at the specified path. */
    RES_CREATE_DIR_FAIL = -10,   /**< Directory creation failed. */
    RES_DEL_FILE_FAIL = -11,     /**< File exists but could not be deleted. */
    RES_CREATE_FILE_FAIL = -12,  /**< File could not be created or written. */
    RES_DIR_NOT_FOUND = -13,     /**< Directory does not exist at the specified path. */
    RES_DEL_DIR_FAIL = -14,      /**< Directory exists but could not be deleted. */
} cmResult;

/** @brief Function pointer that checks whether a module is currently running. */
typedef bool (*cmIsRunningFn)(void);

// Functions ———————————————————————————————————————————————————————————————————————————————————————

// Start Related

/**
 * @brief Guard that verifies a module is running before a public API continues; logs an error if not.
 *
 * @param cmIsRunning Callback that checks whether the module is running.
 * @param module      Name of the module performing the check (for logging).
 * @param fnName      Name of the calling function (for logging).
 *
 * @return true if running, false otherwise.
 */
bool cmIsRunning(cmIsRunningFn cmIsRunning, const char *module, const char *fnName);

// Filesystem

/**
 * @brief Checks whether a directory exists at the given path.
 *
 * @param path Directory path to check.
 *
 * @return true if a directory exists at path, false otherwise.
 */
bool cmDirExists(const char *path);

/**
 * @brief Validates that a path is relative, within CM_PATH_MAX, and contains no .. segments.
 *
 * @param path Path string to validate.
 *
 * @return RES_OK, or RES_NULL_ARG / RES_EMPTY_ARG / RES_INVALID_PATH on failure.
 */
int cmValidatePath(const char *path);

/**
 * @brief Recursively creates a directory and all necessary parents; treats existing as success.
 *
 * @param path Relative path of the directory to create.
 *
 * @return RES_OK, or any code returned by cmValidatePath, or RES_CREATE_DIR_FAIL.
 */
int cmCreateDir(const char *path);

/**
 * @brief Checks whether a file exists and is readable at the given path.
 *
 * @param filename File path to check.
 *
 * @return true if the file can be opened for reading, false otherwise.
 */
bool cmFileExists(const char *filename);

/**
 * @brief Deletes a file at the specified path.
 *
 * @param path Relative path of the file to delete.
 *
 * @return RES_OK, or RES_FILE_NOT_FOUND / RES_DEL_FILE_FAIL on failure.
 */
int cmDeleteFile(const char *path);

/**
 * @brief Deletes an empty directory at the specified path.
 *
 * @param path Relative path of the directory to delete.
 *
 * @return RES_OK, or RES_DIR_NOT_FOUND / RES_DEL_DIR_FAIL on failure.
 */
int cmDeleteDir(const char *path);

// Variables ———————————————————————————————————————————————————————————————————————————————————————

/** @brief Maximum allowed path length in bytes, including the null terminator. */
static constexpr int CM_PATH_MAX = 256;;
