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

bool gsInternalYesNoPrompt(const char *prompt);

void gsInternalFatalHandler(void);

void gsInternalWriteHeader(FILE *f, const gsInternalArgs *args);

void gsInternalWriteSrc(FILE *f, const gsInternalArgs *args);

int gsInternalRun(int argc, char *argv[]);

void gsSetPrompt(gsPromptFn fn);


#endif
