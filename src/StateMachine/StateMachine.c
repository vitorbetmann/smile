// TODO(#7) trim state name when registering state and reject whitespace only
// TODO(#8) trim state name when changing states and reject whitespace only
// TODO(#9) create internal function to trim leading and trailing whitespace

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "StateMachine.h"
#include "../_Internals/Log/Log.h"
#include "../_Internals/Log/LogMessages.h"
#include "../tests/StateMachine/StateMachineTest.h"
#include "StateMachineInternal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

// --------------------------------------------------
// Variables
// --------------------------------------------------
static bool warningsEnabled;
static int stateCount;
static StateTracker *tracker;
static bool canMalloc = true;

// --------------------------------------------------
// Functions
// --------------------------------------------------

bool SM_Init(void) {

  if (tracker) {
    SMILE_INFO(STATE_MACHINE_NAME, LOG_MSG_ALREADY_INITIALIZED);
    return false;
  }

  tracker = SM_Test_Malloc(sizeof(StateTracker));
  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME, LOG_MSG_MEM_ALLOC_FAILED);
    return false;
  }
  tracker->stateMap = NULL;
  tracker->currState = NULL;
  stateCount = 0;

#if defined(SMILE_WARNINGS) && !defined(SMILE_RELEASE)
  SM_Internal_EnableWarnings(true);
#endif

  return true;
}

void SM_Internal_EnableWarnings(bool toggle) { warningsEnabled = toggle; }

bool SM_IsInitialized(void) { return tracker; }

bool SM_RegisterState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void)) {

  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME, "State Machine not initialized.");
    return false;
  }

  if (!name) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Can't register state with NULL name. No new state created.");
    return false;
  }

  if (strlen(name) == 0) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Can't register state with empty name. No new state created.");
    return false;
  }

  if (SM_IsStateRegistered((char *)name)) {
    SMILE_WARN(STATE_MACHINE_NAME,
               "A state called '%s' already exists. No new state created.",
               name);
    return false;
  }

  if (!enterFn && !updateFn && !drawFn && !exitFn) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "State '%s' has no valid functions. No new state created.", name);
    return false;
  }

  State *newState = malloc(sizeof(State));
  if (!newState) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Failed to allocate memory. No new state '%s' created.", name);
    return false;
  }

  char *stateName = malloc(strlen(name) + 1);
  if (!stateName) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Failed to allocate memory. No new state '%s' created.", name);
    free(newState);
    return false;
  }
  strcpy(stateName, name);

  newState->name = stateName;
  newState->enter = enterFn;
  newState->update = updateFn;
  newState->draw = drawFn;
  newState->exit = exitFn;

  StateMap *temp = malloc(sizeof(StateMap));
  if (!temp) {
    free((char *)newState->name);
    free(newState);
    SMILE_ERR(STATE_MACHINE_NAME,
              "Failed to allocate memory. No new state '%s' created.", name);
    return false;
  }
  temp->state = newState;
  temp->name = newState->name;
  HASH_ADD_STR(tracker->stateMap, name, temp);

  stateCount++;

  return true;
}

bool SM_IsStateRegistered(char *name) {
  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Can't find state. State Machine not initialized.");
    return false;
  }

  StateMap *entry;
  HASH_FIND_STR(tracker->stateMap, name, entry);
  return entry;
}

bool SM_ChangeStateTo(const char *name, void *args) {

  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Can't change state. State Machine not initialized.");
    return false;
  }

  if (!name) {
    SMILE_ERR(
        STATE_MACHINE_NAME,
        "Can't change to state with NULL name. Current state not changed.");
    return false;
  }

  if (strlen(name) == 0) {
    SMILE_ERR(
        STATE_MACHINE_NAME,
        "Can't change to state with empty name. Current state not changed.");
    return false;
  }

  State *nextState = (State *)SM_Internal_GetState(name);
  if (!nextState) {
    SMILE_WARN(STATE_MACHINE_NAME,
               "Failed to find state '%s'. Current state not changed.", name);
    return false;
  }

  State *currState = (State *)SM_Internal_GetCurrState();
  if (currState && currState->exit) {
    currState->exit();
  }

  SM_Internal_SetCurrState(nextState);

  currState = (State *)SM_Internal_GetCurrState();
  if (currState && currState->enter) {
    currState->enter(args);
  }

  return true;
}

bool SM_Update(float dt) {
  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Not possible to update. State Machine not initialized.");
    return false;
  }

  State *currState = (State *)SM_Internal_GetCurrState();

  if (!currState) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Not possible to update. No state set to current.");
    return false;
  }

  if (!currState->update) {
    SMILE_WARN(STATE_MACHINE_NAME,
               "Not possible to update state: \"%s\". Update function is NULL.",
               currState->name);
    return false;
  }

  currState->update(dt);
  return true;
}

bool SM_Draw(void) {
  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Not possible to draw. State Machine not initialized.");
    return false;
  }

  State *currState = (State *)SM_Internal_GetCurrState();

  if (!currState) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Not possible to draw. No state set to current.");
    return false;
  }

  if (!currState->draw) {
    SMILE_WARN(STATE_MACHINE_NAME,
               "Not possible to draw state: \"%s\". Draw function is NULL.",
               currState->name);
    return false;
  }

  currState->draw();
  return true;
}

bool SM_Shutdown(void) {

  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Failed to shutdown. State Machine not initialized.");
    return false;
  }

  State *currState = (State *)SM_Internal_GetCurrState();
  if (currState && currState->exit) {
    currState->exit();
  }
  SM_Internal_SetCurrState(NULL);

  StateMap *el, *tmp;
  HASH_ITER(hh, tracker->stateMap, el, tmp) {
    HASH_DEL(tracker->stateMap, el);
    free((char *)el->state->name);
    free(el->state);
    free(el);
    stateCount--;
  }

  free(tracker);
  tracker = NULL;

  return true;
}

const char *SM_GetCurrStateName(void) {

  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME,
              "Can't get current state name. State Machine not initialized.");
    return NULL;
  }

  return tracker->currState ? tracker->currState->name : NULL;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

bool SM_Internal_SetCurrState(const State *state) {
  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME, "State Machine not initialized.");
    return false;
  }

  tracker->currState = state;
  return true;
}

const State *SM_Internal_GetCurrState(void) {

  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME, "State Machine not initialized.");
    return NULL;
  }

  return tracker->currState;
}

const State *SM_Internal_GetState(const char *name) {

  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME, "State Machine Not initialized.");
    return NULL;
  }

  StateMap *sm;
  HASH_FIND_STR(tracker->stateMap, name, sm);
  return sm ? sm->state : NULL;
}

// --------------------------------------------------
// Functions - Tests
// --------------------------------------------------

const StateTracker *SM_Test_GetTracker(void) {

  if (!tracker) {
    SMILE_ERR(STATE_MACHINE_NAME, "State Machine not initialized.");
    return NULL;
  }

  return tracker;
}

const int SM_Test_GetStateCount(void) { return stateCount; }

bool SM_Test_SetCanMalloc(bool toggle) {
  canMalloc = toggle;
  return toggle;
}

void *SM_Test_Malloc(size_t size) {
  if (!canMalloc) {
    return NULL;
  }
  return malloc(size);
}