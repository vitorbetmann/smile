#ifndef SAVE_LOAD_SYSTEM_INTERNAL_H
#define SAVE_LOAD_SYSTEM_INTERNAL_H
// --------------------------------------------------
// Includes
// --------------------------------------------------
#include <stdio.h>

// --------------------------------------------------
// Other defines
// --------------------------------------------------
#define SAVE_LOAD_SYSTEM_NAME "SaveLoadSystem"

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
} SaveLoadSystemTracker;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
char *SLS_Internal_GetDefaultOSDir(void);
char *SLS_Internal_GetGameName(void);
void SLS_Internal_SetFileFormat(const char *file);
bool SLS_Internal_EndsWith(const char *str, const char *suffix);
bool SLS_Internal_BeginSession(FileInteractionMode mode, const char *file,
                               const char *conseqAbort);

#endif