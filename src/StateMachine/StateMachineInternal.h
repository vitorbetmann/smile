#ifndef SMILE_STATE_MACHINE_INTERNAL_H
#define SMILE_STATE_MACHINE_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "../../include/StateMachine.h"
#include <../../external/uthash.h>

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef struct {
  char *name;
  smEnterFn enterFn;
  smUpdateFn updateFn;
  smDrawFn drawFn;
  smExitFn exitFn;
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

const State *smInternalGetState(const char *name);

StateMap *smInternalGetEntry(const char *name);

#endif
