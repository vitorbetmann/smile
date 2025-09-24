#ifndef STATE_MACHINE_INTERNAL_H
#define STATE_MACHINE_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "../../external/uthash.h"
#include "StateMachine.h"

// --------------------------------------------------
// Data types
// --------------------------------------------------

/**
 * @brief Internal representation of a state.
 *
 * Each state can optionally define enter, update, draw, and exit callbacks.
 *
 * @author Vitor Betmann
 */
struct State {
  const char *name;

  void (*enter)(void *args);

  void (*update)(float dt);

  void (*draw)();

  void (*exit)();
};

/**
 * @brief Internal hash entry mapping a state's name to its struct.
 *
 * @author Vitor Betmann
 */
typedef struct {
  const char *name;
  State *state;
  UT_hash_handle hh;
} StateMap;

/**
 * @brief Internal tracker holding the registered states and the current state.
 *
 * @author Vitor Betmann
 */
struct StateMachineTracker {
  StateMap *stateMap;
  const State *currState;
  int stateCount;
};

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Looks up a state by its registered name.
 *
 * For internal use only. Performs a hash table lookup and returns a pointer
 * to the internal state if found.
 *
 * @param name Name of the state to find.
 *
 * @return const State* Pointer to the matching state, or NULL if not found or
 * uninitialized.
 *
 * @author Vitor Betmann
 */
const State *SM_Internal_GetState(const char *name);

#endif
