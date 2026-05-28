#pragma once

// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <time.h>
#include <uthash.h>

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief SceneManager-exclusive result codes; range -100..-199.
 */
typedef enum
{
    RES_SCENE_ALREADY_EXISTS = -100, /**< A scene with the same name already exists.                  */
    RES_SCENE_NOT_FOUND      = -101, /**< Requested scene was not found.                              */
    RES_NO_VALID_FUNCS       = -102, /**< Scene creation received no valid lifecycle callbacks.        */
    RES_CANT_DEL_CURR_SCENE  = -103, /**< Attempted to delete the currently active scene.             */
    RES_NO_CURR_SCENE        = -104, /**< Operation requires an active scene, but none is set.        */
    RES_NO_UPDATE_FUNC       = -105, /**< Active scene has no update callback.                        */
    RES_NO_DRAW_FUNC         = -106, /**< Active scene has no draw callback.                          */
    RES_FREE_ALL_SCENES_FAIL = -107, /**< Internal cleanup invariant failed while freeing all scenes. */
} smInternalResult;

/**
 * @brief Represents a scene and its lifecycle callbacks.
 */
typedef struct
{
    char      *name;   /**< Scene name (owned by SceneManager).       */
    smEnterFn  enter;  /**< Optional callback executed when entering. */
    smUpdateFn update; /**< Optional callback executed during update. */
    smDrawFn   draw;   /**< Optional callback executed during draw.   */
    smExitFn   exit;   /**< Optional callback executed when exiting.  */
} smInternalScene;

/**
 * @brief Hash map entry that associates a scene name with its internal scene.
 */
typedef struct
{
    char            *name;  /**< Hash key / scene name.                         */
    smInternalScene *scene; /**< Pointer to the scene.                          */
    UT_hash_handle   hh;    /**< uthash handle for map operations.              */
} smInternalSceneMap;

/**
 * @brief Tracks current SceneManager runtime state.
 */
typedef struct
{
    smInternalSceneMap    *sceneMap;   /**< Hash map of registered scenes.                       */
    const smInternalScene *currScene;  /**< Current active scene (or nullptr).                   */
    int                    sceneCount; /**< Number of currently registered scenes.               */
    int                    fps;        /**< Target FPS (used by delta-time first-call fallback). */
    struct timespec        lastTime;   /**< Last timestamp used by delta-time computation.       */
} smInternalTracker;

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Retrieves a scene pointer by name.
 *
 * @param name Name of the scene to look up.
 *
 * @return Pointer to the matching scene, or nullptr if not found.
 */
const smInternalScene *smInternalGetScene(const char *name);

/**
 * @brief Retrieves a scene-map entry pointer by name.
 *
 * @param name Name of the scene-map entry to look up.
 *
 * @return Pointer to the matching map entry, or nullptr if not found.
 */
smInternalSceneMap *smInternalGetEntry(const char *name);

// Variables ———————————————————————————————————————————————————————————————————————————————————————

/** @brief Target FPS used by smGetDt() as the first-call dt fallback. */
static const int DEFAULT_FPS = 60;