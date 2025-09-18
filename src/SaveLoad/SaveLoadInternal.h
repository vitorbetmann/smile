#ifndef SAVE_LOAD_SYSTEM_INTERNAL_H
#define SAVE_LOAD_SYSTEM_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <stdio.h>

// --------------------------------------------------
// Other defines
// --------------------------------------------------

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
  SAVE,
  LOAD,
} FileInteractionMode;

typedef struct {
  char *dirPath;
  char *filePath;
  FILE *saveStream;
  FILE *loadStream;
} SaveLoadTracker;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
const char *SL_Internal_GetDefaultSysDir(void);
bool SL_Internal_CreateDir(const char *dir);
bool SL_Internal_BeginSession(FileInteractionMode mode, const char *file,
                              const char *conseqAbort);

bool SL_Internal_DirExists(const char *absoluteDir);
#endif