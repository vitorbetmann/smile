#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// -----------------------------------------------------------------------------
// Data types
// -----------------------------------------------------------------------------

typedef void (*smEnterFn)(void *args);

typedef void (*smUpdateFn)(float dt);

typedef void (*smDrawFn)(void);

typedef void (*smExitFn)(void);

// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

// Start Related

bool smStart(void);

bool smHasStarted(void);

// State Functions

bool smCreateState(const char *stateName, smEnterFn enterFn,
                   smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn);

bool smStateExists(const char *name);

bool smSetState(const char *name, void *args);

const char *smGetCurrentStateName(void);

int smGetStateCount(void);

bool smDeleteState(const char *name);

// Lifecycle Functions

bool smUpdate(float dt);

bool smDraw(void);

// Stop Related

bool smStop(void);

#endif
