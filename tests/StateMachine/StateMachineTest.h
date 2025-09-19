#ifndef STATE_MACHINE_TEST_H
#define STATE_MACHINE_TEST_H

// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateMachine.h"
#include "stdlib.h"

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Returns a pointer to the internal StateTracker.
 *
 * Used for unit testing to inspect the current state of the state machine.
 *
 * @return const StateTracker* Pointer to internal tracker, or NULL if not
 * initialized.
 * @author Vitor Betmann
 */
const StateMachineTracker *SM_Test_GetTracker(void);

/**
 * @brief Returns the current number of registered states.
 *
 * Used for verifying internal state count in tests.
 *
 * @return const int Number of states currently registered.
 * @author Vitor Betmann
 */
int SM_Test_GetStateCount(void);

#endif
