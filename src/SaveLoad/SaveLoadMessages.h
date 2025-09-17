#ifndef SAVE_LOAD_SYSTEM_MESSAGES_H
#define SAVE_LOAD_SYSTEM_MESSAGES_H

// --------------------------------------------------
// INFO no arguments
// --------------------------------------------------
#define LOG_INFO_INIT_SUCCESSFUL "Init successful."
#define LOG_INFO_SET_GAME_DIR_SUCCESSFUL "SetGameDir successful."
#define LOG_INFO_SAVE_SESSION_STARTED "Save session started."
#define LOG_INFO_SAVE_SESSION_ENDED "Save session ended."
#define LOG_INFO_LOAD_SESSION_STARTED "Load session started."
#define LOG_INFO_LOAD_SESSION_ENDED "Load session ended."
#define LOG_INFO_SAVE_SUCCESSFUL "Data saved successfully."
#define LOG_INFO_LOAD_SUCCESSFUL "Data loaded successfully."
#define LOG_INFO_LOAD_TO_SUCCESSFUL "Data loaded to buffer successfully."
#define LOG_INFO_HAS_NEXT_SUCCESSFUL "Next data found successfully."

// --------------------------------------------------
// INFO w/ arguments
// --------------------------------------------------

// --------------------------------------------------
// CAUSE no arguments
// --------------------------------------------------
#define LOG_CAUSE_SAVE_SESSION_NOT_OPEN "Save Session not open."
#define LOG_CAUSE_LOAD_SESSION_NOT_OPEN "Load Session not open."
#define LOG_CAUSE_SAVE_SESSION_ALREADY_OPEN "Save Session already open."
#define LOG_CAUSE_LOAD_SESSION_ALREADY_OPEN "Load Session already open."
#define LOG_CAUSE_DEST_FILE_NOT_SET "Destination file not set."
#define LOG_CAUSE_NULL_DATA "Data is NULL."
#define LOG_CAUSE_NULL_ARGUMENT "Argument is NULL."
#define LOG_CAUSE_INVALID_DEST "Destination buffer is NULL."
#define LOG_CAUSE_NO_MORE_DATA "No more data to read."
#define LOG_CAUSE_DATA_TRUNCATED_FORMATTING "Data truncated during formatting."
#define LOG_CAUSE_DATA_TRUNCATED_WRITING "Data truncated during writing."
#define LOG_CAUSE_INDICATOR_NOT_RESET "Failed to reposition stream indicator."
#define LOG_CAUSE_FAILED_TO_CREATE_SMILE_DIR "Failed to create SMILE directory."

// --------------------------------------------------
// CAUSE w/ arguments
// --------------------------------------------------
#define LOG_CAUSE_FAILED_TO_OPEN_FILE "Failed to open file: "
#define LOG_CAUSE_FAILED_TO_CLOSE_FILE "Failed to close file: "
#define LOG_CAUSE_DIR_NOT_FOUND "Target directory not found: "
#define LOG_CAUSE_FILE_NOT_FOUND "Target file not found: "
#define LOG_CAUSE_ERROR_LOADING_DATA "Error loading data from: "

// --------------------------------------------------
// CONSEQes - Public
// --------------------------------------------------
#define LOG_CONSEQ_SAVE_NEXT_ABORTED "SaveNext aborted."
#define LOG_CONSEQ_BEGIN_SAVE_SESSION_ABORTED "BeginSaveSession aborted."
#define LOG_CONSEQ_END_SAVE_SESSION_ABORTED "EndSaveSession aborted."
#define LOG_CONSEQ_BEGIN_LOAD_SESSION_ABORTED "BeginLoadSession aborted."
#define LOG_CONSEQ_HAS_NEXT_ABORTED "HasNext aborted."
#define LOG_CONSEQ_END_LOAD_SESSION_ABORTED "EndLoadSession aborted."
#define LOG_CONSEQ_LOAD_NEXT_ABORTED "LoadNext aborted."
#define LOG_CONSEQ_LOAD_NEXT_TO_ABORTED "LoadNextTo aborted."
#define LOG_CONSEQ_SHUTDOWN_ABORTED "Shutdown aborted."
#define LOG_CONSEQ_LOAD_GAME_ABORTED "LoadGame aborted."
#define LOG_CONSEQ_DIR_EXISTS_ABORTED "SetGameDir aborted."
#define LOG_CONSEQ_GET_GAME_DIR_ABORTED "GetGameDir aborted."
#define LOG_CONSEQ_GET_GAME_FILE_ABORTED "GetGameFile aborted."
#define LOG_CONSEQ_FILE_EXISTS_ABORTED "FileExists aborted."

// --------------------------------------------------
// CONSEQes - Internal
// --------------------------------------------------
#define LOG_CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED                              \
  "Internal BeginSession aborted."
#define LOG_CONSEQ_INTERNAL_GET_DEFAULT_SYS_DIR_ABORTED                        \
  "Internal GetDefaultSysDir aborted."

// --------------------------------------------------
// CONSEQes - Tests
// --------------------------------------------------

#endif