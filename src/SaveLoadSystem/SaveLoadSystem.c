// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "SaveLoadSystem.h"
#include "SaveLoadSystemInternal.h"
#include "src/SaveLoadSystem/SaveLoadSystemMessages.h"
#include "src/_Internals/Log/LogInternal.h"
#include "src/_Internals/Log/LogMessages.h"
#include <stddef.h>
#include <string.h>

// --------------------------------------------------
// Defines - Values
// --------------------------------------------------
#define MODULE_NAME "SaveLoadSystem"

// --------------------------------------------------
// Defines - Funcs
// --------------------------------------------------
#define RETURN_FALSE_IF_NOT_INITIALIZED(conseq)                                \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED, conseq);               \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RETURN_NULL_IF_NOT_INITIALIZED(conseq)                                 \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED, conseq);               \
      return NULL;                                                             \
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
// bool SLS_SetGameName(char* name){}
// bool SLS_DirExists(char *dir) {}
// bool SLS_FileExists(char *file) {}
// bool SLS_DeleteSave(void) {}

bool SLS_Init(const char *file, const char *dir) {

  if (tracker) {
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_ALREADY_INITIALIZED,
               LOG_CONSEQ_INIT_ABORTED);
    return false;
  }

  tracker = calloc(1, sizeof(SaveLoadSystemTracker));
  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);
    return false;
  }

  tracker->dirPath = dir ? (char *)dir : SLS_Internal_GetDefaultOSDir();
  if (!tracker->dirPath) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DIR_NOT_FOUND, LOG_CONSEQ_INIT_ABORTED);
    free(tracker);
    return false;
  }

  const char *targetFile = file ? file : SLS_Internal_GetGameName();
  size_t fileLen = strlen(targetFile);
  size_t totalLen = strlen(tracker->dirPath) + fileLen + 1;

  tracker->filePath = malloc(totalLen);
  if (!tracker->filePath) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);
    free(tracker->dirPath);
    free(tracker);
    return false;
  }

  int filePathLen = snprintf(tracker->filePath, totalLen, "%s%s",
                             tracker->dirPath, targetFile);
  if (filePathLen < 0 || filePathLen >= totalLen) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED_FORMATTING,
              LOG_CONSEQ_INIT_ABORTED);
    free(tracker->dirPath);
    free(tracker->filePath);
    free(tracker);
    return false;
  }

  SMILE_INFO(MODULE_NAME, LOG_INFO_INIT_SUCCESSFUL);
  return true;
}

bool SLS_BeginSaveSession(const char *file) {

  const char *conseq = LOG_CONSEQ_BEGIN_SAVE_SESSION_ABORTED;
  if (SLS_Internal_BeginSession(SAVE, file, conseq)) {
    SMILE_INFO(MODULE_NAME, LOG_INFO_SAVE_SESSION_STARTED);
    return true;
  }

  return false;
}

bool SLS_SaveNext(const char *data) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_SAVE_NEXT_ABORTED);

  if (!tracker->saveStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_SAVE_SESSION_NOT_OPEN,
              LOG_CONSEQ_SAVE_NEXT_ABORTED);
    return false;
  }

  if (!data) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NULL_DATA, LOG_CONSEQ_SAVE_NEXT_ABORTED);
    return false;
  }

  size_t bufferSize = strlen(data) + 2; // '\n' + '\0'
  char *buffer = malloc(bufferSize);
  if (!buffer) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_SAVE_NEXT_ABORTED);
    return false;
  }

  int bufferLen = snprintf(buffer, bufferSize, "%s\n", data);
  if (bufferLen < 0 || bufferLen >= bufferSize) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED_FORMATTING,
              LOG_CONSEQ_SAVE_NEXT_ABORTED);
    free(buffer);
    return false;
  }

  bool success = true;
  if (fwrite(buffer, bufferLen, 1, tracker->saveStream) != 1) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED_WRITING,
              LOG_CONSEQ_SAVE_NEXT_ABORTED);
    success = false;
  }

  free(buffer);

  if (success) {
    SMILE_INFO(MODULE_NAME, LOG_INFO_SAVE_SUCCESSFUL);
  }
  return success;
}

bool SLS_EndSaveSession() {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_END_SAVE_SESSION_ABORTED);

  if (!tracker->saveStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_SAVE_SESSION_NOT_OPEN,
              LOG_CONSEQ_END_SAVE_SESSION_ABORTED);
    return false;
  }

  if (fclose(tracker->saveStream) == EOF) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_FAILED_TO_CLOSE_FILE,
              LOG_CONSEQ_END_SAVE_SESSION_ABORTED);
    return false;
  }

  tracker->saveStream = NULL;
  SMILE_INFO(MODULE_NAME, LOG_INFO_SAVE_SESSION_ENDED);
  return true;
}

bool SLS_BeginLoadSession(const char *file) {

  const char *conseq = LOG_CONSEQ_BEGIN_LOAD_SESSION_ABORTED;
  if (SLS_Internal_BeginSession(LOAD, file, conseq)) {
    SMILE_INFO(MODULE_NAME, LOG_INFO_LOAD_SESSION_STARTED);
    return true;
  }

  return false;
}

bool SLS_HasNext(void) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_HAS_NEXT_ABORTED);

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_NOT_OPEN,
              LOG_CONSEQ_HAS_NEXT_ABORTED);
    return false;
  }

  int c = fgetc(tracker->loadStream);
  if (c == EOF) {
    return false;
  }

  if (ungetc(c, tracker->loadStream) == EOF) {
    SMILE_FATAL(MODULE_NAME, LOG_CAUSE_INDICATOR_NOT_RESET,
                LOG_CONSEQ_HAS_NEXT_ABORTED);
    return false;
  }

  SMILE_INFO(MODULE_NAME, LOG_INFO_HAS_NEXT_SUCCESSFUL);
  return true;
}

char *SLS_LoadNext(void) {

  RETURN_NULL_IF_NOT_INITIALIZED(LOG_CONSEQ_LOAD_NEXT_ABORTED);

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_NOT_OPEN,
              LOG_CONSEQ_LOAD_NEXT_ABORTED);
    return NULL;
  }

  long startPos = ftell(tracker->loadStream);
  size_t counter = 0;
  int cursor;

  // Find out line length and store it in counter
  while ((cursor = fgetc(tracker->loadStream)) != EOF && cursor != '\n') {
    counter++;
  }

  if (counter == 0 && cursor == EOF) {
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_NO_MORE_DATA,
               LOG_CONSEQ_LOAD_NEXT_ABORTED);
    return NULL;
  }

  size_t bufferSize = counter + 2; // room for '\n' and '\0'
  char *buffer = malloc(bufferSize);
  if (!buffer) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_LOAD_NEXT_ABORTED);
    return NULL;
  }

  if (fseek(tracker->loadStream, startPos, SEEK_SET) != 0) {
    free(buffer);
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_INDICATOR_NOT_RESET,
              LOG_CONSEQ_LOAD_NEXT_ABORTED);
    return NULL;
  }

  if (!fgets(buffer, bufferSize, tracker->loadStream)) {
    free(buffer);
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_ERROR_LOADING_DATA,
               LOG_CONSEQ_LOAD_NEXT_ABORTED);
    return NULL;
  }

  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }

  SMILE_INFO(MODULE_NAME, LOG_INFO_LOAD_SUCCESSFUL);
  return buffer;
}

bool SLS_LoadNextTo(char *dest, size_t size) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_LOAD_NEXT_TO_ABORTED);

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_NOT_OPEN,
              LOG_CONSEQ_LOAD_NEXT_TO_ABORTED);
    return false;
  }

  if (!dest) {
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_INVALID_DEST,
               LOG_CONSEQ_LOAD_NEXT_TO_ABORTED);
    return false;
  }

  if (!fgets(dest, size, tracker->loadStream)) {
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_ERROR_LOADING_DATA,
               LOG_CONSEQ_LOAD_NEXT_TO_ABORTED);
    return false;
  }

  size_t len = strlen(dest);
  if (len > 0 && dest[len - 1] == '\n') {
    dest[len - 1] = '\0';
  }

  SMILE_INFO(MODULE_NAME, LOG_INFO_LOAD_SUCCESSFUL);
  return true;
}

bool SLS_EndLoadSession(void) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_END_LOAD_SESSION_ABORTED);

  if (!tracker->loadStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_NOT_OPEN,
              LOG_CONSEQ_END_LOAD_SESSION_ABORTED);
    return false;
  }

  if (fclose(tracker->loadStream) == EOF) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_FAILED_TO_CLOSE_FILE,
              LOG_CONSEQ_END_LOAD_SESSION_ABORTED);
    return false;
  }

  tracker->loadStream = NULL;
  SMILE_INFO(MODULE_NAME, LOG_INFO_LOAD_SESSION_ENDED);
  return true;
}

bool SLS_Shutdown(void) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_SHUTDOWN_ABORTED);

  bool isFatal = false;

  if (tracker->saveStream) {
    if (fclose(tracker->saveStream) == EOF) {
      isFatal = true;
    }
    tracker->saveStream = NULL;
  }

  if (tracker->loadStream) {
    if (fclose(tracker->loadStream) == EOF) {
      isFatal = true;
    }
    tracker->loadStream = NULL;
  }

  free(tracker->dirPath);
  tracker->dirPath = NULL;

  free(tracker->filePath);
  tracker->filePath = NULL;

  free(tracker);
  tracker = NULL;

  if (isFatal) {
    SMILE_FATAL_WITH_NAME(
        MODULE_NAME, LOG_CAUSE_FAILED_TO_CLOSE_FILE, "name",
        LOG_CONSEQ_SHUTDOWN_ABORTED); // TODO get load file name
    return false;
  }

  SMILE_INFO(MODULE_NAME, LOG_INFO_SHUTDOWN_SUCCESSFUL);
  return true;
}

// --------------------------------------------------
// Internal
// --------------------------------------------------

char *SLS_Internal_GetDefaultOSDir() {

#ifdef __APPLE__
#endif
#ifdef _WIN32
#endif
#ifdef __linux__
#endif

  // TODO improve this, make system dependand like in LOVE2D
  /*
TODO check if dirPath ends with a '/', and if not, add one at the end
This should be responsibility of SLS_Internal_GetDirName, I think
*/
  char *buffer = malloc(3);
  strcpy(buffer, "./");
  return buffer;
}

char *SLS_Internal_GetGameName(void) {

  // TODO get root dir name
  return "breakout.txt";
}

bool SLS_Internal_BeginSession(FileInteractionMode mode, const char *file,
                               const char *conseqAbort) {

  RETURN_FALSE_IF_NOT_INITIALIZED(conseqAbort);

  FILE **currStream = NULL;
  const char *causeAlreadyOpen = NULL;
  const char *openAs = NULL;

  switch (mode) {
  case SAVE:
    if (tracker->saveStream) {
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_SAVE_SESSION_ALREADY_OPEN, conseqAbort);
      return false;
    }
    currStream = &tracker->saveStream;
    openAs = "w";
    break;

  case LOAD:
    if (tracker->loadStream) {
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_LOAD_SESSION_ALREADY_OPEN, conseqAbort);
      return false;
    }
    currStream = &tracker->loadStream;
    openAs = "r";
    break;
  }

  char *targetFile = NULL;
  bool shouldFree = false;

  if (!file) {
    if (!tracker->filePath) {
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_DEST_FILE_NOT_SET, conseqAbort);
      return false;
    }
    targetFile = tracker->filePath;
  } else {
    size_t totalLen = strlen(tracker->dirPath) + strlen(file) + 1;
    targetFile = malloc(totalLen);
    if (!targetFile) {
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, conseqAbort);
      return false;
    }
    size_t written =
        snprintf(targetFile, totalLen, "%s%s", tracker->dirPath, file);
    if (written < 0 || written >= totalLen) {
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_DATA_TRUNCATED_FORMATTING, conseqAbort);
      free(targetFile);
      return false;
    }
    shouldFree = true;
  }

  *currStream = fopen(targetFile, openAs);
  if (!*currStream) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_FAILED_TO_OPEN_FILE, conseqAbort);
    if (shouldFree) {
      free(targetFile);
    }
    return false;
  }

  if (shouldFree) {
    free(targetFile);
  }

  return true;
}
