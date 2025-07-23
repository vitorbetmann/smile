// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "SaveLoadSystem.h"
#include "SaveLoadSystemInternal.h"
#include <string.h>

// --------------------------------------------------
// Defines - Values
// --------------------------------------------------

// --------------------------------------------------
// Defines - Funcs
// --------------------------------------------------

#define ASSERT_SLS_INITIALIZED_OR_RETURN_FALSE()                               \
  if (!tracker) {                                                              \
    /* TODO add error SaveLoadSystemNotInitialized */                          \
    return false;                                                              \
  }

#define ASSERT_SLS_INITIALIZED_OR_RETURN_NULL()                                \
  if (!tracker) {                                                              \
    /* TODO add error SaveLoadSystemNotInitialized */                          \
    return NULL;                                                               \
  }

#define ASSERT_SAVE_SESSION_CLOSED_OR_RETURN_FALSE()                           \
  if (tracker->saveStream) {                                                   \
    /* TODO add SaveSessionOpenError */                                        \
    return false;                                                              \
  }

#define ASSERT_SAVE_SESSION_OPEN_OR_RETURN_FALSE()                             \
  if (!tracker->saveStream) {                                                  \
    /* TODO add SaveSessionClosedError */                                      \
    return false;                                                              \
  }

#define ASSERT_LOAD_SESSION_CLOSED_OR_RETURN_FALSE()                           \
  if (tracker->loadStream) {                                                   \
    /* TODO add SaveSessionOpenError */                                        \
    return false;                                                              \
  }

#define ASSERT_LOAD_SESSION_OPEN_OR_RETURN_NULL()                              \
  if (!tracker->loadStream) {                                                  \
    /* TODO add SaveSessionOpenError */                                        \
    return NULL;                                                               \
  }

#define ASSERT_LOAD_SESSION_OPEN_OR_RETURN_FALSE()                             \
  if (!tracker->loadStream) {                                                  \
    /* TODO add SaveSessionClosedError */                                      \
    return false;                                                              \
  }

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
    // TODO add error SaveLoadSystemAlreadyInitialized
    return false;
  }

  tracker = calloc(1, sizeof(SaveLoadSystemTracker));
  if (!tracker) {
    // TODO add MemAllocFailedError
    return false;
  }

  tracker->dirPath = SLS_Internal_GetDirName(dir);
  if (!tracker->dirPath) {
    // TODO add TargetDirNotFoundError
    free(tracker);
    return false;
  }

  const char *targetFile = file ? file : SLS_Internal_GetGameName();
  int fileLen = strlen(targetFile);
  int totalLen = strlen(tracker->dirPath) + fileLen + 1;

  tracker->filePath = malloc(totalLen);
  if (!tracker->filePath) {
    // TODO add MemAllocFailedError
    free(tracker->dirPath);
    free(tracker);
    return false;
  }

  // TODO check if dirPath ends with a '/', and if not, add one at the end
  int filePathLen = snprintf(tracker->filePath, totalLen, "%s%s",
                             tracker->dirPath, targetFile);
  if (filePathLen < 0 || filePathLen >= totalLen) {
    free(tracker->dirPath);
    free(tracker->filePath);
    free(tracker);
    // TODO add DataTruncatedError
    return false;
  }

  return true;
}

bool SLS_BeginSaveSession(const char *file) {

  return SLS_Internal_BeginSession(SAVE, file);
}

bool SLS_SaveNext(const char *data) {

  ASSERT_SLS_INITIALIZED_OR_RETURN_FALSE();
  ASSERT_SAVE_SESSION_OPEN_OR_RETURN_FALSE();

  if (!data) {
    // TODO add NullDataError
    return false;
  }

  size_t bufferSize = strlen(data) + 2;
  char *buffer = malloc(bufferSize);
  if (!buffer)
    // TODO add MemAllocFailedError
    return false;

  int bufferLen = snprintf(buffer, bufferSize, "%s\n", data);
  if (bufferLen < 0 || bufferLen >= bufferSize) {
    // TODO add DataTruncatedError
    free(buffer);
    return false;
  }

  bool success = true;
  if (fwrite(buffer, bufferLen, 1, tracker->saveStream) != 1) {
    // TODO add DataNotSavedError
    success = false;
  }

  free(buffer);

  return success;
}

bool SLS_EndSaveSession() {

  ASSERT_SLS_INITIALIZED_OR_RETURN_FALSE();
  ASSERT_SAVE_SESSION_OPEN_OR_RETURN_FALSE();

  if (fclose(tracker->saveStream) == EOF) {
    // TODO add FileFailedToCloseProperlyError
    return false;
  }

  tracker->saveStream = NULL;
  return true;
}

bool SLS_BeginLoadSession(const char *file) {

  return SLS_Internal_BeginSession(LOAD, file);
}

bool SLS_HasNext(void) {

  ASSERT_SLS_INITIALIZED_OR_RETURN_FALSE();
  ASSERT_LOAD_SESSION_OPEN_OR_RETURN_FALSE();

  int pos = ftell(tracker->loadStream);

  if (fgetc(tracker->loadStream) == EOF) {
    return false;
  }

  if (fseek(tracker->loadStream, pos, SEEK_SET) != 0) {
    // TODO add CorruptedDataError
    return false;
  }
  return true;
}

char *SLS_LoadNext(void) {

  ASSERT_SLS_INITIALIZED_OR_RETURN_NULL();
  ASSERT_LOAD_SESSION_OPEN_OR_RETURN_NULL();

  long startPos = ftell(tracker->loadStream);
  long counter = 0;
  int cursor;

  while ((cursor = fgetc(tracker->loadStream)) != EOF && cursor != '\n') {
    counter++;
  }

  if (counter == 0 && cursor == EOF) {
    // TODO add EOFReachedOrEmptyLineWarn
    return NULL;
  }

  int bufferSize = counter + 2; // room for '\n' and '\0'
  char *buffer = malloc(bufferSize);
  if (!buffer) {
    // TODO add MemAllocFailedError
    return NULL;
  }

  if (fseek(tracker->loadStream, startPos, SEEK_SET) != 0) {
    free(buffer);
    // TODO add CorruptedDataError
    return NULL;
  }

  if (!fgets(buffer, bufferSize, tracker->loadStream)) {
    free(buffer);
    // TODO add CorruptedDataError or EOFReachedWarn
    return NULL;
  }

  size_t len = strlen(buffer);
  // len should never be 0, but you never know...
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }

  return buffer;
}

bool SLS_LoadNextTo(char *dest, size_t size) {

  ASSERT_SLS_INITIALIZED_OR_RETURN_FALSE();
  ASSERT_LOAD_SESSION_OPEN_OR_RETURN_FALSE();

  if (!dest) {
    // TODO add InvalidDestWarn
    return false;
  }

  if (!fgets(dest, size, tracker->loadStream)) {
    // TODO add CorruptedDataError or EOFReachedWarn
    return false;
  }

  size_t len = strlen(dest);
  if (len > 0 && dest[len - 1] == '\n') {
    dest[len - 1] = '\0';
  }

  return true;
}

bool SLS_EndLoadSession() {

  ASSERT_LOAD_SESSION_OPEN_OR_RETURN_FALSE();

  if (fclose(tracker->loadStream) == EOF) {
    return false;
  }

  tracker->loadStream = NULL;
  return true;
}

bool SLS_Shutdown(void) {

  ASSERT_SLS_INITIALIZED_OR_RETURN_FALSE();

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

  // TODO improve this, make system dependand like in LOVE2D

  ASSERT_SLS_INITIALIZED_OR_RETURN_NULL();

  char *buffer = malloc(3);
  strcpy(buffer, "./");
  return buffer;
}

char *SLS_Internal_GetGameName(void) {

  ASSERT_SLS_INITIALIZED_OR_RETURN_NULL();

  // TODO get root dir name
  return "breakout.txt";
}

bool SLS_Internal_BeginSession(FileInteractionMode mode, const char *file) {

  ASSERT_SLS_INITIALIZED_OR_RETURN_FALSE();

  FILE **currStream;
  char *openAs;
  switch (mode) {
  case SAVE:
    ASSERT_SAVE_SESSION_CLOSED_OR_RETURN_FALSE();
    currStream = &tracker->saveStream;
    openAs = "w";
    break;
  case LOAD:
    ASSERT_LOAD_SESSION_CLOSED_OR_RETURN_FALSE();
    currStream = &tracker->loadStream;
    openAs = "r";
    break;
  }

  char *targetFile;
  bool shouldFree = false;

  if (!file) {
    if (!tracker->filePath) {
      // TODO add DestFileNotSetError
      return false;
    }
    targetFile = tracker->filePath;
  } else {
    int totalLen = strlen(tracker->dirPath) + strlen(file) + 1;
    targetFile = malloc(totalLen);
    if (!targetFile) {
      // TODO add MemAllocFailedError
      return false;
    }

    // TODO: check if dirPath ends with '/', or use safe path join helper
    int targetFileLen =
        snprintf(targetFile, totalLen, "%s%s", tracker->dirPath, file);
    if (targetFileLen < 0 || targetFileLen >= totalLen) {
      // TODO add DataTruncatedError
      free(targetFile);
      return false;
    }
    shouldFree = true;
  }

  bool success = true;
  *currStream = fopen(targetFile, openAs);
  if (!*currStream) {
    // TODO add FileNotFoundError
    success = false;
  }

  if (shouldFree) {
    free(targetFile);
  }

  return success;
}