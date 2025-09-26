// TODO(#7) trim state name when registering state and reject whitespace only
// TODO(#8) trim state name when changing states and reject whitespace only
// TODO(#9) create internal function to trim leading and trailing whitespace

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <string.h>

#include "CommonMessages.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"
#include "LogInternal.h"
#include "LogMessages.h"
#include "src/_Internal/Test/TestInternal.h"

// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

static StateMachineTracker *tracker;

// -----------------------------------------------------------------------------
// Functions - Public
// -----------------------------------------------------------------------------


// Start Related -------------------------------------

bool smStart(void) {
    if (tracker) {
        lgInternalEvent(LOG_WARNING,MODULE, CAUSE_ALREADY_STARTED,FN_START,
                        CONSEQ_ABORTED);
        return false;
    }

    tracker = TEST_Calloc(1, sizeof(StateMachineTracker));
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_START,
                        CONSEQ_ABORTED);
        return false;
    }

    lgInternalEvent(LOG_INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                    CONSEQ_SUCCESSFUL);
    return true;
}

bool smHasStarted(void) {
    return tracker;
}

// State Functions ----------------------------------

bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,FN_CREATE_STATE,
                        CONSEQ_ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_NULL_ARG, "name",
                               FN_CREATE_STATE,CONSEQ_ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_EMPTY_ARG, "name",
                               FN_CREATE_STATE,CONSEQ_ABORTED);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (entry) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,CAUSE_STATE_ALREADY_EXISTS,
                               name, FN_CREATE_STATE,CONSEQ_ABORTED);
        return false;
    }

    if (!enterFn && !updateFn && !drawFn && !exitFn) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,CAUSE_NO_VALID_FUNCTIONS, name,
                               FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    State *newState = malloc(sizeof(State));
    if (!newState) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                        FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    char *stateName = malloc(strlen(name) + 1);
    if (!stateName) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                        FN_CREATE_STATE, CONSEQ_ABORTED);
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
                        FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }
    temp->state = newState;
    temp->name = newState->name;
    HASH_ADD_STR(tracker->stateMap, name, temp);

    tracker->stateCount++;

    lgInternalEventWithArg(LOG_INFO, MODULE, CAUSE_STATE_CREATED, name,
                           FN_CREATE_STATE, CONSEQ_SUCCESSFUL);
    return true;
}

bool smStateExists(const char *name) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,FN_STATE_EXISTS,
                        CONSEQ_ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_NULL_ARG, "name",
                               FN_STATE_EXISTS,CONSEQ_ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_EMPTY_ARG, "name",
                               FN_STATE_EXISTS,CONSEQ_ABORTED);
        return false;
    }

    return smInternalGetEntry(name);
}

bool smSetState(const char *name, void *args) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,FN_SET_STATE,
                        CONSEQ_ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_NULL_ARG, "name",
                               FN_SET_STATE, CONSEQ_ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_EMPTY_ARG, "name",
                               FN_SET_STATE, CONSEQ_ABORTED);
        return false;
    }

    const State *nextState = smInternalGetState(name);
    if (!nextState) {
        lgInternalEventWithArg(LOG_WARNING, MODULE, CAUSE_STATE_NOT_FOUND, name,
                               FN_SET_STATE, CONSEQ_ABORTED);
        return false;
    }

    if (tracker->currState && tracker->currState->exitFn) {
        tracker->currState->exitFn();
    }

    tracker->currState = nextState;

    if (tracker->currState && tracker->currState->enterFn) {
        tracker->currState->enterFn(args);
    }

    lgInternalEventWithArg(LOG_INFO, MODULE, CAUSE_STATE_SET_TO, name,
                           FN_SET_STATE, CONSEQ_SUCCESSFUL);
    return true;
}

const char *smGetCurrentStateName(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,
                        FN_GET_CURRENT_STATE_NAME, CONSEQ_ABORTED);
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

int smGetStateCount(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,FN_GET_STATE_COUNT,
                        CONSEQ_ABORTED);
        return -1;
    }

    return tracker->stateCount;
}

bool smDeleteState(const char *name) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,FN_DELETE_STATE,
                        CONSEQ_ABORTED);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_NULL_ARG, "name",
                               FN_DELETE_STATE,CONSEQ_ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, CAUSE_EMPTY_ARG, "name",
                               FN_DELETE_STATE,CONSEQ_ABORTED);
        return false;
    }

    if (tracker->currState && strcmp(name, tracker->currState->name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,
                               CAUSE_CANNOT_DELETE_CURRENT_STATE,
                               name,FN_DELETE_STATE, CONSEQ_ABORTED);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (!entry) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,CAUSE_STATE_NOT_FOUND, name,
                               FN_DELETE_STATE, CONSEQ_ABORTED);
        return false;
    }

    HASH_DEL(tracker->stateMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->stateCount--;

    lgInternalEventWithArg(LOG_INFO, MODULE, CAUSE_STATE_DELETED, name,
                           FN_DELETE_STATE, CONSEQ_SUCCESSFUL);
    return true;
}

// Lifecycle Functions ------------------------------

bool smUpdate(float dt) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,FN_UPDATE,
                        CONSEQ_ABORTED);
        return false;
    }
    if (!tracker->currState) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_STATE_NOT_FOUND,FN_UPDATE,
                        CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->updateFn) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,CAUSE_STATE_HAS_NULL_UPDATE,
                               tracker->currState->name, FN_UPDATE,
                               CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->updateFn(dt);
    return true;
}

bool smDraw(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED, FN_DRAW,
                        CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_CURRENT_STATE_IS_NULL,FN_DRAW,
                        CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->drawFn) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,CAUSE_STATE_HAS_NULL_DRAW,
                               tracker->currState->name, FN_DRAW,
                               CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->drawFn();
    return true;
}

// cmFunctions[STOP -----------------------------------------

bool smStop(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, CAUSE_NOT_STARTED,FN_STOP,
                        CONSEQ_ABORTED);
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

    lgInternalEvent(LOG_INFO, MODULE, CAUSE_MODULE_STOPPED, FN_STOP,
                    CONSEQ_SUCCESSFUL);
    return true;
}

// -----------------------------------------------------------------------------
// Functions - Internal
// -----------------------------------------------------------------------------

const State *smInternalGetState(const char *name) {
    StateMap *entry = smInternalGetEntry(name);
    return entry ? entry->state : nullptr;
}

StateMap *smInternalGetEntry(const char *name) {
    StateMap *entry;
    HASH_FIND_STR(tracker->stateMap, name, entry);
    return entry;
}
