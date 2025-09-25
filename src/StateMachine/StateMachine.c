// TODO(#7) trim state name when registering state and reject whitespace only
// TODO(#8) trim state name when changing states and reject whitespace only
// TODO(#9) create internal function to trim leading and trailing whitespace

// --------------------------------------------------
// Includes
// --------------------------------------------------

#include <string.h>

#include "CommonMessages.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"
#include "LogInternal.h"
#include "LogMessages.h"
#include "src/_Internal/Test/TestInternal.h"

// --------------------------------------------------
// Variables
// --------------------------------------------------

static StateMachineTracker *tracker;

// --------------------------------------------------
// Functions - Public
// --------------------------------------------------

// Init Related -------------------------------------

bool smStart(void) {
    if (tracker) {
        lgInternalEvent(LOG_WARNING,MODULE, CAUSE_ALREADY_INITIALIZED, INIT,
                        ABORTED);
        return false;
    }

    tracker = TEST_Calloc(1, sizeof(StateMachineTracker));
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, INIT,
                        ABORTED);
        return false;
    }

    lgInternalEvent(LOG_INFO, MODULE, "",INIT, SUCCESSFUL);
    return true;
}

bool smHasStarted(void) {
    return tracker;
}

// State Functions ----------------------------------

bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED,
                        REGISTER_STATE, ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_NULL_ARG,
                               "name",REGISTER_STATE, ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_EMPTY_ARG,
                               "name",REGISTER_STATE, ABORTED);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (entry) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,CAUSE_WITH_ARGS_ALREADY_EXISTS,
                               name,REGISTER_STATE, ABORTED);
        return false;
    }

    if (!enterFn && !updateFn && !drawFn && !exitFn) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,
                               CAUSE_WITH_ARGS_NO_VALID_FUNCTIONS, name,
                               REGISTER_STATE, ABORTED);
        return false;
    }

    State *newState = malloc(sizeof(State));
    if (!newState) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                        REGISTER_STATE, ABORTED);
        return false;
    }

    char *stateName = malloc(strlen(name) + 1);
    if (!stateName) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                        REGISTER_STATE, ABORTED);
        free(newState);
        return false;
    }
    strcpy(stateName, name);

    newState->name = stateName;
    newState->enterFn = enterFn;
    newState->updateFn = updateFn;
    newState->drawFn = drawFn;
    newState->exitFn = exitFn;

    StateMap *temp = malloc(sizeof(StateMap));
    if (!temp) {
        free(newState->name);
        free(newState);
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                        REGISTER_STATE, ABORTED);
        return false;
    }
    temp->state = newState;
    temp->name = newState->name;
    HASH_ADD_STR(tracker->stateMap, name, temp);

    tracker->stateCount++;

    lgInternalEventFmt(LOG_INFO, MODULE,
                       "Created state: '%s'. State count: %d.", name,
                       tracker->stateCount);
    return true;
}

bool smStateExists(const char *name) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED,
                        IS_STATE_REGISTERED, ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_NULL_ARG,
                               "name",IS_STATE_REGISTERED, ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_EMPTY_ARG,
                               "name",IS_STATE_REGISTERED, ABORTED);
        return false;
    }

    return smInternalGetEntry(name);
}

bool smSetState(const char *name, void *args) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED,
                        CHANGE_STATE_TO, ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_NULL_ARG,
                               "name",CHANGE_STATE_TO, ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_EMPTY_ARG,
                               "name",CHANGE_STATE_TO, ABORTED);
        return false;
    }

    const State *nextState = smInternalGetState(name);
    if (!nextState) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,
                               CAUSE_WITH_ARGS_STATE_NOT_FOUND, name,
                               CHANGE_STATE_TO, ABORTED);
        return false;
    }

    if (tracker->currState && tracker->currState->exitFn) {
        tracker->currState->exitFn();
    }

    tracker->currState = nextState;

    if (tracker->currState && tracker->currState->enterFn) {
        tracker->currState->enterFn(args);
    }

    // SMILE_INFO_WITH_ARGS(MODULE, INFO_WITH_ARGS_CHANGED_STATE_TO, name);
    return true;
}

const char *smGetCurrentStateName(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED,
                        GET_CURR_STATE_NAME, ABORTED);
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

int smGetStateCount(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED,
                        GET_STATE_COUNT, ABORTED);
        return -1;
    }

    return tracker->stateCount;
}

bool smDestroyState(const char *name) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED,
                        UNREGISTER_STATE, ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_NULL_ARG,
                               "name",UNREGISTER_STATE, ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_WITH_ARGS_EMPTY_ARG,
                               "name",UNREGISTER_STATE, ABORTED);
        return false;
    }

    if (tracker->currState && strcmp(name, tracker->currState->name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,
                               CAUSE_WITH_ARGS_STATE_IS_ACTIVE, name,
                               UNREGISTER_STATE, ABORTED);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (!entry) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,
                               CAUSE_WITH_ARGS_STATE_NOT_FOUND, name,
                               UNREGISTER_STATE, ABORTED);
        return false;
    }

    HASH_DEL(tracker->stateMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->stateCount--;

    lgInternalEventFmt(LOG_INFO, MODULE,
                       "Destroyed state: '%s'. Total count: %d.", name,
                       tracker->stateCount);
    return true;
}

// Lifecycle Functions ------------------------------

bool smUpdate(float dt) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED, UPDATE,
                        ABORTED);
        return false;
    }
    if (!tracker->currState) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NULL_CURRENT_STATE, UPDATE,
                        ABORTED);
        return false;
    }

    if (!tracker->currState->updateFn) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,
                               CAUSE_WITH_ARGS_NULL_UPDATE_FUNCTION,
                               tracker->currState->name, UPDATE, ABORTED);
        return false;
    }

    tracker->currState->updateFn(dt);
    return true;
}

bool smDraw(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED, DRAW,
                        ABORTED);
        return false;
    }

    if (!tracker->currState) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NULL_CURRENT_STATE, DRAW,
                        ABORTED);
        return false;
    }

    if (!tracker->currState->drawFn) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,
                               CAUSE_WITH_ARGS_NULL_DRAW_FUNCTION,
                               tracker->currState->name, DRAW, ABORTED);
        return false;
    }

    tracker->currState->drawFn();
    return true;
}

// Shutdown -----------------------------------------

bool smStop(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_INITIALIZED, SHUTDOWN,
                        ABORTED);
        return false;
    }

    if (tracker->currState && tracker->currState->exitFn) {
        tracker->currState->exitFn();
    }
    tracker->currState = nullptr;

    StateMap *el, *tmp;
    HASH_ITER(hh, tracker->stateMap, el, tmp) {
        HASH_DEL(tracker->stateMap, el);
        free(el->state->name);
        free(el->state);
        free(el);
    }

    free(tracker);
    tracker = nullptr;

    //  SMILE_INFO(MODULE, SHUTDOWN, SUCCESSFUL);
    return true;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

const State *smInternalGetState(const char *name) {
    StateMap *entry = smInternalGetEntry(name);
    return entry ? entry->state : nullptr;
}

StateMap *smInternalGetEntry(const char *name) {
    StateMap *entry;
    HASH_FIND_STR(tracker->stateMap, name, entry);
    return entry;
}
