#ifndef SMILE_SAVE_LOAD_INTERNAL_H
#define SMILE_SAVE_LOAD_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <stdio.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

#define SMILE_DIR "SmileEngine/"

#ifdef __APPLE__
#define DEFAULT_SYS_DIR "/Library/Application Support/"
#define ALT_SYS_DIR "/Documents/"
#endif

#ifdef __linux__
#define DEFAULT_SYS_DIR
#define ALT_SYS_DIR
#endif

#ifdef _WIN32
#define DEFAULT_SYS_DIR
#define ALT_SYS_DIR
#endif

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef enum {
    WRITE,
    APPEND,
    LOAD,
} FileInteractionMode;

typedef struct {
    char *defaultDir;
    char *gameDir;
    char *gameFile;
    char *gamePath;
    FILE *saveStream;
    FILE *loadStream;
} SaveLoadTracker;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

// Game dir -----------------------------------------

char *SL_Internal_GetDefaultSysDir(void);

bool SL_Internal_DirExists(const char *absoluteDir);

bool SL_Internal_CreateDir(const char *dir);

bool SL_Internal_IsValidDir(const char *dir);

bool SL_Internal_IsValidFile(const char *file);

// Game file -----------------------------------------

char *SL_Internal_SanitizeDir(const char *dir);

char *SL_Internal_SanitizeFile(const char *file);

bool SL_Internal_UpdateGamePath(void);

// Save Load ------------------------------------------

bool SL_Internal_BeginSession(FileInteractionMode mode, const char *file, const char *conseqAbort);

#endif
