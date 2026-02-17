/**
 * @file
 * @brief Internal declarations of data structures and functions for the
 *        SceneManager module.
 *
 * Defines the private types, constants, and utilities used internally by
 * SceneManager to manage states, timing, and runtime context.
 *
 * @see SceneManager.c
 * @see SceneManager.h
 * @see SceneManagerMessages.h
 *
 * @author Vitor Betmann
 * @date 2025-11-04
 * @version 1.0.0
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
 * @brief Represents an individual state within the state machine.
 *
 * Each state includes optional lifecycle functions for handling entry,
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
 * @brief Maps a state name to its corresponding State structure.
 *
 * Used internally by the state machine to efficiently look up states
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
 * @brief Tracks the current state machine context.
 *
 * Contains all runtime information such as registered states, the current
 * active state, frame rate settings, and timing data used for delta time
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
 * @brief Retrieves a pointer to a State by name.
 *
 * @param name The name of the state to look up.
 *
 * @return Pointer to the matching State, or NULL if not found.
 *
 * @author Vitor Betmann
 */
const smInternalScene *smInternalGetScene(const char *name);

/**
 * @brief Retrieves a pointer to a StateMap entry by name.
 *
 * @param name The name of the state entry to look up.
 *
 * @return Pointer to the matching StateMap entry, or NULL if not found.
 *
 * @author Vitor Betmann
 */
smInternalSceneMap *smInternalGetEntry(const char *name);


#endif // #ifndef SMILE_SCENE_MANAGER_INTERNAL_H
