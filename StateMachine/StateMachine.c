// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "../external/stb_ds.h"

// --------------------------------------------------
// Other defines
// --------------------------------------------------
#define SM_WARN(str, ...)                                                      \
  fprintf(stderr, "\033[33m[SMILE WARNING]\033[0m " str "\n", ##__VA_ARGS__)
#define SM_ERR(str, ...)                                                       \
  fprintf(stderr, "\033[31m[SMILE ERROR]\033[0m " str "\n", ##__VA_ARGS__)

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
  const char *key;
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

  if (tracker) {
    SM_WARN("State Machine already initialized.");
    return;
  }

  tracker = malloc(sizeof(StateTracker));
  if (!tracker) {
    SM_ERR("Failed to allocate memory. State Machine not initialized.");
    return;
  }
  tracker->stateMap = NULL;
  tracker->currState = NULL;
}

void SM_RegisterState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void)) {

  if (!tracker) {
    SM_ERR("State Machine not initialized.");
    return;
  }

  if (hmget(tracker->stateMap, (char *)name)) {
    SM_WARN("A state called '%s' already exists. No new state created.", name);
    return;
  }

  State *newState = malloc(sizeof(State));
  if (!newState) {
    SM_ERR("Failed to allocate memory. No new state '%s' created.", name);
    return;
  }

  newState->name = malloc(strlen(name));
  if (!newState->name) {
    SM_ERR("Failed to allocate memory. No new state '%s' created.", name);
    free(newState);
    return;
  }
  strcpy((char *)newState->name, name);

  newState->enter = enterFn;
  newState->update = updateFn;
  newState->draw = drawFn;
  newState->exit = exitFn;

  hmput(tracker->stateMap, (char *)newState->name, newState);
}

void SM_ChangeStateTo(const char *name, void *args) {
  if (tracker->currState && tracker->currState->exit) {
    tracker->currState->exit();
  }

  State *nextState = (State *)SM_GetState(name);
  if (!nextState) {
    SM_WARN("Failed to find state '%s'. Current state not changed.", name);
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

  if (!tracker) {
    SM_WARN("Failed to shutdown. State Machine not initialized.");
    return;
  }

  if (tracker->currState && tracker->currState->exit) {
    tracker->currState->exit();
  }
  tracker->currState = NULL;

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

  if (!tracker) {
    SM_WARN("State Machine not initialized.");
    return NULL;
  }

  return tracker->currState;
}

const char *SM_GetCurrStateName(void) {

  if (!tracker) {
    SM_WARN("State Machine not initialized.");
    return NULL;
  }

  return tracker->currState ? tracker->currState->name : NULL;
}

const State *SM_GetState(const char *name) {

  if (!tracker) {
    SM_WARN("State Machine not initialized.");
    return NULL;
  }

  return hmget(tracker->stateMap, (char *)name);
}

bool SM_IsInitialized(void) { return tracker; }
