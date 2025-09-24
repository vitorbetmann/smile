#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef void (*SM_EnterFn)(void *args);

typedef void (*SM_UpdateFn)(float dt);

typedef void (*SM_DrawFn)(void);

typedef void (*SM_ExitFn)(void);

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

// Init Related -------------------------------------

bool SM_Init(void);

bool SM_IsInitialized(void);

// State Functions ----------------------------------

bool SM_RegisterState(const char *name, SM_EnterFn enterFn,
                      SM_UpdateFn updateFn, SM_DrawFn drawFn, SM_ExitFn exitFn);

bool SM_IsStateRegistered(const char *name);

bool SM_ChangeStateTo(const char *name, void *args);

const char *SM_GetCurrStateName(void);

int SM_GetStateCount(void);

bool SM_UnregisterState(const char *name);

// Lifecycle Functions ------------------------------

bool SM_Update(float dt);

bool SM_Draw(void);

// Shutdown -----------------------------------------

bool SM_Shutdown(void);

#endif
