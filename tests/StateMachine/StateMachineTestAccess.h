#ifndef STATE_MACHINE_TEST_ACCESS_H
#define STATE_MACHINE_TEST_ACCESS_H

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
const StateTracker *SM_Test_GetTracker(void);

/**
 * @brief Returns the current number of registered states.
 *
 * Used for verifying internal state count in tests.
 *
 * @return const int Number of states currently registered.
 * @author Vitor Betmann
 */
const int SM_Test_GetStateCount(void);

/**
 * @brief Enables or disables simulated malloc behavior for testing.
 *
 * When set to false, SM_Test_Malloc will return NULL to simulate allocation
 * failure.
 *
 * @param toggle true to allow malloc, false to simulate failure.
 * @return true if malloc is now allowed, false if it is disabled.
 * @author Vitor Betmann
 */
bool SM_Test_SetCanMalloc(bool toggle);

/**
 * @brief Wrapper for malloc() that can be disabled for testing.
 *
 * Returns NULL when simulated failure is enabled via
 * SM_Test_SetCanMalloc(false).
 *
 * @param size Number of bytes to allocate.
 * @return void* Pointer to allocated memory or NULL if disabled.
 * @author Vitor Betmann
 */
void *SM_Test_Malloc(size_t size);

#endif