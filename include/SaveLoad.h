#ifndef SAVE_LOAD_SYSTEM_H
#define SAVE_LOAD_SYSTEM_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <stdlib.h>

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef enum {
    FILES,
    DIRS,
} ListType;

typedef struct {
    char **list;
    int amount;
} SmileList;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

// Init Related -------------------------------------

/**
 * @brief Initialize the SaveLoad system with a specific game directory and file.
 *
 * This function performs three steps:
 * 1. Calls @ref SL_Init() to initialize internal memory and set the default Smile system directory.
 * 2. Sets the game directory via @ref SL_SetGameDir().
 * 3. Sets the game file via @ref SL_SetGameFile().
 *
 * @note Both the directory and file are relative to the Smile system directory.
 * @note If the Smile directory does not exist, it will be automatically created.
 * @note The game directory is also created immediately if it does not exist.
 * @note The game file is not created at this stage — it will be created on the
 *       first call to @ref SL_BeginSaveSession().
 *
 * @param[in] dir  Relative directory path inside the Smile system directory (e.g., "Breakout/").
 * @param[in] file Name of the file to use for saves/loads (e.g., "save1.txt").
 *
 * @return `true` if the system was successfully initialized and both the directory
 *         and file were set, `false` otherwise.
 *
 * @sa SL_Init(), SL_SetGameDir(), SL_SetGameFile()
 *
 * @author Vitor Betmann
 */
bool SL_InitWith(const char *dir, const char *file);

/**
 * @brief Initialize the SaveLoad system.
 *
 * Allocates the internal SaveLoadTracker structure and sets up the default Smile system directory.
 * The default directory will contain the root path up to the "Smile" folder and is guaranteed
 * to end with a trailing slash.
 *
 * @note If the Smile directory does not exist, it will be automatically created.
 * @note This function does NOT set a specific game directory or file. The user must call
 *       @ref SL_SetGameDir() and @ref SL_SetGameFile() afterward, or use @ref SL_InitWith()
 *       for convenience.
 *
 * @return `true` if initialization succeeded, `false` otherwise.
 *
 * @sa SL_InitWith(), SL_SetGameDir(), SL_SetGameFile()
 *
 * @author Vitor Betmann
 */
bool SL_Init(void);

/**
 * @brief Check if the SaveLoad system has been initialized.
 *
 * @return `true` if the system is initialized, `false` otherwise.
 *
 * @sa SL_Init(), SL_Shutdown()
 */
bool SL_IsInitialized(void);

// Game Dir -----------------------------------------

/**
 * @brief Get the current directory being used for saves/loads.
 *
 * @return A pointer to the current game directory string.
 *
 * @note The returned string is owned by the SaveLoad system. Do not free or modify it.
 *
 * @sa SL_SetGameDir()
 *
 * @author Vitor Betmann
 */
const char *SL_GetGameDir(void);

/**
 * @brief Get the absolute path to the Smile system directory.
 *
 * This returns the full path to the Smile directory created during initialization.
 * For example, on macOS this might look like:
 * `~/Library/Application Support/Smile/`
 *
 * @return A pointer to the system directory string.
 *
 * @note The returned string is owned by the SaveLoad system. Do not free or modify it.
 *
 * @sa SL_Init(), SL_GetGameDir(), SL_GetGamePath()
 *
 * @author Vitor Betmann
 */
const char *SL_GetDefaultDir(void);

/**
 * @brief Set the directory used for save/load operations.
 *
 * This function sets the relative game directory inside the Smile system directory.
 * The directory name is validated and sanitized. If it does not exist,
 * it will be automatically created at this stage.
 *
 * @note If the directory is an empty string (""), save files will be created
 *       directly inside the Smile directory.
 * @note If the same directory is passed again, the function does nothing.
 *
 * @param[in] dir Relative directory path inside the Smile system directory
 *                (e.g., "Breakout/" or "").
 *
 * @return `true` if the directory was successfully set (and created if necessary),
 *         `false` if there was an error (invalid path, memory allocation failure, etc.).
 *
 * @sa SL_GetGameDir(), SL_SetGameFile(), SL_InitWith(), SL_Init()
 *
 * @author Vitor Betmann
 */
bool SL_SetGameDir(const char *dir);

/**
 * Check if a dir exists (relative path).
 */
bool SL_DirExists(const char *dir);

// Game File ----------------------------------------

/**
 * @brief Get the current file being used for saves/loads.
 *
 * @return A pointer to the current game file string.
 *
 * @note The returned string is owned by the SaveLoad system. Do not free or modify it.
 *
 * @author Vitor Betmann
 */
const char *SL_GetGameFile(void);

/**
 * @brief Get the absolute path to the current game save file.
 *
 * This is the concatenation of the default Smile system directory,
 * the current game directory, and the current game file.
 * For example:
 * `~/Library/Application Support/Smile/Breakout/save1.txt`
 *
 * @return A pointer to the full game path string, or `NULL` if either the
 *         game directory or game file has not been set.
 *
 * @note The returned string is owned by the SaveLoad system. Do not free or modify it.
 *
 * @sa SL_SetGameDir(), SL_SetGameFile(), SL_GetDefaultDir(), SL_GetGameDir(), SL_GetGameFile()
 *
 * @author Vitor Betmann
 */
const char *SL_GetGamePath(void);

/**
 * @brief Set the file used for save/load operations.
 *
 * This function sets the relative file name inside the current game directory.
 * The file name is validated and sanitized. It must not be empty.
 *
 * @note The file is not created at this stage. It will be created when the user
 *       starts a save session via @ref SL_BeginSaveSession().
 * @note If the same file name is passed again, the function does nothing.
 *
 * @param[in] file Name of the file to use for saves/loads (e.g., "save1.txt").
 *
 * @return `true` if the file was successfully set, `false` if there was an error
 *         (invalid file name, memory allocation failure, etc.).
 *
 * @sa SL_GetGameFile(), SL_SetGameDir(), SL_InitWith(), SL_Init()
 *
 * @author Vitor Betmann
 */
bool SL_SetGameFile(const char *file);

/**
 * Check if a file exists (relative path).
 */
bool SL_FileExists(const char *file);

// Lists --------------------------------------------

SmileList SL_GetListOf(ListType type);

bool SL_FreeList(SmileList list);

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

bool SL_DeleteDir(const char *dir);

/**
 * Delete a specific file by name (inside the current directory).
 */
bool SL_DeleteFile(const char *file);

// Shutdown -----------------------------------------

/**
 * Shut down the SaveLoad system and free all resources.
 */
bool SL_Shutdown(void);

#endif
