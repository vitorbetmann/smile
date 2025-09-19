#ifndef SAVE_LOAD_SYSTEM_H
#define SAVE_LOAD_SYSTEM_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <stdlib.h>

// --------------------------------------------------
// Other defines
// --------------------------------------------------

// --------------------------------------------------
// Data types
// --------------------------------------------------

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

// Init ---------------------------------------------

/**
 * @brief Convenience initialization for SaveLoad with directory and file.
 *
 * This function calls @ref SL_Init first, then sets the game directory
 * and file using @ref SL_SetGameDir and @ref SL_SetGameFile.
 *
 * @param[in] dir  Relative game directory to initialize (e.g., "Breakout/").
 *                 Must be valid according to @ref SL_Internal_IsValidDir.
 * @param[in] file File name to use for saves/loads (e.g., "save1.txt").
 *                 Must be valid according to @ref SL_Internal_IsValidFile.
 *
 * @return `true` if all initialization steps succeeded, `false` otherwise.
 */
bool SL_InitWith(const char *dir, const char *file);

/**
 * @brief Initialize the SaveLoad system.
 *
 * Allocates internal memory and sets the default system directory for saves.
 * The default directory is guaranteed to end with a trailing slash.
 *
 * @note This function does NOT set the game directory or file. The user
 * must call @ref SL_SetGameDir and @ref SL_SetGameFile afterward,
 * or use @ref SL_InitWith for convenience.
 *
 * @return `true` if initialization succeeded, `false` otherwise.
 */
bool SL_Init(void);

/**
 * @brief Check if the SaveLoad system has been initialized.
 *
 * @return `true` if the system is initialized, `false` otherwise.
 */
bool SL_IsInitialized(void);

// Game Directory -----------------------------------

/**
 * @brief Get the current directory being used for saves/loads.
 *
 * @return A pointer to the current game directory string.
 *
 * @note The returned pointer is owned by the SaveLoad system. Do not modify
 *       or free it.
 */
const char *SL_GetGameDir(void);

/**
 * @brief Set the directory used for save/load operations.
 *
 * Validates and sanitizes the directory name, ensures it ends with a
 * trailing slash, and creates the directory if it does not exist.
 *
 * @param[in] dir Relative directory path inside the default system directory.
 *
 * @return `true` if the directory was successfully set and created,
 *         `false` otherwise.
 *
 * @note The directory path is owned by the SaveLoad system; do not modify
 *       or free the returned path.
 */
bool SL_SetGameDir(const char *dir);

/**
 * Check if a fir exists (relative path).
 */
bool SL_DirExists(const char *dir);

// Game File ----------------------------------------

/**
 * @brief Get the current file being used for saves/loads.
 *
 * @return A pointer to the current game file string.
 *
 * @note The returned pointer is owned by the SaveLoad system. Do not modify
 *       or free it.
 */
const char *SL_GetGameFile(void);

/**
 * @brief Set the file used for save/load operations.
 *
 * Validates and sanitizes the file name and combines it with the current
 * game directory to create the full file path.
 *
 * @param[in] file Name of the file to use for saves/loads (e.g., "save1.txt").
 *
 * @return `true` if the file path was successfully set, `false` otherwise.
 *
 * @note The file path is owned by the SaveLoad system; do not modify
 *       or free the returned path.
 */
bool SL_SetGameFile(const char *file);

/**
 * Check if a file exists (relative path).
 */
bool SL_FileExists(const char *file);

// Save ---------------------------------------------

/**
 * Begin a save session using the current directory and file.
 * Creates the file if it does not exist.
 */
bool SL_BeginSaveSession(void);

/**
 * Write the next line of data to the current save file.
 */
bool SL_SaveNext(const char *data);

/**
 * End the current save session (closes the file).
 */
bool SL_EndSaveSession(void);

// Load ---------------------------------------------

/**
 * Begin a load session using the current directory and file.
 * Fails if the file does not exist.
 */
bool SL_BeginLoadSession(void);

/**
 * Check if more data is available to load.
 */
bool SL_HasNext(void);

/**
 * Load the next line of data from the current file (malloc’d string).
 * Caller is responsible for freeing it.
 */
char *SL_LoadNext(void);

/**
 * Load the next line of data into a provided buffer.
 */
bool SL_LoadNextTo(char *dest, size_t size);

/**
 * End the current load session (closes the file).
 */
bool SL_EndLoadSession(void);

// Delete -------------------------------------------

/**
 * Delete a specific file by name (inside the current directory).
 */
bool SL_DeleteFile(const char *file);

bool SL_DeleteDir(const char *dir);

// Shutdown -----------------------------------------

/**
 * Shut down the SaveLoad system and free all resources.
 */
bool SL_Shutdown(void);

#endif
