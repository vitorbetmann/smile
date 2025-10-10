/**
 * @file StateMachine.h
 * @brief `StateMachine` provides a simple, flexible system for defining and
 * controlling game flow through independent states. Each state can specify its
 * own behavior using enter, update, draw, and exit callback functions.
 *
 * @author Vitor Betmann
 */

#ifndef SMILE_STATE_MACHINE_H
#define SMILE_STATE_MACHINE_H

// -----------------------------------------------------------------------------
// Data types
// -----------------------------------------------------------------------------

/**
 * @brief Function pointer type for state entry callbacks.
 *
 * @param args Optional arguments passed when entering the state.
 *
 * @author Vitor Betmann
 */
typedef void (*smEnterFn)(const void *args);

/**
 * @brief Function pointer type for state update callbacks.
 *
 * @param dt Delta time in seconds since the last update.
 *
 * @author Vitor Betmann
 */
typedef void (*smUpdateFn)(float dt);

/**
 * @brief Function pointer type for state draw callbacks.
 *
 * @author Vitor Betmann
 */
typedef void (*smDrawFn)(void);

/**
 * @brief Function pointer type for state exit callbacks.
 *
 * @author Vitor Betmann
 */
typedef void (*smExitFn)(void);

// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

// Start Related

/**
 * @brief Initializes the state machine and prepares it for use.
 *
 * @return True if the state machine started successfully, false otherwise.
 *
 * @note Calling this function when the state machine is already started
 *       will log a warning and return false.
 *
 * @author Vitor Betmann
 */
bool smStart(void);

/**
 * @brief Checks whether the state machine has been initialized.
 *
 * @return True if the state machine has been started, false otherwise.
 *
 * @author Vitor Betmann
 */
bool smHasStarted(void);

// State Functions

/**
 * @brief Creates a new state with the specified name and callback functions.
 *
 * @param stateName Unique name identifying the state.
 * @param enterFn Callback executed when entering the state.
 * @param updateFn Callback executed each frame during update.
 * @param drawFn Callback executed each frame during rendering.
 * @param exitFn Callback executed when exiting the state.
 *
 * @return True if the state was created successfully, false otherwise.
 *
 * @note All function pointers are optional, but at least one must be provided.
 *       Attempting to create a state that already exists will fail.
 *
 * @author Vitor Betmann
 */
bool smCreateState(const char *stateName, smEnterFn enterFn,
                   smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn);

/**
 * @brief Checks whether a state with the given name exists.
 *
 * @param name Name of the state to check.
 *
 * @return True if the state exists, false otherwise.
 *
 * @author Vitor Betmann
 */
bool smStateExists(const char *name);

/**
 * @brief Sets the current active state by name and triggers its enter function.
 *
 * @param name Name of the state to switch to.
 * @param args Optional arguments passed to the state's enter function.
 *
 * @return True if the state was successfully changed, false otherwise.
 *
 * @note If a state is active, its exit function is called before switching.
 *
 * @author Vitor Betmann
 */
bool smSetState(const char *name, const void *args);

/**
 * @brief Retrieves the name of the currently active state.
 *
 * @return A pointer to the name of the current state, or nullptr if none is active.
 *
 * @note The returned string is owned by the state machine. The user must not
 *       attempt to modify or free it and should make a copy before doing so.
 *       The pointer remains valid until the state is deleted or the state
 *       machine is stopped.
 *
 * @author Vitor Betmann
 */
const char *smGetCurrentStateName(void);

/**
 * @brief Deletes a state by name from the state machine.
 *
 * @param name Name of the state to delete.
 *
 * @return True if the state was successfully deleted, false otherwise.
 *
 * @note Attempting to delete the currently active state will fail.
 *
 * @author Vitor Betmann
 */
bool smDeleteState(const char *name);

/**
 * @brief Retrieves the total number of registered states.
 *
 * @return The number of registered states, or -1 if the state machine is not started.
 *
 * @author Vitor Betmann
 */
int smGetStateCount(void);

// Lifecycle Functions

/**
 * @brief Updates the currently active state.
 *
 * @param dt Delta time in seconds since the last update.
 *
 * @return True if the update function was called successfully, false otherwise.
 *
 * @note If the current state has no update function, a warning is logged.
 *
 * @author Vitor Betmann
 */
bool smUpdate(float dt);

/**
 * @brief Executes the draw function of the currently active state.
 *
 * @return True if the draw function was called successfully, false otherwise.
 *
 * @note If the current state has no draw function, a warning is logged.
 *
 * @author Vitor Betmann
 */
bool smDraw(void);

// Stop Related

/**
 * @brief Stops the state machine and frees all allocated states.
 *
 * @return True if the state machine was successfully stopped, false otherwise.
 *
 * @note The exit function of the current state is called before cleanup.
 *       After stopping, the state machine must be restarted with smStart().
 *
 * @author Vitor Betmann
 */
bool smStop(void);

#endif
