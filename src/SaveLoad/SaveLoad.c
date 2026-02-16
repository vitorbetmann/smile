/**
 * @file
 * @brief Implementation of the SaveLoad module.
 *
 * @see SaveLoad.h
 * @see SaveLoadInternal.h
 * @see SaveLoadMessages.h
 *
 * @bug No known bugs.
 *
 * @author Vitor Betmann
 * @date 2026-01-01
 * @version 1.0.0
 */


// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "SaveLoad.h"
#include "SaveLoadInternal.h"
#include "SaveLoadMessages.h"

#include "CommonInternal.h"
#include "CommonInternalMessages.h"
#include "LogInternal.h"
#include "TestInternal.h"


// —————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————

static slInternalTracker *tracker;


// —————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————

// Start Related

// bool slStartWith(const char *dir, const char *file)
// {
//     if (!(slStart() && slSetGameDir(dir) && slSetGameFile(file)))
//     {
//         return false;
//     }
//
//     return true;
// }

bool slStart(void)
{
    if (tracker)
    {
        lgInternalLog(WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(slInternalTracker));
    if (!tracker)
    {
        lgInternalLog(WARNING, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    char *sysDir = slInternalGetDefaultSysDir();
    if (!sysDir)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        goto error;
    }

    tracker->defaultDir = tsInternalMalloc(
        strlen(sysDir) + strlen(SMILE_DIR) + 1);
    if (!tracker->defaultDir)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        goto error;
    }

    snprintf(tracker->defaultDir, strlen(sysDir) + strlen(SMILE_DIR) + 1,
             "%s%s", sysDir, SMILE_DIR);

    if (!slInternalDirExists(tracker->defaultDir))
    {
        if (!slInternalCreateDir(tracker->defaultDir))
        {
            lgInternalLog(FATAL, MODULE, CAUSE_FAILED_TO_CREATE_DIR, FN_START,
                          CONSEQ_ABORTED);
            goto error;
        }
    }

    free(sysDir);
    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return true;

error:
    free(sysDir);

    free(tracker->defaultDir);
    tracker->defaultDir = nullptr;

    free(tracker);
    tracker = nullptr;

    return false;
}

bool slIsInitialized(void)
{
    return tracker;
}

// Dir Functions ------------------------------------

const char *slGetGameDir(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, FN_GET_GAME_DIR))
    {
        return nullptr;
    }

    return tracker->gameDir;
}

const char *slGetDefaultDir(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, GET_DEFAULT_DIR))
    {
        return nullptr;
    }

    return tracker->defaultDir;
}

bool slSetGameDir(const char *dir)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, FN_SET_GAME_DIR))
    {
        return false;
    }

    if (!dir)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_ARG, FN_SET_GAME_DIR,
                      CONSEQ_ABORTED);
        return false;
    }

    if (tracker->gameDir && strcmp(dir, tracker->gameDir) == 0)
    {
        SMILE_WARN_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_TARGET_DIR_IS_SAME_AS_CURR,
                             dir, FN_SET_GAME_DIR, ABORTED);
        return true;
    }

    if (!slInternal_IsValidDir(dir))
    {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_INVALID_PATH, dir,
                            FN_SET_GAME_DIR, ABORTED);
        return false;
    }

    char *sanitizedDir = slInternal_SanitizeDir(dir);
    if (!sanitizedDir)
    {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, FN_SET_GAME_DIR, ABORTED);
        return false;
    }

    const size_t gameDirLen = strlen(tracker->defaultDir) + strlen(sanitizedDir)
                              + 1; // '\0'

    free(tracker->gameDir);
    tracker->gameDir = TEST_Malloc(gameDirLen);
    if (!tracker->gameDir)
    {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, FN_SET_GAME_DIR, ABORTED);
        free(sanitizedDir);
        return false;
    }

    snprintf(tracker->gameDir, gameDirLen, "%s%s", tracker->defaultDir,
             sanitizedDir);
    free(sanitizedDir);


    if (!slInternal_DirExists(tracker->gameDir))
    {
        if (!slInternal_CreateDir(tracker->gameDir))
        {
            SMILE_FATAL(MODULE, CAUSE_FAILED_TO_CREATE_DIR, FN_SET_GAME_DIR,
                        ABORTED);
            return false;
        }
    }

    if (tracker->gameFile)
    {
        if (!slInternal_UpdateGamePath())
        {
            SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, FN_SET_GAME_DIR, ABORTED);
            return false;
        }
    }

    SMILE_INFO_WITH_NAME(MODULE, INFO_WITH_ARGS_SET_GAME_DIR_SUCCESSFUL,
                         tracker->gameDir);
    return true;
}

bool slDirExists(const char *dir)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, DIR_EXISTS))
    {
        return false;
    }

    if (!dir)
    {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_NULL_ARG, "dir", DIR_EXISTS,
                            ABORTED);
        return false;
    }

    return true;
}

// File Functions -----------------------------------

const char *slGetGameFile(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, GET_GAME_FILE))
    {
        return nullptr;
    }

    return tracker->gameFile;
}

const char *slGetGamePath(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, GET_GAME_PATH))
    {
        return nullptr;
    }

    return tracker->gamePath;
}

bool slSetGameFile(const char *file)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, SET_GAME_FILE))
    {
        return false;
    }

    if (!file)
    {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_NULL_ARG, "file",
                            SET_GAME_FILE, ABORTED);
        return false;
    }

    if (tracker->gameFile && strcmp(file, tracker->gameFile) == 0)
    {
        SMILE_WARN_WITH_ARGS(
            MODULE, CAUSE_WITH_ARGS_TARGET_FILE_IS_SAME_AS_CURR, file,
            SET_GAME_FILE, ABORTED);
        return true;
    }

    if (!slInternal_IsValidFile(file))
    {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_INVALID_PATH, file,
                            SET_GAME_FILE, ABORTED);
        return false;
    }

    char *sanitizedFile = slInternal_SanitizeFile(file);
    if (!sanitizedFile)
    {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, SET_GAME_FILE, ABORTED);
        return false;
    }

    const size_t sanitizedFileLen = strlen(sanitizedFile);
    const size_t gameFileLen = sanitizedFileLen + 1; // '\0'

    free(tracker->gameFile);
    tracker->gameFile = TEST_Malloc(gameFileLen);
    if (!tracker->gameFile)
    {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, SET_GAME_FILE, ABORTED);
        free(sanitizedFile);
        return false;
    }
    snprintf(tracker->gameFile, gameFileLen, "%s", sanitizedFile);

    free(sanitizedFile);

    if (tracker->gameDir)
    {
        if (!slInternal_UpdateGamePath())
        {
            SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, SET_GAME_FILE, ABORTED);
            return false;
        }
    }
    SMILE_INFO_WITH_NAME(MODULE, INFO_WITH_ARGS_SET_GAME_FILE_SUCCESSFUL,
                         tracker->gameFile);
    return true;
}

bool slFileExists(const char *file)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, FILE_EXISTS))
    {
        return false;
    }

    return false;
}

// List Functions -----------------------------------

// List slGetListOf(ListType type) {
// }
//
// bool slFreeList(List list) {
// }

// Save Functions -----------------------------------

bool slBeginSaveSession(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, BEGIN_SAVE_SESSION))
    {
        return false;
    }

    if (!tracker->gamePath)
    {
        SMILE_ERR(MODULE, CAUSE_GAME_PATH_NOT_SET,
                  BEGIN_SAVE_SESSION);
        return false;
    }

    const char *conseq = ABORTED;
    if (slInternal_BeginSession(WRITE, tracker->gameFile, conseq))
    {
        SMILE_INFO(MODULE, INFO_SAVE_SESSION_STARTED);
        return true;
    }

    return false;
}

bool slSaveNext(const char *data)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, SAVE_NEXT))
    {
        return false;
    }

    if (!tracker->saveStream)
    {
        SMILE_ERR(MODULE, CAUSE_SAVE_SESSION_NOT_OPEN,SAVE_NEXT);
        return false;
    }

    if (!data)
    {
        SMILE_ERR(MODULE, CAUSE_NULL_DATA, SAVE_NEXT);
        return false;
    }

    size_t bufferSize = strlen(data) + 2; // '\n' + '\0'
    char *buffer = malloc(bufferSize);
    if (!buffer)
    {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, SAVE_NEXT);
        return false;
    }

    int bufferLen = snprintf(buffer, bufferSize, "%s\n", data);
    if (bufferLen < 0 || bufferLen >= bufferSize)
    {
        SMILE_ERR(MODULE, CAUSE_DATA_TRUNCATED_FORMATTING, SAVE_NEXT);
        free(buffer);
        return false;
    }

    bool success = true;
    if (fwrite(buffer, bufferLen, 1, tracker->saveStream) != 1)
    {
        SMILE_ERR(MODULE, CAUSE_DATA_TRUNCATED_WRITING, SAVE_NEXT);
        success = false;
    }

    free(buffer);

    if (success)
    {
        SMILE_INFO(MODULE, INFO_SAVE_SUCCESSFUL);
    }
    return success;
}

bool slEndSaveSession()
{
    if (!cmInternalIsRunning(slIsRunning, MODULE,END_SAVE_SESSION))
    {
        return false;
    }

    if (!tracker->saveStream)
    {
        SMILE_ERR(MODULE, CAUSE_SAVE_SESSION_NOT_OPEN,
                  CONSEQ_END_SAVE_SESSION_ABORTED);
        return false;
    }

    if (fclose(tracker->saveStream) == EOF)
    {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_FAILED_TO_CLOSE_FILE,
                            tracker->saveStream,
                            CONSEQ_END_SAVE_SESSION_ABORTED);
        return false;
    }

    tracker->saveStream = nullptr;
    SMILE_INFO(MODULE, INFO_SAVE_SESSION_ENDED);
    return true;
}

// Load ---------------------------------------------

bool slBeginLoadSession(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE,BEGIN_LOAD_SESSION))
    {
        return false;
    }

    if (!tracker->gamePath)
    {
        SMILE_ERR(MODULE, CAUSE_GAME_PATH_NOT_SET,
                  CONSEQ_BEGIN_SAVE_SESSION_ABORTED);
        return false;
    }

    const char *conseq = CONSEQ_BEGIN_LOAD_SESSION_ABORTED;
    if (slInternal_BeginSession(LOAD, tracker->gameFile, conseq))
    {
        SMILE_INFO(MODULE, INFO_LOAD_SESSION_STARTED);
        return true;
    }

    return false;
}

bool slHasNext(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, HAS_NEXT))
    {
        return false;
    }

    if (!tracker->loadStream)
    {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_HAS_NEXT_ABORTED);
        return false;
    }

    int c = fgetc(tracker->loadStream);
    if (c == EOF)
    {
        return false;
    }

    if (ungetc(c, tracker->loadStream) == EOF)
    {
        SMILE_FATAL(MODULE, CAUSE_INDICATOR_NOT_RESET,
                    CONSEQ_HAS_NEXT_ABORTED);
        return false;
    }

    SMILE_INFO(MODULE, INFO_HAS_NEXT_SUCCESSFUL);
    return true;
}

char *slLoadNext(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, LOAD_NEXT))
    {
        return nullptr;
    }

    if (!tracker->loadStream)
    {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    long startPos = ftell(tracker->loadStream);
    size_t counter = 0;
    int cursor;

    // Find out line length and store it in counter
    while ((cursor = fgetc(tracker->loadStream)) != EOF && cursor != '\n')
    {
        counter++;
    }

    if (counter == 0 && cursor == EOF)
    {
        SMILE_WARN(MODULE, CAUSE_NO_MORE_DATA,
                   CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    size_t bufferSize = counter + 2; // room for '\n' and '\0'
    char *buffer = malloc(bufferSize);
    if (!buffer)
    {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED,
                  CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    if (fseek(tracker->loadStream, startPos, SEEK_SET) != 0)
    {
        free(buffer);
        SMILE_ERR(MODULE, CAUSE_INDICATOR_NOT_RESET,
                  CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    if (!fgets(buffer, bufferSize, tracker->loadStream))
    {
        free(buffer);
        SMILE_WARN(MODULE, "hi",
                   CONSEQ_LOAD_NEXT_ABORTED);
        return nullptr;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }

    SMILE_INFO(MODULE, INFO_LOAD_SUCCESSFUL);
    return buffer;
}

bool slLoadNextTo(char *dest, size_t size)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, LOAD_NEXT_TO))
    {
        return false;
    }

    if (!tracker->loadStream)
    {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_LOAD_NEXT_TO_ABORTED);
        return false;
    }

    if (!dest)
    {
        SMILE_WARN(MODULE, "hi",
                   CONSEQ_LOAD_NEXT_TO_ABORTED);
        return false;
    }

    if (!fgets(dest, size, tracker->loadStream))
    {
        SMILE_WARN(MODULE, "hi",
                   CONSEQ_LOAD_NEXT_TO_ABORTED);
        return false;
    }

    size_t len = strlen(dest);
    if (len > 0 && dest[len - 1] == '\n')
    {
        dest[len - 1] = '\0';
    }

    SMILE_INFO(MODULE, INFO_LOAD_SUCCESSFUL);
    return true;
}

bool slEndLoadSession(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE,END_LOAD_SESSION))
    {
        return false;
    }

    if (!tracker->loadStream)
    {
        SMILE_ERR(MODULE, CAUSE_LOAD_SESSION_NOT_OPEN,
                  CONSEQ_END_LOAD_SESSION_ABORTED);
        return false;
    }

    if (fclose(tracker->loadStream) == EOF)
    {
        SMILE_ERR(MODULE, "hi",
                  CONSEQ_END_LOAD_SESSION_ABORTED);
        return false;
    }

    tracker->loadStream = nullptr;
    SMILE_INFO(MODULE, INFO_LOAD_SESSION_ENDED);
    return true;
}

// Delete -------------------------------------------

bool slDeleteDir(const char *dir)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, FN_DELETE_DIR))
    {
        return false;
    }

    return false;
}

bool slDeleteFile(const char *file)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, FN_DELETE_FILE))
    {
        return false;
    }

    return false;
}

// Shutdown -----------------------------------------

bool slStop(void)
{
    if (!cmInternalIsRunning(slIsRunning, MODULE, FN_STOP))
    {
        return false;
    }

    bool isFatal = TEST_Fatal();

    if (tracker->saveStream)
    {
        if (fclose(tracker->saveStream) == EOF)
        {
            isFatal = true;
        }
        tracker->saveStream = nullptr;
    }

    if (tracker->loadStream)
    {
        if (fclose(tracker->loadStream) == EOF)
        {
            isFatal = true;
        }
        tracker->loadStream = nullptr;
    }

    free(tracker->defaultDir);
    tracker->defaultDir = nullptr;

    free(tracker->gameDir);
    tracker->gameDir = nullptr;

    free(tracker->gameFile);
    tracker->gameFile = nullptr;

    free(tracker);
    tracker = nullptr;

    if (isFatal)
    {
        // TODO get files names
        SMILE_FATAL_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_FAILED_TO_CLOSE_FILE,
                              "name", CONSEQ_SHUTDOWN_ABORTED);
        return false;
    }

    SMILE_INFO(MODULE, INFO_SHUTDOWN_SUCCESSFUL);
    return true;
}

// --------------------------------------------------
// TestInternal
// --------------------------------------------------

char *slInternal_GetDefaultSysDir(void)
{
    const char *homeDir;
#if defined(__APPLE__) || defined(__linux)
    homeDir = getenv("HOME");
    if (!homeDir)
    {
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
    if (!buffer)
    {
        return nullptr;
    }
    snprintf(buffer, bufferLen, "%s%s", homeDir, DEFAULT_SYS_DIR);

    if (!slInternal_DirExists(buffer))
    {
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

bool slInternal_DirExists(const char *absoluteDir)
{
    struct stat buf;
    return stat(absoluteDir, &buf) == 0 && S_ISDIR(buf.st_mode);
}

bool slInternal_CreateDir(const char *dir)
{
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

bool slInternalIsValidDir(const char *dir)
{
    return true;
}

char *slInternalSanitizeDir(const char *dir)
{
    // TODO sanitize and add '/' at the end
    return nullptr;
}

bool slInternalIsValidFile(const char *file)
{
    return true;
}

char *slInternalSanitizeFile(const char *file)
{
    return nullptr;
}

bool slInternalUpdateGamePath(void)
{
    free(tracker->gamePath);
    size_t len = strlen(tracker->defaultDir) + strlen(tracker->gameDir) +
                 strlen(tracker->gameFile) + 1;
    tracker->gamePath = TEST_Malloc(len);
    if (!tracker->gamePath)
    {
        return false;
    }
    snprintf(tracker->gamePath, len, "%s%s%s", tracker->defaultDir,
             tracker->gameDir, tracker->gameFile);
    return true;
}

bool slInternalBeginSession(const FileInteractionMode mode, const char *file,
                            const char *conseqAbort)
{
    FILE **currStream = nullptr;
    const char *openAs = nullptr;

    switch (mode)
    {
        case WRITE:
            if (tracker->saveStream)
            {
                SMILE_ERR(MODULE, CAUSE_SAVE_SESSION_ALREADY_OPEN, conseqAbort);
                return false;
            }
            currStream = &tracker->saveStream;
            openAs = "w";
            break;

        case LOAD:
            if (tracker->loadStream)
            {
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

    if (!file)
    {
        if (!tracker->gameFile)
        {
            SMILE_ERR(MODULE, CAUSE_DEST_FILE_NOT_SET, conseqAbort);
            return false;
        }
        targetFile = tracker->gameFile;
    } else
    {
        size_t totalLen = strlen(tracker->gameDir) + strlen(file) + 1;
        targetFile = malloc(totalLen);
        if (!targetFile)
        {
            SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, conseqAbort);
            return false;
        }
        size_t written =
                snprintf(targetFile, totalLen, "%s%s", tracker->gameDir, file);
        if (written < 0 || written >= totalLen)
        {
            SMILE_ERR(MODULE, CAUSE_DATA_TRUNCATED_FORMATTING, conseqAbort);
            free(targetFile);
            return false;
        }
        shouldFree = true;
    }

    *currStream = fopen(targetFile, openAs);
    if (!*currStream)
    {
        SMILE_ERR(MODULE, CAUSE_WITH_ARGS_FAILED_TO_OPEN_FILE, conseqAbort);
        if (shouldFree)
        {
            free(targetFile);
        }
        return false;
    }

    if (shouldFree)
    {
        free(targetFile);
    }

    return true;
}
