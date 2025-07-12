// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateMachine.h"
#include "SM_Error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "../external/stb_ds.h"

// --------------------------------------------------
// Data types
// --------------------------------------------------
struct State {
  const char *name;
  void (*enter)(void *args);
  void (*update)(float dt);
  void (*draw)();
  void (*exit)();
};

typedef struct {
  char *key;
  State *value;
} StateTable;

typedef struct {
  StateTable *stateMap;
  const State *currState;
} StateTracker;

// --------------------------------------------------
// Variables
// --------------------------------------------------
static StateTracker *tracker = NULL;

// --------------------------------------------------
// Functions
// --------------------------------------------------
void SM_Init(void) {

  SM_ASSERT_TRACKER_INIT();

  tracker = malloc(sizeof(StateTracker));
  SM_REQUIRE_TRACKER_OR_RETURN_VOID();

  tracker->stateMap = NULL;
  tracker->currState = NULL;
}

void NewState(const char *name, void (*enterFn)(void *),
              void (*updateFn)(float), void (*drawFn)(void),
              void (*exitFn)(void)) {

  if (hmget(tracker->stateMap, (char *)name)) {
    // TODO add warning log "name already exists"
    return;
  }

  State *newState = malloc(sizeof(State));

  newState->name = strdup(name);
  newState->enter = enterFn;
  newState->update = updateFn;
  newState->draw = drawFn;
  newState->exit = exitFn;

  hmput(tracker->stateMap, (char *)newState->name, newState);
}

void SM_ChangeState(const char *name, void *args) {
  if (tracker->currState && tracker->currState->exit) {
    tracker->currState->exit();
  }

  State *nextState = (State *)SM_GetState(name);
  if (!nextState) {
    // TODO add warning log "next state notfound"
    return;
  }
  tracker->currState = nextState;

  if (tracker->currState && tracker->currState->enter) {
    tracker->currState->enter(args);
  }
}

void SM_Update(float dt) {
  if (tracker->currState && tracker->currState->update) {
    tracker->currState->update(dt);
  }
}

void SM_Draw(void) {
  if (tracker->currState && tracker->currState->draw) {
    tracker->currState->draw();
  }
}

void SM_Shutdown(void) {
  SM_REQUIRE_TRACKER_OR_RETURN_VOID();

  if (tracker->currState && tracker->currState->exit) {
    tracker->currState->exit();
  }
  tracker->currState = NULL;

  // TODO free states and their names

  for (int i = 0; i < hmlen(tracker->stateMap); i++) {
    State *state = tracker->stateMap[i].value;
    free((char *)state->name);
    free(state);
  }
  hmfree(tracker->stateMap);

  free(tracker);
  tracker = NULL;
}

// Getters
const State *SM_GetCurrState(void) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();
  return tracker->currState;
}

const char *SM_GetCurrStateName(void) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();
  return tracker->currState ? tracker->currState->name : NULL;
}

const State *SM_GetState(const char *name) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();

  return hmget(tracker->stateMap, (char *)name);
}

bool SM_IsInitialized(void) { return tracker; }
