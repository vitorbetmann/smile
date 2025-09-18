// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "SaveLoad.h"
#include "SaveLoadInternal.h"
#include "src/SaveLoad/SaveLoadMessages.h"
#include "src/_Internals/Log/LogInternal.h"
#include "src/_Internals/Log/LogMessages.h"
#include "src/_Internals/Test/TestInternal.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


// --------------------------------------------------
// Defines - Values
// --------------------------------------------------

#define MODULE_NAME "SaveLoad"
#define SMILE_DIR "smile/"

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

static SaveLoadTracker *tracker;

// --------------------------------------------------
// Functions
// --------------------------------------------------

bool SL_Init(const char *gameName) {
    // TODO what if the user provides names with '/'? Or with spaces? Or weird
    // symbols, like .txt at the end?

    if (tracker) {
        SMILE_WARN(MODULE_NAME, LOG_CAUSE_ALREADY_INITIALIZED,
                   LOG_CONSEQ_INIT_ABORTED);
        return false;
    }

    tracker = TEST_Calloc(1, sizeof(SaveLoadTracker));
    if (!tracker) {
        SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);
        return false;
    }

    const char *defaultDir = SL_Internal_GetDefaultSysDir();
    if (!defaultDir) {
        SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);

        free(tracker);
        tracker = nullptr;

        return false;
    }

    size_t gameNameLen = strlen(gameName);
    size_t dirLen =
            strlen(defaultDir) + strlen(SMILE_DIR) + gameNameLen + 2; // '/' + '\0'

    tracker->dirPath = TEST_Malloc(dirLen);
    if (!tracker->dirPath) {
        SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);

        free((void *) defaultDir);
        defaultDir = nullptr;

        free(tracker);
        tracker = nullptr;

        return false;
    }
    snprintf(tracker->dirPath, dirLen, "%s%s%s/", defaultDir, SMILE_DIR,
             gameName);

    size_t smileDirLen = dirLen - gameNameLen - 1;
    char smileDir[smileDirLen];
    snprintf(smileDir, smileDirLen, "%s", tracker->dirPath);

    if (!SL_Internal_DirExists(smileDir)) {
        if (!SL_Internal_CreateDir(smileDir)) {
            SMILE_FATAL(MODULE_NAME, LOG_CAUSE_FAILED_TO_CREATE_SMILE_DIR,
                        LOG_CONSEQ_INIT_ABORTED);
            return false;
        }
    }

    if (!SL_Internal_DirExists(tracker->dirPath)) {
        if (!SL_Internal_CreateDir(tracker->dirPath)) {
            SMILE_FATAL(MODULE_NAME, LOG_CAUSE_FAILED_TO_CREATE_SMILE_DIR,
                        LOG_CONSEQ_INIT_ABORTED);
            return false;
        }
    }

    size_t fileLen = dirLen + gameNameLen + 5; // ".txt" + '\0'
    tracker->filePath = TEST_Malloc(fileLen);
    if (!tracker->filePath) {
        SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);

        free((void *) defaultDir);
        defaultDir = nullptr;

        free(tracker->dirPath);
        tracker->dirPath = nullptr;

        free(tracker);
        tracker = nullptr;

        return false;
    }
    snprintf(tracker->filePath, fileLen, "%s%s.txt", tracker->dirPath, gameName);

    free((void *) defaultDir);
    defaultDir = nullptr;

    SMILE_INFO(MODULE_NAME, LOG_INFO_INIT_SUCCESSFUL);
    return true;
}

bool SL_IsInitialized(void) { return tracker; }

// --------------------------------------------------
// Game Dir
// --------------------------------------------------

const char *SL_GetGameDir(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(LOG_CONSEQ_GET_GAME_DIR_ABORTED);

    return tracker->dirPath;
}

bool SL_SetGameDir(const char *dir) {
    // TODO this func now creates a dir under the gameName folder if it doesn't
    // exist

    if (!dir) {
        SMILE_ERR(MODULE_NAME, LOG_CAUSE_NULL_ARGUMENT,
                  LOG_CONSEQ_DIR_EXISTS_ABORTED);
        return false;
    }

    if (!SL_DirExists(dir)) {
        SMILE_ERR_WITH_NAME(MODULE_NAME, LOG_CAUSE_DIR_NOT_FOUND, dir,
                            LOG_CONSEQ_DIR_EXISTS_ABORTED);
        return false;
    }

    tracker->dirPath = (char *) dir;
    SMILE_INFO(MODULE_NAME, LOG_INFO_SET_GAME_DIR_SUCCESSFUL);
    return true;
}

bool SL_DirExists(const char *dir) {
    if (!dir) {
        return false;
    }

    // TODO look for dir. if found, return true, else false
    return false;
}

// --------------------------------------------------
// Game File
// --------------------------------------------------

bool SL_SetGameFile(const char *file) {
    // TODO this func now creates a dir under the gameName dir if it doesn't exist
    return false;
}

const char *SL_GetGameFile(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(LOG_CONSEQ_GET_GAME_FILE_ABORTED);

    // TODO get root dir name
    return "breakout.txt";
}

bool SL_FileExists(const char *file) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED,
              LOG_CONSEQ_FILE_EXISTS_ABORTED);

    return false;
}

// --------------------------------------------------
// Save
// --------------------------------------------------

bool SL_BeginSaveSession(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_BEGIN_SAVE_SESSION_ABORTED);

    const char *conseq = LOG_CONSEQ_BEGIN_SAVE_SESSION_ABORTED;
    if (SL_Internal_BeginSession(SAVE, tracker->filePath, conseq)) {
        SMILE_INFO(MODULE_NAME, LOG_INFO_SAVE_SESSION_STARTED);
        return true;
    }

    return false;
}

bool SL_SaveNext(const char *data) {
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

bool SL_EndSaveSession() {
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

    tracker->saveStream = nullptr;
    SMILE_INFO(MODULE_NAME, LOG_INFO_SAVE_SESSION_ENDED);
    return true;
}

// --------------------------------------------------
// Load
// --------------------------------------------------

bool SL_BeginLoadSession(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_BEGIN_LOAD_SESSION_ABORTED);

    const char *conseq = LOG_CONSEQ_BEGIN_LOAD_SESSION_ABORTED;
    if (SL_Internal_BeginSession(LOAD, tracker->filePath, conseq)) {
        SMILE_INFO(MODULE_NAME, LOG_INFO_LOAD_SESSION_STARTED);
        return true;
    }

    return false;
}

bool SL_HasNext(void) {
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

char *SL_LoadNext(void) {
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
        return nullptr;
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

bool SL_LoadNextTo(char *dest, size_t size) {
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

bool SL_EndLoadSession(void) {
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

// --------------------------------------------------
// Delete
// --------------------------------------------------

bool SL_DeleteCurrSave(void) { return false; }

// --------------------------------------------------
// Shutdown
// --------------------------------------------------

bool SL_Shutdown(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_SHUTDOWN_ABORTED);

    bool isFatal = TEST_Fatal();

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

const char *SL_Internal_GetDefaultSysDir(void) {
    // TODO improve this, make system dependand like in LOVE2D
    // TODO check if dirPath ends with a '/', and if not, add one at the end
    // This should be responsibility of SL_Internal_GetDirName, I think

    const char *homeDir;
#if defined(__APPLE__) || defined(__linux)
    homeDir = getenv("HOME");
    if (!homeDir) {
        return NULL;
    }
#endif
#ifdef TARGET_OS_WIN32
#endif

#if defined(__APPLE__) || defined(__linux)
    size_t homeDirLen = strlen(homeDir);
    size_t sysDirLen = strlen(DEFAULT_SYS_DIR);
    size_t bufferLen = homeDirLen + sysDirLen + 1;

    char *buffer = TEST_Malloc(bufferLen);
    if (!buffer) {
        return NULL;
    }
    snprintf(buffer, bufferLen, "%s%s", homeDir, DEFAULT_SYS_DIR);

    // Check if dir exists
    if (!SL_Internal_DirExists(buffer)) {
        free(buffer);

        sysDirLen = strlen(ALT_SYS_DIR);
        bufferLen = homeDirLen + sysDirLen + 1;
        buffer = TEST_Malloc(bufferLen);
        snprintf(buffer, bufferLen, "%s%s", homeDir, ALT_SYS_DIR);
    }

#endif
#ifdef _WIN32
#endif

    return buffer;
}

bool SL_Internal_BeginSession(FileInteractionMode mode, const char *file,
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

    char *targetFile = nullptr;
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

bool SL_Internal_CreateDir(const char *dir) {
#ifdef __APPLE__
    constexpr mode_t mode = 0755;
    return mkdir(dir, mode) == 0;

    // // TODO what the hell is this code? From here1
    // char tmp[512];
    // snprintf(tmp, sizeof(tmp), "%s", dir);
    // size_t len = strlen(tmp);
    // if (tmp[len - 1] == '/')
    //   tmp[len - 1] = '\0';

    // for (char *p = tmp + 1; *p; p++) {
    //   if (*p == '/') {
    //     *p = '\0';
    //     if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
    //       return false;
    //     }
    //     *p = '/';
    //   }
    // }
    // return mkdir(tmp, 0755) == 0 || errno == EEXIST;
    // // To here1

#endif
#ifdef __linux__

#endif
#ifdef _WIN32

#endif
}

bool SL_Internal_DirExists(const char *absoluteDir) {
    struct stat buf;
    return stat(absoluteDir, &buf) == 0 && S_ISDIR(buf.st_mode);
}
