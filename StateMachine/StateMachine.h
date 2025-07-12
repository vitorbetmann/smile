#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
// --------------------------------------------------
// Data types
// --------------------------------------------------
typedef struct State State;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * Initializes the SMILE state machine.
 */
void SM_Init(void);

/**
 * Returns true if the state machine has been initialized.
 */
bool SM_IsInitialized(void);

/**
 * Registers a new state with the SMILE state machine.
 */
void SM_RegisterState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void));

/**
 * Changes the current active state.
 */
void SM_ChangeStateTo(const char *name, void *args);

/**
 * Updates the current active state.
 */
void SM_Update(float dt);

/**
 * Draws the current active state.
 */
void SM_Draw(void);

/**
 * Shuts down the SMILE state machine.
 */
void SM_Shutdown(void);

// Getters
/**
 * Returns the currently active state.
 */
const State *SM_GetCurrState(void);

/**
 * Returns the `StateID` of the currently active state.
 */
const char *SM_GetCurrStateName(void);

/**
 * Finds a state by its registered name.
 */
const State *SM_GetState(const char *name);

#endif