#ifndef SAVE_LOAD_SYSTEM_H
#define SAVE_LOAD_SYSTEM_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

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
 * @brief Initializes the save/load system.
 *
 * @param file Optional filename (e.g., "save.txt"). If NULL, defaults to game
 * name.
 * @param dir Optional save directory. If NULL, uses system default (e.g.,
 * "./").
 * @return true if initialized successfully, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_Init(const char *file, const char *dir);

/**
 * @brief Overrides the default game directory for saving and loading.
 *
 * @param dir The new directory path.
 * @return true if the directory is valid and set successfully, false otherwise.
 * @author Vitor Betmann
 */
bool SetGameDir(char *dir);

/**
 * @brief Sets a custom save file name.
 *
 * @param file The file name (e.g., "save1.txt").
 * @return true if the name was valid and stored, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_SetGameFile(char *file);

/**
 * @brief Checks if the specified directory exists.
 *
 * @param dir Path to the directory.
 * @return true if it exists, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_DirExists(char *dir);

/**
 * @brief Checks if a save file exists.
 *
 * @param file File path or name.
 * @return true if it exists, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_FileExists(char *file);

/**
 * @brief Saves a block of data to the default save file.
 *
 * This is a convenience wrapper that:
 * 1. Begins a save session using the default file.
 * 2. Writes the data as a single line (with a trailing newline).
 * 3. Ends the session automatically.
 *
 * @param data Null-terminated string to save.
 * @return true on success, false on failure.
 * @author Vitor Betmann
 */
bool SLS_SaveGame(const char *data);

/**
 * @brief Loads the entire contents of the default save file.
 *
 * This is a convenience wrapper that:
 * 1. Begins a load session using the default file.
 * 2. Reads the entire file contents into a malloc’d null-terminated string.
 * 3. Ends the session automatically.
 *
 * @return Returns a malloc’d null-terminated string. Returns NULL on failure.
 *         Caller must free.
 * @author Vitor Betmann
 */
char *SLS_LoadGame(void);

/**
 * @brief Deletes the current save file.
 *
 * @return true if deleted successfully, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_DeleteSave(void);

/**
 * @brief Begins a save session, allowing incremental writing.
 *
 * @param file Optional override of the default save file.
 *             If NULL, the system will use the default save file set during
 * initialization.
 * @return true if session started, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_BeginSaveSession(const char *file);

/**
 * @brief Saves a single line of data during an active save session.
 *
 * @param data Null-terminated string to save.
 * @return true if written successfully, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_SaveNext(const char *data);

/**
 * @brief Ends the current save session, flushing and closing the file.
 *
 * @return true if closed successfully, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_EndSaveSession(void);

/**
 * @brief Begins a load session, allowing incremental reading.
 *
 * @param file Optional override of the default save file.
 *             If NULL, the system will use the default save file set during
 * initialization.
 * @return true if session started, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_BeginLoadSession(const char *file);

/**
 * @brief Checks if another line is available in the current load session.
 *
 * @return true if more data is available, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_HasNext(void);

/**
 * @brief Reads the next line of text from the open save file.
 *
 * @return Returns a malloc’d null-terminated string containing the next line.
 *         Returns NULL if at EOF, if reading fails, or if allocation fails.
 *         Caller must free.
 * @note The returned string excludes the trailing newline character, if
 * present.
 * @author Vitor Betmann
 */
char *SLS_LoadNext(void);

/**
 * @brief Ends the current load session, closing the file.
 *
 * @return true if closed successfully, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_EndLoadSession(void);

/**
 * @brief Saves data to a specific file inside the configured game directory.
 *
 * @param file File name to save to.
 * @param data Null-terminated string to save.
 * @return true if successful, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_SaveTo(const char *file, const char *data);

/**
 * @brief Loads data from a specific file into a newly allocated buffer.
 *
 * @param file File to load from.
 * @return Returns a malloc’d null-terminated string. Returns NULL on failure.
 * Caller must free.
 * @author Vitor Betmann
 */
char *SLS_LoadFrom(const char *file);

/**
 * @brief Deletes a specific save file inside the configured game directory.
 *
 * @param file File to delete.
 * @return true if deleted successfully, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_DeleteSaveEx(const char *file);

/**
 * @brief Frees all memory and closes any open sessions.
 *
 * @return true if system shutdown cleanly, false otherwise.
 * @author Vitor Betmann
 */
bool SLS_Shutdown(void);

// --------------------------------------------------
// Variables
// --------------------------------------------------

#endif