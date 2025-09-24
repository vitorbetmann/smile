// TODO(#7) trim state name when registering state and reject whitespace only
// TODO(#8) trim state name when changing states and reject whitespace only
// TODO(#9) create internal function to trim leading and trailing whitespace

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "StateMachine.h"
#include "../_Internal/Log/LogInternal.h"
#include "../_Internal/Log/LogMessages.h"
#include "../tests/StateMachine/StateMachineTest.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"
#include "src/_Internal/Test/TestInternal.h"
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

#define RETURN_FALSE_IF_NOT_INITIALIZED(funcName)                                \
do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE, CAUSE_NOT_INITIALIZED, funcName, ABORTED);               \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RETURN_NULL_IF_NOT_INITIALIZED(funcName)                                 \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE, CAUSE_NOT_INITIALIZED, funcName, ABORTED);               \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

// --------------------------------------------------
// Variables
// --------------------------------------------------

static StateMachineTracker *tracker;

// --------------------------------------------------
// Functions - Public
// --------------------------------------------------

// Init Related -------------------------------------

bool SM_Init(void) {
    if (tracker) {
        SMILE_WARN(MODULE, CAUSE_ALREADY_INITIALIZED, INIT, ABORTED);
        return false;
    }

    tracker = TEST_Calloc(1, sizeof(StateMachineTracker));
    if (!tracker) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, INIT, ABORTED);
        return false;
    }

    SMILE_INFO(MODULE, INIT, SUCCESSFUL);
    return true;
}

bool SM_IsInitialized(void) {
    return tracker;
}

// State Functions ----------------------------------

bool SM_RegisterState(const char *name, void (*enterFn)(void *), void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void)) {
    RETURN_FALSE_IF_NOT_INITIALIZED(REGISTER_STATE);

    if (!name) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_NULL_ARG, "name", REGISTER_STATE, ABORTED);
        return false;
    }

    if (strlen(name) == 0) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_EMPTY_ARG, "name", REGISTER_STATE, ABORTED);
        return false;
    }

    if (SM_IsStateRegistered((char *) name)) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_ALREADY_EXISTS, name, REGISTER_STATE, ABORTED);
        return false;
    }

    if (!enterFn && !updateFn && !drawFn && !exitFn) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_NO_VALID_FUNCTIONS, name, REGISTER_STATE, ABORTED);
        return false;
    }

    State *newState = malloc(sizeof(State));
    if (!newState) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, REGISTER_STATE, ABORTED);
        return false;
    }

    char *stateName = malloc(strlen(name) + 1);
    if (!stateName) {
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, REGISTER_STATE, ABORTED);
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
        SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, REGISTER_STATE, ABORTED);
        return false;
    }
    temp->state = newState;
    temp->name = newState->name;
    HASH_ADD_STR(tracker->stateMap, name, temp);

    tracker->stateCount++;

    SMILE_INFO_FMT(MODULE, "Created state #%d: '%s'.", tracker->stateCount, name);
    return true;
}

bool SM_IsStateRegistered(const char *name) {
    RETURN_FALSE_IF_NOT_INITIALIZED(IS_STATE_REGISTERED);

    StateMap *entry;
    HASH_FIND_STR(tracker->stateMap, name, entry);
    return entry;
}

bool SM_ChangeStateTo(const char *name, void *args) {
    RETURN_FALSE_IF_NOT_INITIALIZED(CHANGE_STATE_TO);

    if (!name) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_NULL_ARG, "name", CHANGE_STATE_TO, ABORTED);
        return false;
    }

    if (strlen(name) == 0) {
        SMILE_ERR_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_EMPTY_ARG, "name", CHANGE_STATE_TO, ABORTED);
        return false;
    }

    const State *nextState = SM_Internal_GetState(name);
    if (!nextState) {
        SMILE_WARN_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_STATE_NOT_FOUND, name, CHANGE_STATE_TO, ABORTED);
        return false;
    }

    if (tracker->currState && tracker->currState->exit) {
        tracker->currState->exit();
    }

    tracker->currState = nextState;

    if (tracker->currState && tracker->currState->enter) {
        tracker->currState->enter(args);
    }

    SMILE_INFO_WITH_ARGS(MODULE, INFO_WITH_ARGS_CHANGED_STATE_TO, name);
    return true;
}

const char *SM_GetCurrStateName(void) {
    RETURN_NULL_IF_NOT_INITIALIZED(GET_CURR_STATE_NAME);

    return tracker->currState ? tracker->currState->name : nullptr;
}

// Lifecycle Functions ------------------------------

bool SM_Update(const float dt) {
    RETURN_FALSE_IF_NOT_INITIALIZED(UPDATE);

    if (!tracker->currState) {
        SMILE_ERR(MODULE, CAUSE_NULL_CURRENT_STATE, UPDATE, ABORTED);
        return false;
    }

    if (!tracker->currState->update) {
        SMILE_WARN_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_NULL_UPDATE_FUNCTION, tracker->currState->name, UPDATE, ABORTED);
        return false;
    }

    tracker->currState->update(dt);
    return true;
}

bool SM_Draw(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(DRAW);

    if (!tracker->currState) {
        SMILE_ERR(MODULE, CAUSE_NULL_CURRENT_STATE, DRAW, ABORTED);
        return false;
    }

    if (!tracker->currState->draw) {
        SMILE_WARN_WITH_ARGS(MODULE, CAUSE_WITH_ARGS_NULL_DRAW_FUNCTION, tracker->currState->name, DRAW, ABORTED);
        return false;
    }

    tracker->currState->draw();
    return true;
}

// Shutdown -----------------------------------------

bool SM_Shutdown(void) {
    RETURN_FALSE_IF_NOT_INITIALIZED(SHUTDOWN);

    if (tracker->currState && tracker->currState->exit) {
        tracker->currState->exit();
    }
    tracker->currState = nullptr;

    StateMap *el, *tmp;
    HASH_ITER(hh, tracker->stateMap, el, tmp) {
        HASH_DEL(tracker->stateMap, el);
        free((char *) el->state->name);
        free(el->state);
        free(el);
        tracker->stateCount--;
    }

    free(tracker);
    tracker = nullptr;

    SMILE_INFO(MODULE, SHUTDOWN, SUCCESSFUL);
    return true;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

const State *SM_Internal_GetState(const char *name) {
    StateMap *sm;
    HASH_FIND_STR(tracker->stateMap, name, sm);
    return sm ? sm->state : nullptr;
}
