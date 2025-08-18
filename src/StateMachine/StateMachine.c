// TODO(#7) trim state name when registering state and reject whitespace only
// TODO(#8) trim state name when changing states and reject whitespace only
// TODO(#9) create internal function to trim leading and trailing whitespace

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "StateMachine.h"
#include "../_Internals/Log/LogInternal.h"
#include "../_Internals/Log/LogMessages.h"
#include "../tests/StateMachine/StateMachineTest.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

#define MODULE_NAME "StateMachine"

// Helper macro to check initialization and return (with error log)
#define RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ)                                \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED, CONSEQ);               \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RETURN_NULL_IF_NOT_INITIALIZED(CONSEQ)                                 \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED, CONSEQ);               \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

// --------------------------------------------------
// Variables
// --------------------------------------------------
static int stateCount;
static StateTracker *tracker;
static bool canMalloc = true;

// --------------------------------------------------
// Functions
// --------------------------------------------------

bool SM_Init(void) {

  if (tracker) {
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_ALREADY_INITIALIZED,
               LOG_CONSEQ_INIT_ABORTED);
    return false;
  }

  tracker = SM_Test_Malloc(sizeof(StateTracker));
  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);
    return false;
  }

  tracker->stateMap = NULL;
  tracker->currState = NULL;
  stateCount = 0;

  SMILE_INFO(MODULE_NAME, LOG_INFO_INIT_SUCCESSFUL);

  return true;
}

bool SM_IsInitialized(void) { return tracker; }

bool SM_RegisterState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void)) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_REGISTER_STATE_ABORTED);

  if (!name) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NULL_NAME,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  if (strlen(name) == 0) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_EMPTY_NAME,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  if (SM_IsStateRegistered((char *)name)) {
    SMILE_ERR_WITH_NAME(MODULE_NAME, LOG_CAUSE_ALREADY_EXISTS, name,
                        LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  if (!enterFn && !updateFn && !drawFn && !exitFn) {
    SMILE_ERR_WITH_NAME(MODULE_NAME, LOG_CAUSE_NO_VALID_FUNCTIONS, name,
                        LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  State *newState = malloc(sizeof(State));
  if (!newState) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  char *stateName = malloc(strlen(name) + 1);
  if (!stateName) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
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
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }
  temp->state = newState;
  temp->name = newState->name;
  HASH_ADD_STR(tracker->stateMap, name, temp);

  stateCount++;

  SMILE_INFO_FMT(MODULE_NAME, "%s '%s'. Total states: %d.",
                 LOG_INFO_STATE_CREATION_SUCCESSFUL, name, stateCount);

  return true;
}

bool SM_IsStateRegistered(const char *name) {
  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_IS_STATE_REGISTERED_ABORTED);

  StateMap *entry;
  HASH_FIND_STR(tracker->stateMap, name, entry);
  return entry;
}

bool SM_ChangeStateTo(const char *name, void *args) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);

  if (!name) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NULL_NAME,
              LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);
    return false;
  }

  if (strlen(name) == 0) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_EMPTY_NAME,
              LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);
    return false;
  }

  State *nextState = (State *)SM_Internal_GetState(name);
  if (!nextState) {
    SMILE_WARN_WITH_NAME(MODULE_NAME, LOG_CAUSE_STATE_NOT_FOUND, name,
                         LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);
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

  SMILE_INFO_WITH_NAME(MODULE_NAME, LOG_INFO_STATE_CHANGE_SUCCESSFUL, name);

  return true;
}

bool SM_Update(float dt) {
  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_UPDATE_ABORTED);

  State *currState = (State *)SM_Internal_GetCurrState();

  if (!currState) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_CURRENT_STATE_NULL,
              LOG_CONSEQ_UPDATE_ABORTED);
    return false;
  }

  if (!currState->update) {
    SMILE_WARN_WITH_NAME(MODULE_NAME, LOG_INFO_UPDATE_FUNCTION_NULL,
                         currState->name, LOG_CONSEQ_UPDATE_ABORTED);
    return false;
  }

  currState->update(dt);
  return true;
}

bool SM_Draw(void) {
  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_DRAW_ABORTED);

  State *currState = (State *)SM_Internal_GetCurrState();

  if (!currState) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_CURRENT_STATE_NULL,
              LOG_CONSEQ_DRAW_ABORTED);
    return false;
  }

  if (!currState->draw) {
    SMILE_WARN_WITH_NAME(MODULE_NAME, LOG_INFO_DRAW_FUNCTION_NULL,
                         currState->name, LOG_CONSEQ_DRAW_ABORTED);
    return false;
  }

  currState->draw();
  return true;
}

bool SM_Shutdown(void) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_SHUTDOWN_ABORTED);

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

  SMILE_INFO(MODULE_NAME, LOG_INFO_SHUTDOWN_SUCCESSFUL);

  return true;
}

const char *SM_GetCurrStateName(void) {

  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED,
              LOG_CONSEQ_GET_CURR_STATE_NAME_ABORTED);
    return NULL;
  }

  return tracker->currState ? tracker->currState->name : NULL;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

bool SM_Internal_SetCurrState(const State *state) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_INTERNAL_SET_CURR_STATE_ABORTED);

  tracker->currState = state;
  return true;
}

const State *SM_Internal_GetCurrState(void) {

  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED,
              LOG_CONSEQ_INTERNAL_GET_CURR_STATE_ABORTED);
    return NULL;
  }

  return tracker->currState;
}

const State *SM_Internal_GetState(const char *name) {

  RETURN_NULL_IF_NOT_INITIALIZED(LOG_CONSEQ_INTERNAL_GET_STATE_ABORTED);

  StateMap *sm;
  HASH_FIND_STR(tracker->stateMap, name, sm);
  return sm ? sm->state : NULL;
}

// --------------------------------------------------
// Functions - Tests
// --------------------------------------------------

const StateTracker *SM_Test_GetTracker(void) { return tracker; }

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