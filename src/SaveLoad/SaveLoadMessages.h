#ifndef SAVE_LOAD_SYSTEM_MESSAGES_H
#define SAVE_LOAD_SYSTEM_MESSAGES_H

// --------------------------------------------------
// INFO no arguments
// --------------------------------------------------
#define INFO_INIT_SUCCESSFUL "Init successful."
#define INFO_SAVE_SESSION_STARTED "Save session started."
#define INFO_SAVE_SESSION_ENDED "Save session ended."
#define INFO_LOAD_SESSION_STARTED "Load session started."
#define INFO_LOAD_SESSION_ENDED "Load session ended."
#define INFO_SAVE_SUCCESSFUL "Data saved successfully."
#define INFO_LOAD_SUCCESSFUL "Data loaded successfully."
#define INFO_LOAD_TO_SUCCESSFUL "Data loaded to buffer successfully."
#define INFO_HAS_NEXT_SUCCESSFUL "Next data found successfully."

// --------------------------------------------------
// INFO w/ arguments
// --------------------------------------------------

#define INFO_WITH_ARGS_SET_GAME_DIR_SUCCESSFUL "SetGameDir successful: "
#define INFO_WITH_ARGS_SET_GAME_FILE_SUCCESSFUL "SetGameFile successful: "

// --------------------------------------------------
// CAUSE no arguments
// --------------------------------------------------
#define CAUSE_SAVE_SESSION_NOT_OPEN "Save Session not open."
#define CAUSE_LOAD_SESSION_NOT_OPEN "Load Session not open."
#define CAUSE_SAVE_SESSION_ALREADY_OPEN "Save Session already open."
#define CAUSE_LOAD_SESSION_ALREADY_OPEN "Load Session already open."
#define CAUSE_DEST_FILE_NOT_SET "Destination file not set."
#define CAUSE_NULL_DATA "Data is NULL."
#define CAUSE_NULL_ARGUMENT "Argument is NULL."
#define CAUSE_NO_MORE_DATA "No more data to read."
#define CAUSE_DATA_TRUNCATED_FORMATTING "Data truncated during formatting."
#define CAUSE_DATA_TRUNCATED_WRITING "Data truncated during writing."
#define CAUSE_INDICATOR_NOT_RESET "Failed to reposition stream indicator."
#define CAUSE_FAILED_TO_CREATE_DIR "Failed to create directory."

// --------------------------------------------------
// CAUSE w/ arguments
// --------------------------------------------------
#define CAUSE_WITH_ARGS_INVALID_PATH "Invalid path: "
#define CAUSE_WITH_ARGS_FAILED_TO_OPEN_FILE "Failed to open file: "
#define CAUSE_WITH_ARGS_FAILED_TO_CLOSE_FILE "Failed to close file: "
#define CAUSE_WITH_ARGS_DIR_NOT_FOUND "Target directory not found: "
#define CAUSE_WITH_ARGS_FILE_NOT_FOUND "Target file not found: "
#define CAUSE_WITH_ARGS_TARGET_FILE_IS_SAME_AS_CURR "Target file is same as current: "
#define CAUSE_WITH_ARGS_ERROR_LOADING_DATA "Error loading data from: "


// --------------------------------------------------
// CONSEQes - Public
// --------------------------------------------------
#define CONSEQ_SAVE_NEXT_ABORTED "SaveNext aborted."
#define CONSEQ_BEGIN_SAVE_SESSION_ABORTED "BeginSaveSession aborted."
#define CONSEQ_END_SAVE_SESSION_ABORTED "EndSaveSession aborted."
#define CONSEQ_BEGIN_LOAD_SESSION_ABORTED "BeginLoadSession aborted."
#define CONSEQ_HAS_NEXT_ABORTED "HasNext aborted."
#define CONSEQ_END_LOAD_SESSION_ABORTED "EndLoadSession aborted."
#define CONSEQ_LOAD_NEXT_ABORTED "LoadNext aborted."
#define CONSEQ_LOAD_NEXT_TO_ABORTED "LoadNextTo aborted."
#define CONSEQ_SHUTDOWN_ABORTED "Shutdown aborted."
#define CONSEQ_LOAD_GAME_ABORTED "LoadGame aborted."
#define CONSEQ_GET_GAME_DIR_ABORTED "GetGameDir aborted."
#define CONSEQ_GET_GAME_FILE_ABORTED "GetGameFile aborted."
#define CONSEQ_SET_GAME_DIR_ABORTED "SetGameDir aborted."
#define CONSEQ_SET_GAME_FILE_ABORTED "SetGameFile aborted."
#define CONSEQ_FILE_EXISTS_ABORTED "FileExists aborted."
#define CAUSE_GAME_PATH_NOT_SET "Game directory or file or set."

// --------------------------------------------------
// CONSEQes - Internal
// --------------------------------------------------
#define CONSEQ_INTERNAL_BEGIN_SESSION_ABORTED                              \
  "Internal BeginSession aborted."
#define CONSEQ_INTERNAL_GET_DEFAULT_SYS_DIR_ABORTED                        \
  "Internal GetDefaultSysDir aborted."

// --------------------------------------------------
// CONSEQes - Tests
// --------------------------------------------------

#endif
