#ifndef SMILE_STATE_MACHINE_INTERNAL_H
#define SMILE_STATE_MACHINE_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <time.h>
#include <external/uthash.h>

#include "include/StateMachine.h"

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
#if defined(_WIN32)
  // TODO add Windows support
#else
  struct timespec lastTime;
#endif
} StateMachineTracker;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

const State *smInternalGetState(const char *name);

StateMap *smInternalGetEntry(const char *name);

#endif
