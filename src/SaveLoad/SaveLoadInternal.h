#ifndef SAVE_LOAD_SYSTEM_INTERNAL_H
#define SAVE_LOAD_SYSTEM_INTERNAL_H
// --------------------------------------------------
// Includes
// --------------------------------------------------
#include <stdio.h>

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
char *SL_Internal_GetDefaultSysDir(void);
bool SL_Internal_CreateDir(const char *dir);
bool SL_Internal_BeginSession(FileInteractionMode mode, const char *file,
                              const char *conseqAbort);

#endif