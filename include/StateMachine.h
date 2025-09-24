#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

// Init Related -------------------------------------

bool SM_Init(void);

bool SM_IsInitialized(void);

// State Functions ----------------------------------

bool SM_RegisterState(const char *name, void (*enterFn)(void *), void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void));

bool SM_IsStateRegistered(const char *name);

bool SM_ChangeStateTo(const char *name, void *args);

const char *SM_GetCurrStateName(void);

int SM_GetStateCount(void);

// Lifecycle Functions ------------------------------

bool SM_Update(float dt);

bool SM_Draw(void);

// Shutdown -----------------------------------------

bool SM_Shutdown(void);

#endif
