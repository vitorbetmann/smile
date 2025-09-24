#ifndef STATE_MACHINE_INTERNAL_H
#define STATE_MACHINE_INTERNAL_H

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "../../external/uthash.h"

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef struct {
  const char *name;

  void (*enter)(void *args);

  void (*update)(float dt);

  void (*draw)();

  void (*exit)();
} State;

typedef struct {
  const char *name;
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

#endif
