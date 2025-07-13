#ifndef STATE_MACHINE_INTERNAL_H
#define STATE_MACHINE_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateMachine.h"

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * Set the current active state.
 *
 * This function is for internal use only.
 * It replaces the current state pointer without calling enter/exit functions.
 * Returns early if the state machine is not initialized.
 */
void SM_SetCurrState(const State *state);

/**
 * Get a pointer to the current active state.
 *
 * This function is for internal use only.
 * Returns NULL if the state machine is not initialized.
 */
const State *SM_GetCurrState(void);

/**
 * Look up a state by its registered name.
 *
 * This function is for internal use only.
 * Returns NULL if the state machine is not initialized or if the state is not
 * found.
 */
const State *SM_GetState(const char *name);

#endif