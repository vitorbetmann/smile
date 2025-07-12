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
 * Initialize the state machine.
 */
void SM_Init(void);

/**
 * Return whether the state machine is initialized.
 */
bool SM_IsInitialized(void);

/**
 * Register a new named state with lifecycle functions.
 */
void SM_RegisterState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void));

/**
 * Switch to another state by name, optionally passing arguments.
 */
void SM_ChangeStateTo(const char *name, void *args);

/**
 * Call the update function of the current active state.
 */
void SM_Update(float dt);

/**
 * Call the draw function of the current active state.
 */
void SM_Draw(void);

/**
 * Shut down the state machine and free all resources.
 */
void SM_Shutdown(void);

/**
 * Get the name of the current active state.
 */
const char *SM_GetCurrStateName(void);

#endif