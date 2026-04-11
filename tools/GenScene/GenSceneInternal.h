/**
 * @file
 * @brief Declarations of internal data types and functions for the
 *        GenScene tool.
 *
 * @see GenScene.c
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_GEN_SCENE_INTERNAL_H
#define SMILE_GEN_SCENE_INTERNAL_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define DEFAULT_SRC_DIR     "src"
#define DEFAULT_INCLUDE_DIR "include"
#define GS_NAME_MAX         64
#define GS_SECTION_DIV      "// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Result codes for the GenScene tool.
 *
 * @note GenScene-specific failures cover the following range: `-100..-199`.
 *
 * @see  src/_Internal/_Common/CommonInternal.h for common result codes
 *
 * @author Vitor Betmann
 */
typedef enum
{
    GS_RESULT_INVALID_FLAG = -100,
    GS_RESULT_NO_CALLBACKS = -101,
    GS_RESULT_USER_ABORTED = -102,
} gsInternalResult;

typedef struct
{
    char *sceneName;
    char *srcPath;
    char *includePath;

    bool addSection;
    bool noEnter;
    bool noUpdate;
    bool noDraw;
    bool noExit;
} gsInternalArgs;

typedef bool (*gsPromptFn)(const char *prompt);


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int gsInternalSanitizeName(char *buf, size_t bufSize, const char *name);

void gsInternalFatalHandler(void);

int gsInternalRun(int argc, char *argv[]);

#endif
