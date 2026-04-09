/**
 * @file
 * @brief Declarations of internal data types and functions for the
 *        GenScene tool.
 *
 * @note TODO #XX [Function] for [CommonInternal] - Create a func to sanitize name input
 *                                                  int cmInternalSanitizeName(char *buf, size_t size, const char *name);
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

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define DEFAULT_SRC_DIR "src"
#define DEFAULT_INCLUDE_DIR "include"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef struct
{
    char *sceneName;
    char *srcPath;
    char *includePath;

    bool noEnter;
    bool noUpdate;
    bool noDraw;
    bool noExit;
} gsInternalArgs;


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool gsInternalYesNoPrompt(const char *prompt);

#endif
