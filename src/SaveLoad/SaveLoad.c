// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "SaveLoad.h"
#include "SaveLoadInternal.h"
#include "src/SaveLoad/SaveLoadMessages.h"
#include "src/_Internals/Log/LogInternal.h"
#include "src/_Internals/Log/LogMessages.h"
#include "src/_Internals/Test/TestInternal.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

#define RETURN_FALSE_IF_NOT_INITIALIZED(conseq)                                \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE, CAUSE_NOT_INITIALIZED, conseq);                        \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RETURN_NULL_IF_NOT_INITIALIZED(conseq)                                 \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE, CAUSE_NOT_INITIALIZED, conseq);                        \
      return nullptr;                                                          \
    }                                                                          \
  } while (0)

// --------------------------------------------------
// Variables
// --------------------------------------------------

static SaveLoadTracker *tracker;

// --------------------------------------------------
// Functions
// --------------------------------------------------

// Init ---------------------------------------------

bool SL_InitWith(const char *dir, const char *file) {
    if (!SL_Init()) { return false; }
    if (!SL_SetGameDir(dir)) { return false; }
    if (!SL_SetGameFile(file)) { return false; }
    return true;
}

bool SL_Init(void) {
    if (tracker) {
        SMILE_WARN(MODULE, CAUSE_ALREADY_INITIALIZED, CONSEQ_INIT_ABORTED);
        return false;
    }

    tracker = TEST_Calloc(1, sizeof(SaveLoadTracker));
    if (!tracker) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_INIT_ABORTED);
        return false;
    }

    char *sysDir = SL_Internal_GetDefaultSysDir();
    if (!sysDir) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_INIT_ABORTED);

        free(tracker);
        tracker = nullptr;

        return false;
    }

    tracker->defaultDir = TEST_Malloc(strlen(sysDir) + strlen(SMILE_DIR) + 1);
    if (!tracker->defaultDir) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_INIT_ABORTED);

        free(sysDir);

        free(tracker);
        tracker = nullptr;

        return false;
    }

    snprintf(tracker->defaultDir, strlen(sysDir) + strlen(SMILE_DIR) + 1, "%s%s", sysDir, SMILE_DIR);
    free(sysDir);

    if (!SL_Internal_DirExists(tracker->defaultDir)) {
        if (!SL_Internal_CreateDir(tracker->defaultDir)) {
            SMILE_FATAL_WITH_ARGS(MODULE, CAUSE_FAILED_TO_CREATE_DIR, SMILE_DIR, CONSEQ_SET_GAME_DIR_ABORTED);

            free(tracker->defaultDir);
            tracker->defaultDir = nullptr;

            free(tracker);
            tracker = nullptr;

            return false;
        }
    }

    SMILE_INFO(MODULE, INFO_INIT_SUCCESSFUL);
    return true;
}

bool SL_IsInitialized(void) { return tracker; }

// Game Dir ------------------------------------------

const char *SL_GetGameDir(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(CONSEQ_GET_GAME_DIR_ABORTED);

    return tracker->gameDir;
}

const char *SL_GetDefaultDir(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(CONSEQ_GET_DEFAULT_DIR_ABORTED);

    return tracker->defaultDir;
}

bool SL_SetGameDir(const char *dir) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_SET_GAME_DIR_ABORTED);

    if (!dir) {
        SMILE_ERR(MODULE, CAUSE_NULL_ARGUMENT, CONSEQ_SET_GAME_DIR_ABORTED);
        return false;
    }

    if (tracker->gameDir && strcmp(dir, tracker->gameDir) == 0) {
        SMILE_WARN_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_TARGET_DIR_IS_SAME_AS_CURR, dir, CONSEQ_SET_GAME_DIR_ABORTED);
        return true;
    }

    if (!SL_Internal_IsValidDir(dir)) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_INVALID_PATH, dir, CONSEQ_SET_GAME_DIR_ABORTED);
        return false;
    }

    char *sanitizedDir = SL_Internal_SanitizeDir(dir);
    if (!sanitizedDir) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_SET_GAME_DIR_ABORTED);
        return false;
    }

    const size_t gameDirLen = strlen(tracker->defaultDir) + strlen(sanitizedDir) + 1; // '\0'

    free(tracker->gameDir);
    tracker->gameDir = TEST_Malloc(gameDirLen);
    if (!tracker->gameDir) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_SET_GAME_DIR_ABORTED);
        free(sanitizedDir);
        return false;
    }

    snprintf(tracker->gameDir, gameDirLen, "%s%s", tracker->defaultDir, sanitizedDir);
    free(sanitizedDir);


    if (!SL_Internal_DirExists(tracker->gameDir)) {
        if (!SL_Internal_CreateDir(tracker->gameDir)) {
            SMILE_FATAL(MODULE, CAUSE_FAILED_TO_CREATE_DIR, CONSEQ_SET_GAME_DIR_ABORTED);
            return false;
        }
    }

    if (tracker->gameFile) {
        if (!SL_Internal_UpdateGamePath()) {
            SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_SET_GAME_DIR_ABORTED);
            return false;
        }
    }

    SMILE_INFO_WITH_NAME(MODULE, INFO_WITH_ARGS_SET_GAME_DIR_SUCCESSFUL, tracker->gameDir);
    return true;
}

bool SL_DirExists(const char *dir) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_DIR_EXISTS_ABORTED);

    if (!dir) {
        return false;
    }

    return true;
}

// Game File -----------------------------------------

const char *SL_GetGameFile(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(CONSEQ_GET_GAME_FILE_ABORTED);

    return tracker->gameFile;
}

const char *SL_GetGamePath(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(CONSEQ_GET_GAME_PATH_ABORTED);

    return tracker->gamePath;
}

bool SL_SetGameFile(const char *file) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_SET_GAME_FILE_ABORTED);

    if (!file) {
        SMILE_ERR(MODULE, CAUSE_NULL_ARGUMENT, CONSEQ_SET_GAME_FILE_ABORTED);
        return false;
    }

    if (tracker->gameFile && strcmp(file, tracker->gameFile) == 0) {
        SMILE_WARN_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_TARGET_FILE_IS_SAME_AS_CURR, file,
                             CONSEQ_SET_GAME_FILE_ABORTED);
        return true;
    }

    if (!SL_Internal_IsValidFile(file)) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_INVALID_PATH, file, CONSEQ_SET_GAME_FILE_ABORTED);
        return false;
    }

    char *sanitizedFile = SL_Internal_SanitizeFile(file);
    if (!sanitizedFile) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_SET_GAME_FILE_ABORTED);
        return false;
    }

    const size_t sanitizedFileLen = strlen(sanitizedFile);
    const size_t gameFileLen = sanitizedFileLen + 1; // '\0'

    free(tracker->gameFile);
    tracker->gameFile = TEST_Malloc(gameFileLen);
    if (!tracker->gameFile) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_SET_GAME_FILE_ABORTED);
        free(sanitizedFile);
        return false;
    }
    snprintf(tracker->gameFile, gameFileLen, "%s", sanitizedFile);

    free(sanitizedFile);

    if (tracker->gameDir) {
        if (!SL_Internal_UpdateGamePath()) {
            SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_SET_GAME_DIR_ABORTED);
            return false;
        }
    }
    SMILE_INFO_WITH_NAME(MODULE, INFO_WITH_ARGS_SET_GAME_FILE_SUCCESSFUL, tracker->gameFile);
    return true;
}

bool SL_FileExists(const char *file) {
    SMILE_ERR(MODULE, CAUSE_NOT_INITIALIZED, CONSEQ_FILE_EXISTS_ABORTED);

    return false;
}

// Save ---------------------------------------------

bool SL_BeginSaveSession(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_BEGIN_SAVE_SESSION_ABORTED);

    if (!tracker->gamePath) {
        SMILE_ERR(MODULE, CAUSE_GAME_PATH_NOT_SET, CONSEQ_BEGIN_SAVE_SESSION_ABORTED);
        return false;
    }

    const char *conseq = CONSEQ_BEGIN_SAVE_SESSION_ABORTED;
    if (SL_Internal_BeginSession(WRITE, tracker->gameFile, conseq)) {
        SMILE_INFO(MODULE, INFO_SAVE_SESSION_STARTED);
        return true;
    }

    return false;
}

bool SL_SaveNext(const char *data) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_SAVE_NEXT_ABORTED);

    if (!tracker->saveStream) {
        SMILE_ERR(MODULE, CAUSE_SAVE_SESSION_NOT_OPEN,
                  CONSEQ_SAVE_NEXT_ABORTED);
        return false;
    }

    if (!data) {
        SMILE_ERR(MODULE, CAUSE_NULL_DATA, CONSEQ_SAVE_NEXT_ABORTED);
        return false;
    }

    size_t bufferSize = strlen(data) + 2; // '\n' + '\0'
    char *buffer = malloc(bufferSize);
    if (!buffer) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED,
                  CONSEQ_SAVE_NEXT_ABORTED);
        return false;
    }

    int bufferLen = snprintf(buffer, bufferSize, "%s\n", data);
    if (bufferLen < 0 || bufferLen >= bufferSize) {
        SMILE_ERR(MODULE, CAUSE_DATA_TRUNCATED_FORMATTING,
                  CONSEQ_SAVE_NEXT_ABORTED);
        free(buffer);
        return false;
    }

    bool success = true;
    if (fwrite(buffer, bufferLen, 1, tracker->saveStream) != 1) {
        SMILE_ERR(MODULE, CAUSE_DATA_TRUNCATED_WRITING,
                  CONSEQ_SAVE_NEXT_ABORTED);
        success = false;
    }

    free(buffer);

    if (success) {
        SMILE_INFO(MODULE, INFO_SAVE_SUCCESSFUL);
    }
    return success;
}

bool SL_EndSaveSession() {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_END_SAVE_SESSION_ABORTED);

    if (!tracker->saveStream) {
        SMILE_ERR(MODULE, CAUSE_SAVE_SESSION_NOT_OPEN, CONSEQ_END_SAVE_SESSION_ABORTED);
        return false;
    }

    if (fclose(tracker->saveStream) == EOF) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_FAILED_TO_CLOSE_FILE, tracker->saveStream,
                            CONSEQ_END_SAVE_SESSION_ABORTED);
        return false;
    }

    tracker->saveStream = nullptr;
    SMILE_INFO(MODULE, INFO_SAVE_SESSION_ENDED);
    return true;
}

// Load ---------------------------------------------

bool SL_BeginLoadSession(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_BEGIN_LOAD_SESSION_ABORTED);

    if (!tracker->gamePath) {
        SMILE_ERR(MODULE, CAUSE_GAME_PATH_NOT_SET, CONSEQ_BEGIN_SAVE_SESSION_ABORTED);
        return false;
    }

    const char *conseq = CONSEQ_BEGIN_LOAD_SESSION_ABORTED;
    if (SL_Internal_BeginSession(LOAD, tracker->gameFile, conseq)) {
        SMILE_INFO(MODULE, INFO_LOAD_SESSION_STARTED);
        return true;
    }

    return false;
}

bool SL_HasNext(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_HAS_NEXT_ABORTED);

    if (!tracker->loadStream) {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_HAS_NEXT_ABORTED);
        return false;
    }

    int c = fgetc(tracker->loadStream);
    if (c == EOF) {
        return false;
    }

    if (ungetc(c, tracker->loadStream) == EOF) {
        SMILE_FATAL(MODULE, CAUSE_INDICATOR_NOT_RESET,
                    CONSEQ_HAS_NEXT_ABORTED);
        return false;
    }

    SMILE_INFO(MODULE, INFO_HAS_NEXT_SUCCESSFUL);
    return true;
}

char *SL_LoadNext(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(CONSEQ_LOAD_NEXT_ABORTED);

    if (!tracker->loadStream) {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    long startPos = ftell(tracker->loadStream);
    size_t counter = 0;
    int cursor;

    // Find out line length and store it in counter
    while ((cursor = fgetc(tracker->loadStream)) != EOF && cursor != '\n') {
        counter++;
    }

    if (counter == 0 && cursor == EOF) {
        SMILE_WARN(MODULE, CAUSE_NO_MORE_DATA,
                   CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    size_t bufferSize = counter + 2; // room for '\n' and '\0'
    char *buffer = malloc(bufferSize);
    if (!buffer) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED,
                  CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    if (fseek(tracker->loadStream, startPos, SEEK_SET) != 0) {
        free(buffer);
        SMILE_ERR(MODULE, CAUSE_INDICATOR_NOT_RESET,
                  CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    if (!fgets(buffer, bufferSize, tracker->loadStream)) {
        free(buffer);
        SMILE_WARN(MODULE, "hi",
                   CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    SMILE_INFO(MODULE, INFO_LOAD_SUCCESSFUL);
    return buffer;
}

bool SL_LoadNextTo(char *dest, size_t size) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_LOAD_NEXT_TO_ABORTED);

    if (!tracker->loadStream) {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_LOAD_NEXT_TO_ABORTED);
        return false;
    }

    if (!dest) {
        SMILE_WARN(MODULE, "hi",
                   CONSEQ_LOAD_NEXT_TO_ABORTED);
        return false;
    }

    if (!fgets(dest, size, tracker->loadStream)) {
        SMILE_WARN(MODULE, "hi",
                   CONSEQ_LOAD_NEXT_TO_ABORTED);
        return false;
    }

    size_t len = strlen(dest);
    if (len > 0 && dest[len - 1] == '\n') {
        dest[len - 1] = '\0';
    }

    SMILE_INFO(MODULE, INFO_LOAD_SUCCESSFUL);
    return true;
}

bool SL_EndLoadSession(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_END_LOAD_SESSION_ABORTED);

    if (!tracker->loadStream) {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_END_LOAD_SESSION_ABORTED);
        return false;
    }

    if (fclose(tracker->loadStream) == EOF) {
        SMILE_ERR(MODULE, "hi",
                  CONSEQ_END_LOAD_SESSION_ABORTED);
        return false;
    }

    tracker->loadStream = nullptr;
    SMILE_INFO(MODULE, INFO_LOAD_SESSION_ENDED);
    return true;
}

// Delete -------------------------------------------

bool SL_DeleteDir(const char *dir) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_DELETE_DIR_ABORTED);

    return false;
}

bool SL_DeleteFile(const char *file) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_DELETE_SAVE_ABORTED);

    return false;
}

// Shutdown -----------------------------------------

bool SL_Shutdown(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_SHUTDOWN_ABORTED);

    bool isFatal = TEST_Fatal();

    if (tracker->saveStream) {
        if (fclose(tracker->saveStream) == EOF) {
            isFatal = true;
        }
        tracker->saveStream = nullptr;
    }

    if (tracker->loadStream) {
        if (fclose(tracker->loadStream) == EOF) {
            isFatal = true;
        }
        tracker->loadStream = nullptr;
    }

    free((void *) tracker->defaultDir);
    tracker->defaultDir = nullptr;

    free(tracker->gameDir);
    tracker->gameDir = nullptr;

    free(tracker->gameFile);
    tracker->gameFile = nullptr;

    free(tracker);
    tracker = nullptr;

    if (isFatal) {
        // TODO get files names
        SMILE_FATAL_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_FAILED_TO_CLOSE_FILE, "name", CONSEQ_SHUTDOWN_ABORTED);
        return false;
    }

    SMILE_INFO(MODULE, INFO_SHUTDOWN_SUCCESSFUL);
    return true;
}

// --------------------------------------------------
// Internal
// --------------------------------------------------

char *SL_Internal_GetDefaultSysDir(void) {
    const char *homeDir;
#if defined(__APPLE__) || defined(__linux)
    homeDir = getenv("HOME");
    if (!homeDir) {
        return nullptr;
    }
#endif
#ifdef TARGET_OS_WIN32
#endif

#if defined(__APPLE__) || defined(__linux)
    const size_t homeDirLen = strlen(homeDir);
    size_t sysDirLen = strlen(DEFAULT_SYS_DIR);
    size_t bufferLen = homeDirLen + sysDirLen + 1;

    char *buffer = TEST_Malloc(bufferLen);
    if (!buffer) {
        return nullptr;
    }
    snprintf(buffer, bufferLen, "%s%s", homeDir, DEFAULT_SYS_DIR);

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

bool SL_Internal_DirExists(const char *absoluteDir) {
    struct stat buf;
    return stat(absoluteDir, &buf) == 0 && S_ISDIR(buf.st_mode);
}

bool SL_Internal_CreateDir(const char *dir) {
#ifdef __APPLE__
    constexpr mode_t mode = 0755;
    return mkdir(dir, mode) == 0;

    // TODO implement a recursive way to create dirs

#endif
#ifdef __linux__

#endif
#ifdef _WIN32

#endif
}

bool SL_Internal_IsValidDir(const char *dir) {
    return true;
}

char *SL_Internal_SanitizeDir(const char *dir) {
    // TODO sanitize and add '/' at the end
    return nullptr;
}

bool SL_Internal_IsValidFile(const char *file) {
    return true;
}

char *SL_Internal_SanitizeFile(const char *file) {
    return nullptr;
}

bool SL_Internal_UpdateGamePath(void) {
    free(tracker->gamePath);
    size_t len = strlen(tracker->defaultDir) + strlen(tracker->gameDir) + strlen(tracker->gameFile) + 1;
    tracker->gamePath = TEST_Malloc(len);
    if (!tracker->gamePath) {
        return false;
    }
    snprintf(tracker->gamePath, len, "%s%s%s", tracker->defaultDir, tracker->gameDir, tracker->gameFile);
    return true;
}

bool SL_Internal_BeginSession(const FileInteractionMode mode, const char *file, const char *conseqAbort) {
    RETURN_FALSE_IF_NOT_INITIALIZED(conseqAbort);

    FILE **currStream = nullptr;
    const char *openAs = nullptr;

    switch (mode) {
        case WRITE:
            if (tracker->saveStream) {
                SMILE_ERR(MODULE, CAUSE_SAVE_SESSION_ALREADY_OPEN, conseqAbort);
                return false;
            }
            currStream = &tracker->saveStream;
            openAs = "w";
            break;

        case LOAD:
            if (tracker->loadStream) {
                SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_ALREADY_OPEN, conseqAbort);
                return false;
            }
            currStream = &tracker->loadStream;
            openAs = "r";
            break;
        case APPEND:
            break;
        default:
            // TODO add error here
            return false;
    }

    char *targetFile;
    bool shouldFree = false;

    if (!file) {
        if (!tracker->gameFile) {
            SMILE_ERR(MODULE, CAUSE_DEST_FILE_NOT_SET, conseqAbort);
            return false;
        }
        targetFile = tracker->gameFile;
    } else {
        size_t totalLen = strlen(tracker->gameDir) + strlen(file) + 1;
        targetFile = malloc(totalLen);
        if (!targetFile) {
            SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, conseqAbort);
            return false;
        }
        size_t written =
                snprintf(targetFile, totalLen, "%s%s", tracker->gameDir, file);
        if (written < 0 || written >= totalLen) {
            SMILE_ERR(MODULE, CAUSE_DATA_TRUNCATED_FORMATTING, conseqAbort);
            free(targetFile);
            return false;
        }
        shouldFree = true;
    }

    *currStream = fopen(targetFile, openAs);
    if (!*currStream) {
        SMILE_ERR(MODULE, CAUSE_WITH_ARGS_FAILED_TO_OPEN_FILE, conseqAbort);
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
