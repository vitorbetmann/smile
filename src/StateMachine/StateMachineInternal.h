#ifndef STATE_MACHINE_INTERNAL_H
#define STATE_MACHINE_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../../external/uthash.h"
#include "StateMachine.h"

// --------------------------------------------------
// Other defines
// --------------------------------------------------
#define STATE_MACHINE_NAME "StateMachine"

// Module specific messages
#define LOG_MSG_DRAW_FUNCTION_NULL "Draw function is NULL."
#define LOG_MSG_UPDATE_FUNCTION_NULL "Update function is NULL."
#define LOG_MSG_CURRENT_STATE_NULL "Current state is NULL."

// Causes
#define LOG_CAUSE_ALREADY_EXISTS "State already exists"
#define LOG_CAUSE_NO_VALID_FUNCTIONS "State has no valid functions"
#define LOG_CAUSE_STATE_NOT_FOUND "State not found"

// Effects (Consequence)
#define LOG_EFFECT_REGISTER_STATE_ABORTED "RegisterState aborted"
#define LOG_EFFECT_CHANGE_STATE_ABORTED "ChangeStateTo aborted"
#define LOG_EFFECT_DRAW_ABORTED "Draw aborted"
#define LOG_EFFECT_UPDATE_ABORTED "Update aborted"

// Requires something after
#define LOG_MSG_STATE_NOT_FOUND "State not found:"
#define LOG_MSG_NO_VALID_FUNCTIONS "State has no valid functions:"
#define LOG_MSG_STATE_ALREADY_EXISTS "State already exists:"
#define LOG_MSG_STATE_CREATION_SUCCESSFUL "Successful state creation:"
#define LOG_MSG_STATE_CHANGE_SUCCESSFUL "Successful state change to:"

// Suffixes - Public
#define LOG_SUFFIX_DRAW_ABORTED "Draw aborted."
#define LOG_SUFFIX_UPDATE_ABORTED "Update aborted."
#define LOG_SUFFIX_REGISTER_STATE_ABORTED "RegisterState aborted."
#define LOG_SUFFIX_GET_CURR_STATE_NAME_ABORTED "GetCurrStateName aborted."
#define LOG_SUFFIX_CHANGE_STATE_TO_ABORTED "ChangeStateTo aborted."
#define LOG_SUFFIX_IS_STATE_REGISTERED_ABORTED "IsStateRegistered aborted."

// Suffixes - Internal
#define LOG_SUFFIX_INTERNAL_GET_TRACKER_ABORTED "Internal_GetTracker aborted."
#define LOG_SUFFIX_INTERNAL_SET_CURR_STATE_ABORTED                             \
  "Internal_SetCurrState aborted."
#define LOG_SUFFIX_INTERNAL_GET_CURR_STATE_ABORTED                             \
  "Internal_GetCurrState aborted."
#define LOG_SUFFIX_INTERNAL_GET_STATE_ABORTED "Internal_GetState aborted."

// Suffixes - Tests
#define LOG_SUFFIX_TEST_GET_STATE_COUNT_ABORTED                                \
  "Internal_GetStateCount aborted."

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