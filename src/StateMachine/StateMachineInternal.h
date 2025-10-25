/**
 * @file StateMachineInternal.h
 * @brief Internal structures and functions for the Smile State Machine module.
 *
 * @author Vitor Betmann
 */

#ifndef SMILE_STATE_MACHINE_INTERNAL_H
#define SMILE_STATE_MACHINE_INTERNAL_H
// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <time.h>
#include <external/uthash.h>

#include "include/StateMachine.h"


// --------------------------------------------------
// Defines
// --------------------------------------------------

#define DEFAULT_FPS 60


// --------------------------------------------------
// Data types
// --------------------------------------------------

/**
 * @brief Represents an individual state within the state machine.
 *
 * Each state includes optional lifecycle functions for handling entry,
 * update, drawing, and exit logic.
 */
typedef struct
{
  char *name;
  smEnterFn enter;
  smUpdateFn update;
  smDrawFn draw;
  smExitFn exit;
} InternalState;

/**
 * @brief Maps a state name to its corresponding State structure.
 *
 * Used internally by the state machine to efficiently look up states
 * by name using uthash.
 */
typedef struct
{
  char *name;
  InternalState *state;
  UT_hash_handle hh;
} InternalStateMap;

/**
 * @brief Tracks the current state machine context.
 *
 * Contains all runtime information such as registered states, the current
 * active state, frame rate settings, and timing data used for delta time
 * calculations.
 */
typedef struct
{
  InternalStateMap *stateMap;
  const InternalState *currState;
  int stateCount;
  int fps;
#if defined(_WIN32)
  // TODO add Windows support
#else
  struct timespec lastTime;
#endif
} InternalTracker;


// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Retrieves a pointer to a State by name.
 *
 * @param name The name of the state to look up.
 * @return Pointer to the matching State, or NULL if not found.
 */
const InternalState *smInternalGetState(const char *name);

/**
 * @brief Retrieves a pointer to a StateMap entry by name.
 *
 * @param name The name of the state entry to look up.
 * @return Pointer to the matching StateMap entry, or NULL if not found.
 */
InternalStateMap *smInternalGetEntry(const char *name);

#endif // #ifndef SMILE_STATE_MACHINE_INTERNAL_H
