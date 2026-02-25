/**
 * @file
 * @brief Declarations of internal data types and functions for the
 *        SceneManager module.
 *
 * @see SceneManager.c
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_SCENE_MANAGER_INTERNAL_H
#define SMILE_SCENE_MANAGER_INTERNAL_H


// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <time.h>
#include <uthash.h>

#include "SceneManager.h"


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#define DEFAULT_FPS 60


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Represents an individual scene within SceneManager.
 *
 * Each scene includes optional lifecycle functions for handling entry,
 * update, drawing, and exit logic.
 *
 * @author Vitor Betmann
 */
typedef struct {
    char *name;
    smEnterFn enter;
    smUpdateFn update;
    smDrawFn draw;
    smExitFn exit;
} smInternalScene;

/**
 * @brief Maps a scene name to its corresponding scene structure.
 *
 * Used internally by SceneManager to efficiently look up scenes
 * by name using uthash.
 *
 * @author Vitor Betmann
 */
typedef struct {
    char *name;
    smInternalScene *state;
    UT_hash_handle hh;
} smInternalSceneMap;

/**
 * @brief Tracks the current SceneManager context.
 *
 * Contains all runtime information such as registered scenes, the current
 * active scene, frame rate settings, and timing data used for delta time
 * calculations.
 *
 * @author Vitor Betmann
 */
typedef struct {
    smInternalSceneMap *sceneMap;
    const smInternalScene *currScene;
    int sceneCount;
    int fps;
    struct timespec lastTime;
} smInternalTracker;


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Retrieves a pointer to a scene by name.
 *
 * @param name The name of the scene to look up.
 *
 * @return Pointer to the matching scene, or NULL if not found.
 *
 * @author Vitor Betmann
 */
const smInternalScene *smInternalGetScene(const char *name);

/**
 * @brief Retrieves a pointer to a scene-map entry by name.
 *
 * @param name The name of the scene entry to look up.
 *
 * @return Pointer to the matching scene-map entry, or NULL if not found.
 *
 * @author Vitor Betmann
 */
smInternalSceneMap *smInternalGetEntry(const char *name);


#endif // #ifndef SMILE_SCENE_MANAGER_INTERNAL_H
