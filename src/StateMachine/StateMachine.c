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
#include "src/_Internals/Test/TestInternal.h"
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------
// Defines - values
// --------------------------------------------------

#define MODULE_NAME "StateMachine"

// --------------------------------------------------
// Defines - funcs
// --------------------------------------------------

#define RETURN_FALSE_IF_NOT_INITIALIZED(conseq)                                \
do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, CAUSE_NOT_INITIALIZED, conseq);               \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RETURN_NULL_IF_NOT_INITIALIZED(conseq)                                 \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, CAUSE_NOT_INITIALIZED, conseq);               \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

// --------------------------------------------------
// Variables
// --------------------------------------------------

static int stateCount;
static StateMachineTracker *tracker;

// --------------------------------------------------
// Functions
// --------------------------------------------------

bool SM_Init(void) {
    if (tracker) {
        SMILE_WARN(MODULE_NAME, CAUSE_ALREADY_INITIALIZED,
                   CONSEQ_INIT_ABORTED);
        return false;
    }

    tracker = TEST_Malloc(sizeof(StateMachineTracker));
    if (!tracker) {
        SMILE_ERR(MODULE_NAME, CAUSE_MEM_ALLOC_FAILED, CONSEQ_INIT_ABORTED);
        return false;
    }

    tracker->stateMap = nullptr;
    tracker->currState = nullptr;
    stateCount = 0;

    SMILE_INFO(MODULE_NAME, INFO_INIT_SUCCESSFUL);

    return true;
}

bool SM_IsInitialized(void) { return tracker; }

bool SM_RegisterState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void)) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_REGISTER_STATE_ABORTED);

    if (!name) {
        SMILE_ERR(MODULE_NAME, CAUSE_STATE_NAME_NULL, CONSEQ_REGISTER_STATE_ABORTED);
        return false;
    }

    if (strlen(name) == 0) {
        SMILE_ERR(MODULE_NAME, CAUSE_STATE_NAME_EMPTY, CONSEQ_REGISTER_STATE_ABORTED);
        return false;
    }

    if (SM_IsStateRegistered((char *) name)) {
        SMILE_ERR_WITH_ARGS(MODULE_NAME, CAUSE_ALREADY_EXISTS, name,
                            CONSEQ_REGISTER_STATE_ABORTED);
        return false;
    }

    if (!enterFn && !updateFn && !drawFn && !exitFn) {
        SMILE_ERR_WITH_ARGS(MODULE_NAME, CAUSE_NO_VALID_FUNCTIONS, name,
                            CONSEQ_REGISTER_STATE_ABORTED);
        return false;
    }

    State *newState = malloc(sizeof(State));
    if (!newState) {
        SMILE_ERR(MODULE_NAME, CAUSE_MEM_ALLOC_FAILED,
                  CONSEQ_REGISTER_STATE_ABORTED);
        return false;
    }

    char *stateName = malloc(strlen(name) + 1);
    if (!stateName) {
        SMILE_ERR(MODULE_NAME, CAUSE_MEM_ALLOC_FAILED,
                  CONSEQ_REGISTER_STATE_ABORTED);
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
        free((char *) newState->name);
        free(newState);
        SMILE_ERR(MODULE_NAME, CAUSE_MEM_ALLOC_FAILED,
                  CONSEQ_REGISTER_STATE_ABORTED);
        return false;
    }
    temp->state = newState;
    temp->name = newState->name;
    HASH_ADD_STR(tracker->stateMap, name, temp);

    stateCount++;

    SMILE_INFO_FMT(MODULE_NAME, "%s '%s'. Total states: %d.",
                   INFO_STATE_CREATION_SUCCESSFUL, name, stateCount);

    return true;
}

bool SM_IsStateRegistered(const char *name) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_IS_STATE_REGISTERED_ABORTED);

    StateMap *entry;
    HASH_FIND_STR(tracker->stateMap, name, entry);
    return entry;
}

bool SM_ChangeStateTo(const char *name, void *args) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_CHANGE_STATE_TO_ABORTED);

    if (!name) {
        SMILE_ERR(MODULE_NAME, CAUSE_STATE_NAME_NULL, CONSEQ_CHANGE_STATE_TO_ABORTED);
        return false;
    }

    if (strlen(name) == 0) {
        SMILE_ERR(MODULE_NAME, CAUSE_STATE_NAME_EMPTY, CONSEQ_CHANGE_STATE_TO_ABORTED);
        return false;
    }

    State *nextState = (State *) SM_Internal_GetState(name);
    if (!nextState) {
        SMILE_WARN_WITH_ARGS(MODULE_NAME, CAUSE_STATE_NOT_FOUND, name, CONSEQ_CHANGE_STATE_TO_ABORTED);
        return false;
    }

    State *currState = (State *) SM_Internal_GetCurrState();
    if (currState && currState->exit) {
        currState->exit();
    }

    SM_Internal_SetCurrState(nextState);

    currState = (State *) SM_Internal_GetCurrState();
    if (currState && currState->enter) {
        currState->enter(args);
    }

    SMILE_INFO_WITH_NAME(MODULE_NAME, INFO_STATE_CHANGE_SUCCESSFUL, name);

    return true;
}

bool SM_Update(float dt) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_UPDATE_ABORTED);

    State *currState = (State *) SM_Internal_GetCurrState();

    if (!currState) {
        SMILE_ERR(MODULE_NAME, CAUSE_CURRENT_STATE_NULL,
                  CONSEQ_UPDATE_ABORTED);
        return false;
    }

    if (!currState->update) {
        SMILE_WARN_WITH_ARGS(MODULE_NAME, INFO_UPDATE_FUNCTION_NULL,
                             currState->name, CONSEQ_UPDATE_ABORTED);
        return false;
    }

    currState->update(dt);
    return true;
}

bool SM_Draw(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_DRAW_ABORTED);

    State *currState = (State *) SM_Internal_GetCurrState();

    if (!currState) {
        SMILE_ERR(MODULE_NAME, CAUSE_CURRENT_STATE_NULL,
                  CONSEQ_DRAW_ABORTED);
        return false;
    }

    if (!currState->draw) {
        SMILE_WARN_WITH_ARGS(MODULE_NAME, INFO_DRAW_FUNCTION_NULL,
                             currState->name, CONSEQ_DRAW_ABORTED);
        return false;
    }

    currState->draw();
    return true;
}

bool SM_Shutdown(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_SHUTDOWN_ABORTED);

    State *currState = (State *) SM_Internal_GetCurrState();
    if (currState && currState->exit) {
        currState->exit();
    }
    SM_Internal_SetCurrState(nullptr);

    StateMap *el, *tmp;
    HASH_ITER(hh, tracker->stateMap, el, tmp) {
        HASH_DEL(tracker->stateMap, el);
        free((char *) el->state->name);
        free(el->state);
        free(el);
        stateCount--;
    }

    free(tracker);
    tracker = nullptr;

    SMILE_INFO(MODULE_NAME, INFO_SHUTDOWN_SUCCESSFUL);

    return true;
}

const char *SM_GetCurrStateName(void) {
    if (!tracker) {
        SMILE_ERR(MODULE_NAME, CAUSE_NOT_INITIALIZED,
                  CONSEQ_GET_CURR_STATE_NAME_ABORTED);
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

bool SM_Internal_SetCurrState(const State *state) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CONSEQ_INTERNAL_SET_CURR_STATE_ABORTED);

    tracker->currState = state;
    return true;
}

const State *SM_Internal_GetCurrState(void) {
    if (!tracker) {
        SMILE_ERR(MODULE_NAME, CAUSE_NOT_INITIALIZED,
                  CONSEQ_INTERNAL_GET_CURR_STATE_ABORTED);
        return nullptr;
    }

    return tracker->currState;
}

const State *SM_Internal_GetState(const char *name) {
    RETURN_NULL_IF_NOT_INITIALIZED(CONSEQ_INTERNAL_GET_STATE_ABORTED);

    StateMap *sm;
    HASH_FIND_STR(tracker->stateMap, name, sm);
    return sm ? sm->state : nullptr;
}

// --------------------------------------------------
// Functions - Tests
// --------------------------------------------------

const StateMachineTracker *SM_Test_GetTracker(void) { return tracker; }

int SM_Test_GetStateCount(void) { return stateCount; }
