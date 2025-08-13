// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "SaveLoadSystem.h"
#include "SaveLoadSystemInternal.h"
#include "src/SaveLoadSystem/SaveLoadSystemMessages.h"
#include "src/_Internals/Log/Log.h"
#include "src/_Internals/Log/LogMessages.h"
#include <string.h>

// --------------------------------------------------
// Defines - Values
// --------------------------------------------------

// --------------------------------------------------
// Defines - Funcs
// --------------------------------------------------
#define RETURN_IF_NOT_INITIALIZED(suffix)                                      \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, LOG_MSG_NOT_INITIALIZED " " suffix);              \
      return false;                                                            \
    }                                                                          \
  } while (0)

// --------------------------------------------------
// Variables
// --------------------------------------------------
static SaveLoadSystemTracker *tracker;

// --------------------------------------------------
// Functions
// --------------------------------------------------

// bool SetGameDir(char *dir) {}
// bool SLS_SetGameFile(char *file) {}
// bool SLS_DirExists(char *dir) {}
// bool SLS_FileExists(char *file) {}

// bool SLS_SaveGame(const char *data) {}
// char *SLS_LoadGame(void) {}
// bool SLS_DeleteSave(void) {}

// bool SLS_SaveTo(const char *file, const char *data) {}
// char *SLS_LoadFrom(const char *file) {}
// bool SLS_DeleteSaveEx(const char *file) {}

bool SLS_Init(const char *file, const char *dir) {

  if (tracker) {
    SMILE_WARN(MODULE_NAME,
               LOG_MSG_ALREADY_INITIALIZED " " LOG_MSG_INIT_ABORTED);
    return false;
  }

  tracker = calloc(1, sizeof(SaveLoadSystemTracker));
  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_MSG_MEM_ALLOC_FAILED " " LOG_MSG_INIT_ABORTED);
    return false;
  }

  tracker->dirPath = SLS_Internal_GetDirName(dir);
  if (!tracker->dirPath) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DIR_NOT_FOUND " " LOG_MSG_INIT_ABORTED);
    free(tracker);
    return false;
  }

  const char *targetFile = file ? file : SLS_Internal_GetGameName();
  int fileLen = strlen(targetFile);
  int totalLen = strlen(tracker->dirPath) + fileLen + 1;

  tracker->filePath = malloc(totalLen);
  if (!tracker->filePath) {
    SMILE_ERR(MODULE_NAME, LOG_MSG_MEM_ALLOC_FAILED " " LOG_MSG_INIT_ABORTED);
    free(tracker->dirPath);
    free(tracker);
    return false;
  }

  // TODO check if dirPath ends with a '/', and if not, add one at the end
  int filePathLen = snprintf(tracker->filePath, totalLen, "%s%s",
                             tracker->dirPath, targetFile);
  if (filePathLen < 0 || filePathLen >= totalLen) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED " " LOG_MSG_INIT_ABORTED);
    free(tracker->dirPath);
    free(tracker->filePath);
    free(tracker);
    return false;
  }

  SMILE_INFO(MODULE_NAME, LOG_MSG_INIT_SUCCESSFUL);
  return true;
}

bool SLS_BeginSaveSession(const char *file) {

  RETURN_IF_NOT_INITIALIZED(LOG_SUFFIX_BEGIN_SAVE_SESSION_ABORTED);

  if (SLS_Internal_BeginSession(SAVE, file)) {
    SMILE_INFO(MODULE_NAME, LOG_MSG_SAVE_SESSION_STARTED);
    return true;
  }

  SMILE_ERR(MODULE_NAME, LOG_SUFFIX_BEGIN_SAVE_SESSION_ABORTED);
  return false;
}

bool SLS_SaveNext(const char *data) {

  RETURN_IF_NOT_INITIALIZED(LOG_SUFFIX_SAVE_NEXT_ABORTED);

  if (!tracker->saveStream) {
    SMILE_ERR(MODULE_NAME,
              LOG_CAUSE_SAVE_SESSION_NOT_OPEN " " LOG_SUFFIX_SAVE_NEXT_ABORTED);
    return false;
  }

  if (!data) {
    SMILE_ERR(MODULE_NAME,
              LOG_CAUSE_NULL_DATA " " LOG_SUFFIX_SAVE_NEXT_ABORTED);
    return false;
  }

  size_t bufferSize = strlen(data) + 2; // '\n' + '\0'
  char *buffer = malloc(bufferSize);
  if (!buffer) {
    SMILE_ERR(MODULE_NAME,
              LOG_MSG_MEM_ALLOC_FAILED " " LOG_SUFFIX_SAVE_NEXT_ABORTED);
    return false;
  }

  int bufferLen = snprintf(buffer, bufferSize, "%s\n", data);
  if (bufferLen < 0 || bufferLen >= bufferSize) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED_FORMATTING
              " " LOG_SUFFIX_SAVE_NEXT_ABORTED);
    free(buffer);
    return false;
  }

  bool success = true;
  if (fwrite(buffer, bufferLen, 1, tracker->saveStream) != 1) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED_WRITING
              " " LOG_SUFFIX_SAVE_NEXT_ABORTED);
    success = false;
  }

  free(buffer);
  SMILE_INFO(MODULE_NAME, LOG_MSG_SAVE_SUCCESSFUL);
  return success;
}

bool SLS_EndSaveSession() {

  RETURN_IF_NOT_INITIALIZED(LOG_SUFFIX_END_SAVE_SESSION_ABORTED);

  if (!tracker->saveStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_SAVE_SESSION_NOT_OPEN
              " " LOG_SUFFIX_END_SAVE_SESSION_ABORTED);
    return false;
  }

  if (fclose(tracker->saveStream) == EOF) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_FAILED_TO_CLOSE_FILE
              " " LOG_SUFFIX_END_SAVE_SESSION_ABORTED);
    return false;
  }

  tracker->saveStream = NULL;
  SMILE_INFO(MODULE_NAME, LOG_MSG_SAVE_SESSION_ENDED);
  return true;
}

bool SLS_BeginLoadSession(const char *file) {

  RETURN_IF_NOT_INITIALIZED(LOG_SUFFIX_BEGIN_LOAD_SESSION_ABORTED);

  if (SLS_Internal_BeginSession(LOAD, file)) {
    SMILE_INFO(MODULE_NAME, LOG_MSG_LOAD_SESSION_STARTED);
    return true;
  }

  SMILE_ERR(MODULE_NAME, LOG_SUFFIX_BEGIN_LOAD_SESSION_ABORTED);
  return false;
}

bool SLS_HasNext(void) {

  RETURN_IF_NOT_INITIALIZED(LOG_SUFFIX_HAS_NEXT_ABORTED);

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME,
              LOG_CAUSE_LOAD_SESSION_NOT_OPEN " " LOG_SUFFIX_HAS_NEXT_ABORTED);
    return false;
  }

  int pos = ftell(tracker->loadStream);

  if (fgetc(tracker->loadStream) == EOF) {
    return false;
  }

  if (fseek(tracker->loadStream, pos, SEEK_SET) != 0) {
    SMILE_ERR(MODULE_NAME,
              LOG_CAUSE_INDICATOR_NOT_RESET " " LOG_SUFFIX_HAS_NEXT_ABORTED);
    return false;
  }

  SMILE_INFO(MODULE_NAME, LOG_MSG_HAS_NEXT_SUCCESSFUL);
  return true;
}

char *SLS_LoadNext(void) {

  if (!tracker) {
    SMILE_ERR(MODULE_NAME,
              LOG_MSG_NOT_INITIALIZED " " LOG_SUFFIX_LOAD_NEXT_ABORTED);
    return NULL;
  }

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME,
              LOG_CAUSE_LOAD_SESSION_NOT_OPEN " " LOG_SUFFIX_LOAD_NEXT_ABORTED);
    return NULL;
  }

  long startPos = ftell(tracker->loadStream);
  long counter = 0;
  int cursor;

  // Find out line length and store it in counter
  while ((cursor = fgetc(tracker->loadStream)) != EOF && cursor != '\n') {
    counter++;
  }

  if (counter == 0 && cursor == EOF) {
    SMILE_WARN(MODULE_NAME,
               LOG_CAUSE_NO_MORE_DATA " " LOG_SUFFIX_LOAD_NEXT_ABORTED);
    return NULL;
  }

  int bufferSize = counter + 2; // room for '\n' and '\0'
  char *buffer = malloc(bufferSize);
  if (!buffer) {
    SMILE_ERR(MODULE_NAME,
              LOG_MSG_MEM_ALLOC_FAILED " " LOG_SUFFIX_LOAD_NEXT_ABORTED);
    return NULL;
  }

  if (fseek(tracker->loadStream, startPos, SEEK_SET) != 0) {
    free(buffer);
    SMILE_ERR(MODULE_NAME,
              LOG_CAUSE_INDICATOR_NOT_RESET " " LOG_SUFFIX_LOAD_NEXT_ABORTED);
    return NULL;
  }

  if (!fgets(buffer, bufferSize, tracker->loadStream)) {
    free(buffer);
    SMILE_WARN(MODULE_NAME,
               LOG_CAUSE_ERROR_LOADING_DATA " " LOG_SUFFIX_LOAD_NEXT_ABORTED);
    return NULL;
  }

  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }

  SMILE_INFO(MODULE_NAME, LOG_MSG_LOAD_SUCCESSFUL);
  return buffer;
}

bool SLS_LoadNextTo(char *dest, size_t size) {

  RETURN_IF_NOT_INITIALIZED(LOG_SUFFIX_LOAD_NEXT_TO_ABORTED);

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_NOT_OPEN
              " " LOG_SUFFIX_LOAD_NEXT_TO_ABORTED);
    return NULL;
  }

  if (!dest) {
    SMILE_WARN(MODULE_NAME,
               LOG_CAUSE_INVALID_DEST " " LOG_SUFFIX_LOAD_NEXT_TO_ABORTED);
    return false;
  }

  if (!fgets(dest, size, tracker->loadStream)) {
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_ERROR_LOADING_DATA
               " " LOG_SUFFIX_LOAD_NEXT_TO_ABORTED);
    return false;
  }

  size_t len = strlen(dest);
  if (len > 0 && dest[len - 1] == '\n') {
    dest[len - 1] = '\0';
  }

  SMILE_INFO(MODULE_NAME, LOG_MSG_LOAD_SUCCESSFUL);
  return true;
}

bool SLS_EndLoadSession() {

  RETURN_IF_NOT_INITIALIZED(LOG_SUFFIX_END_LOAD_SESSION_ABORTED);

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_NOT_OPEN
              " " LOG_SUFFIX_END_LOAD_SESSION_ABORTED);
    return NULL;
  }

  if (fclose(tracker->loadStream) == EOF) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_FAILED_TO_CLOSE_FILE
              " " LOG_SUFFIX_END_LOAD_SESSION_ABORTED);
    return false;
  }

  tracker->loadStream = NULL;
  SMILE_INFO(MODULE_NAME, LOG_MSG_LOAD_SESSION_ENDED);
  return true;
}

bool SLS_Shutdown(void) {

  RETURN_IF_NOT_INITIALIZED();
  if (tracker->saveStream) {
    fclose(tracker->saveStream);
    tracker->saveStream = NULL;
  }

  if (tracker->loadStream) {
    fclose(tracker->loadStream);
    tracker->loadStream = NULL;
  }

  free(tracker->dirPath);
  tracker->dirPath = NULL;

  free(tracker->filePath);
  tracker->filePath = NULL;

  free(tracker);
  tracker = NULL;

  return true;
}

// --------------------------------------------------
// Internal
// --------------------------------------------------

char *SLS_Internal_GetDirName(const char *dir) {

  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_MSG_NOT_INITIALIZED
              " " LOG_SUFFIX_INTERNAL_GET_DIR_NAME_ABORTED);
    return NULL;
  }

  // TODO improve this, make system dependand like in LOVE2D
  char *buffer = malloc(3);
  strcpy(buffer, "./");
  return buffer;
}

char *SLS_Internal_GetGameName(void) {

  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_MSG_NOT_INITIALIZED
              " " LOG_SUFFIX_INTERNAL_GET_GAME_NAME_ABORTED);
    return NULL;
  }

  // TODO get root dir name
  return "breakout.txt";
}

bool SLS_Internal_BeginSession(FileInteractionMode mode, const char *file) {

  RETURN_IF_NOT_INITIALIZED();
  FILE **currStream;
  char *openAs;
  switch (mode) {
  case SAVE:
    if (tracker->saveStream) {
      SMILE_WARN(MODULE_NAME, LOG_CAUSE_SAVE_SESSION_ALREADY_OPEN);
      return false;
    }
    currStream = &tracker->saveStream;
    openAs = "w";
    break;
  case LOAD:
    if (tracker->loadStream) {
      SMILE_WARN(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_ALREADY_OPEN);
      return false;
    }
    currStream = &tracker->loadStream;
    openAs = "r";
    break;
  }

  char *targetFile;
  bool shouldFree = false;

  if (!file) {
    if (!tracker->filePath) {
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_DEST_FILE_NOT_SET);
      return false;
    }
    targetFile = tracker->filePath;
  } else {
    int totalLen = strlen(tracker->dirPath) + strlen(file) + 1;
    targetFile = malloc(totalLen);
    if (!targetFile) {
      SMILE_ERR(MODULE_NAME, LOG_MSG_MEM_ALLOC_FAILED);
      return false;
    }

    // TODO: check if dirPath ends with '/', or use safe path join helper
    int targetFileLen =
        snprintf(targetFile, totalLen, "%s%s", tracker->dirPath, file);
    if (targetFileLen < 0 || targetFileLen >= totalLen) {
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED_FORMATTING);
      free(targetFile);
      return false;
    }
    shouldFree = true;
  }

  bool success = true;
  *currStream = fopen(targetFile, openAs);
  if (!*currStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_FILE_NOT_FOUND);
    success = false;
  }

  if (shouldFree) {
    free(targetFile);
  }

  return success;
}