#ifndef STATE_MACHINE_INTERNAL_H
#define STATE_MACHINE_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "StateMachine.h"
#include "external/uthash.h"

// --------------------------------------------------
// Data types
// --------------------------------------------------


typedef struct {
  char *name;
  SM_EnterFn enterFn;
  SM_UpdateFn updateFn;
  SM_DrawFn drawFn;
  SM_ExitFn exitFn;
} State;

typedef struct {
  char *name;
  State *state;
  UT_hash_handle hh;
} StateMap;

typedef struct {
  StateMap *stateMap;
  const State *currState;
  int stateCount;
} StateMachineTracker;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

const State *SM_Internal_GetState(const char *name);

StateMap *SM_Internal_GetEntry(const char *name);

#endif
