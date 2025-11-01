#ifndef SAVE_LOAD_SYSTEM_MESSAGES_H
#define SAVE_LOAD_SYSTEM_MESSAGES_H

// --------------------------------------------------
// MODULE NAME
// --------------------------------------------------

#define MODULE "SaveLoad"

// --------------------------------------------------
// FUNCTIONS NAMES
// --------------------------------------------------

#define GET_GAME_DIR "GetGameDir"
#define GET_DEFAULT_DIR "GetDefaultDir"
#define SET_GAME_DIR "SetGameDir"
#define DIR_EXISTS "DirExists"

#define GET_GAME_FILE "GetGameFile"
#define GET_GAME_PATH "GetGamePath"
#define SET_GAME_FILE "SetGameFile"
#define FILE_EXISTS "FileExists"

#define GET_LIST_OF "GetListOf"
#define FreeList "FreeList"

#define BEGIN_SAVE_SESSION "BeginSaveSession"
#define SAVE_NEXT "SaveNext"
#define END_SAVE_SESSION "EndSaveSession"

#define BEGIN_LOAD_SESSION "BeginLoadSession"
#define HAS_NEXT "HasNext"
#define LOAD_NEXT "LoadNext"
#define LOAD_NEXT_TO "LoadNextTo"
#define END_LOAD_SESSION "EndLoadSession"

#define DELETE_DIR "DeleteDir"
#define DELETE_FILE "DeleteFile"

// --------------------------------------------------
// INFO NO ARGS
// --------------------------------------------------

#define INFO_SAVE_SESSION_STARTED "Save session started."
#define INFO_SAVE_SESSION_ENDED "Save session ended."
#define INFO_LOAD_SESSION_STARTED "Load session started."
#define INFO_LOAD_SESSION_ENDED "Load session ended."
#define INFO_SAVE_SUCCESSFUL "Data saved successfully."
#define INFO_LOAD_SUCCESSFUL "Data loaded successfully."
#define INFO_LOAD_TO_SUCCESSFUL "Data loaded to buffer successfully."
#define INFO_HAS_NEXT_SUCCESSFUL "Next data found successfully."

// --------------------------------------------------
// INFO W/ ARGS
// --------------------------------------------------

#define INFO_WITH_ARGS_GAME_DIR_SET "Game directory set to:"
#define INFO_WITH_ARGS_GAME_FILE_SET "Game file set to:"

// --------------------------------------------------
// CAUSE NO ARGS
// --------------------------------------------------

#define CAUSE_GAME_PATH_NOT_SET "SetGameDir or SetGameFile not called."

#define CAUSE_SAVE_SESSION_NOT_OPEN "Save Session not open."
#define CAUSE_SAVE_SESSION_ALREADY_OPEN "Save Session already open."

#define CAUSE_LOAD_SESSION_NOT_OPEN "Load Session not open."
#define CAUSE_LOAD_SESSION_ALREADY_OPEN "Load Session already open."

#define CAUSE_DEST_FILE_NOT_SET "Destination file not set."
#define CAUSE_NULL_DATA "Data is NULL."
#define CAUSE_NO_MORE_DATA "No more data to read."
#define CAUSE_DATA_TRUNCATED_FORMATTING "Data truncated during formatting."
#define CAUSE_DATA_TRUNCATED_WRITING "Data truncated during writing."
#define CAUSE_INDICATOR_NOT_RESET "Failed to reposition stream indicator."
#define CAUSE_FAILED_TO_CREATE_DIR "Failed to create directory."

// --------------------------------------------------
// CAUSE W/ ARGS
// --------------------------------------------------

#define CAUSE_WITH_ARGS_TARGET_DIR_IS_SAME_AS_CURR "Target game dir is same as current: "
#define CAUSE_WITH_ARGS_DIR_NOT_FOUND "Target directory not found: "

#define CAUSE_WITH_ARGS_TARGET_FILE_IS_SAME_AS_CURR "Target game file is same as current: "
#define CAUSE_WITH_ARGS_FILE_NOT_FOUND "Target file not found: "

#define CAUSE_WITH_ARGS_INVALID_PATH "Invalid path: "
#define CAUSE_WITH_ARGS_FAILED_TO_OPEN_FILE "Failed to open file: "
#define CAUSE_WITH_ARGS_FAILED_TO_CLOSE_FILE "Failed to close file: "
#define CAUSE_WITH_ARGS_ERROR_LOADING_DATA "Error loading data from: "

#endif
