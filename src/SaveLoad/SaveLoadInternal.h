/**
* @file
 * @brief Internal declarations of data structures and functions for the
 *        SaveLoad module.
 *
 * Defines the private types, constants, and utilities used internally by
 * SaveLoad to manage files, directories, and runtime context.
 *
 * @see SaveLoad.c
 * @see SaveLoad.h
 * @see SaveLoadMessages.h
 *
 * @author Vitor Betmann
 * @date 2026-01-01
 * @version 1.0.0
 */

#ifndef SMILE_SAVE_LOAD_INTERNAL_H
#define SMILE_SAVE_LOAD_INTERNAL_H

// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <stdio.h>


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#define SMILE_DIR "Smile/"

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


// —————————————————————————————————————————————————————————————————————————————
// Data types
// —————————————————————————————————————————————————————————————————————————————

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
} slInternalTracker;

// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

// Game Dir Related

char *slInternalGetDefaultSysDir(void);

bool slInternalDirExists(const char *absoluteDir);

bool slInternalCreateDir(const char *dir);

bool slInternalIsValidDir(const char *dir);

bool slInternalIsValidFile(const char *file);

// Game File Related

char *slInternalSanitizeDir(const char *dir);

char *slInternalSanitizeFile(const char *file);

bool slInternalUpdateGamePath(void);

// Save and Load Related

bool slInternalBeginSession(FileInteractionMode mode, const char *file,
                            const char *conseqAbort);

#endif
