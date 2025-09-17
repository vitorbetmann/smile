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

/**
 * Initialize the SaveLoad system for a specific game.
 * Creates a sandbox directory under the system's default save location.
 * Example: <sysDefault>/smile/<gameName>/
 */
bool SL_Init(const char *gameName);

/**
 * Check if the SaveLoad system has been successfully initialized.
 */
bool SL_IsInitialized(void);

/**
 * Set (and if needed, create) a subdirectory inside the game directory.
 * Example: "DLC1" -> <sysDefault>/smile/<gameName>/DLC1/
 * NOTE: This always treats 'dir' as relative to the game directory.
 */
bool SL_SetGameDir(const char *dir);

/**
 * Get the current directory being used for saves/loads.
 * This pointer is owned by Smile; do not modify or free it.
 */
const char *SL_GetGameDir(void);

/**
 * Set (and if needed, create) the file inside the current game directory.
 * Example: "save1.txt" -> <currentDir>/save1.txt
 * NOTE: This always treats 'file' as relative to the current game directory.
 */
bool SL_SetGameFile(const char *file);

/**
 * Get the current file being used for saves/loads.
 * This pointer is owned by Smile; do not modify or free it.
 */
const char *SL_GetGameFile(void);

/**
 * Check if a directory exists (absolute path).
 */
bool SL_DirExists(const char *dir);

/**
 * Check if a file exists (absolute path).
 */
bool SL_FileExists(const char *file);

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

/**
 * Convenience: write data directly to a given file (one-shot).
 */
bool SL_SaveTo(const char *file, const char *data);

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

/**
 * Convenience: load entire file into memory (one-shot).
 * Caller is responsible for freeing it.
 */
char *SL_LoadFrom(const char *file);

/**
 * Delete the current save file (as set by SL_SetGameFile).
 */
bool SL_DeleteCurrSave(void);

/**
 * Delete a specific file by name (inside the current directory).
 */
bool SL_DeleteSave(const char *file);

/**
 * Shut down the SaveLoad system and free all resources.
 */
bool SL_Shutdown(void);

#endif