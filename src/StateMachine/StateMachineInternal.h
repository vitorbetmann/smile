// TODO(#9) create internal function to trim leading and trailing whitespace

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
 * @author Vitor Betmann
 */
typedef struct {
  const char *name;
  State *state;
  UT_hash_handle hh;
} StateMap;

/**
 * @brief Internal tracker holding the registered states and the current state.
 * @author Vitor Betmann
 */
struct StateTracker {
  StateMap *stateMap;
  const State *currState;
};

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Sets the current active state without calling lifecycle callbacks.
 *
 * For internal use only. This bypasses any exit/enter logic and just updates
 * the internal state pointer.
 *
 * @param state Pointer to the state to set as current.
 * @return true if the tracker was initialized and the state was set, false
 * otherwise.
 * @author Vitor Betmann
 */
bool SM_Internal_SetCurrState(const State *state);

/**
 * @brief Returns a pointer to the currently active state.
 *
 * For internal use only. Returns NULL if the state machine is not initialized
 * or no state is active.
 *
 * @return const State* Pointer to the current state, or NULL.
 * @author Vitor Betmann
 */
const State *SM_Internal_GetCurrState(void);

/**
 * @brief Looks up a state by its registered name.
 *
 * For internal use only. Performs a hash table lookup and returns a pointer
 * to the internal state if found.
 *
 * @param name Name of the state to find.
 * @return const State* Pointer to the matching state, or NULL if not found or
 * uninitialized.
 * @author Vitor Betmann
 */
const State *SM_Internal_GetState(const char *name);

/**
 * @brief Enables or disables debug-mode warnings at runtime.
 *
 * For internal use only. This function controls whether SMILE logs non-fatal
 * debug warnings (e.g., NULL pointers passed to lifecycle callbacks).
 *
 * This setting only takes effect if the engine was compiled with
 * `-DSMILE_WARNINGS=ON`. If compiled with warnings disabled, this function has
 * no effect.
 *
 * @param toggle If true, warnings are shown at runtime. If false, they are
 * suppressed.
 * @author Vitor Betmann
 */
void SM_Internal_EnableWarnings(bool toggle);

#endif