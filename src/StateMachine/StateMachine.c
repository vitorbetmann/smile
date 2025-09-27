// TODO(#7) trim state name when registering state and reject whitespace only
// TODO(#8) trim state name when changing states and reject whitespace only
// TODO(#9) create internal function to trim leading and trailing whitespace

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "include/StateMachine.h"

#include <string.h>
#include <external/uthash.h>

#include "StateMachineInternal.h"
#include "StateMachineMessages.h"
#include "src/Log/LogInternal.h"
#include "src/_Internal/Test/TestInternal.h"
#include "src/_Internal/_Common/CommonMessages.h"

// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

static StateMachineTracker *tracker;

// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

static bool smHelperHasStarted(const char *fnName);

static bool smHelperIsNameValid(const char *stateName, const char *fnName);

// -----------------------------------------------------------------------------
// Functions - Public
// -----------------------------------------------------------------------------


// Start Related

bool smStart(void) {
    if (tracker) {
        lgInternalLog(LOG_WARNING,MODULE, CAUSE_ALREADY_STARTED,FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker = TEST_Calloc(1, sizeof(StateMachineTracker));
    if (!tracker) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    lgInternalLog(LOG_INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return true;
}

bool smHasStarted(void) {
    return tracker;
}

// State Functions

bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn) {
    if (!smHelperHasStarted(FN_CREATE_STATE)) {
        return false;
    }

    if (!smHelperIsNameValid(name, FN_CREATE_STATE)) {
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (entry) {
        lgInternalLogWithArg(LOG_WARNING, MODULE,
                             CAUSE_STATE_ALREADY_EXISTS,
                             name, FN_CREATE_STATE,CONSEQ_ABORTED);
        return false;
    }

    if (!enterFn && !updateFn && !drawFn && !exitFn) {
        lgInternalLogWithArg(LOG_ERROR, MODULE,CAUSE_NO_VALID_FUNCTIONS,
                             name,
                             FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    State *newState = malloc(sizeof(State));
    if (!newState) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                      FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    char *stateName = malloc(strlen(name) + 1);
    if (!stateName) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
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
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                      FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }
    temp->state = newState;
    temp->name = newState->name;
    HASH_ADD_STR(tracker->stateMap, name, temp);

    tracker->stateCount++;

    lgInternalLogWithArg(LOG_INFO, MODULE, CAUSE_STATE_CREATED, name,
                         FN_CREATE_STATE, CONSEQ_SUCCESSFUL);
    return true;
}

bool smStateExists(const char *name) {
    if (!smHelperHasStarted(FN_STATE_EXISTS)) {
        return false;
    }

    if (!smHelperIsNameValid(name, FN_STATE_EXISTS)) {
        return false;
    }

    return smInternalGetEntry(name);
}

bool smSetState(const char *name, void *args) {
    if (!smHelperHasStarted(FN_SET_STATE)) {
        return false;
    }

    if (!smHelperIsNameValid(name, FN_SET_STATE)) {
        return false;
    }

    const State *nextState = smInternalGetState(name);
    if (!nextState) {
        lgInternalLogWithArg(LOG_WARNING, MODULE, CAUSE_STATE_NOT_FOUND,
                             name,
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

    lgInternalLogWithArg(LOG_INFO, MODULE, CAUSE_STATE_SET_TO, name,
                         FN_SET_STATE, CONSEQ_SUCCESSFUL);
    return true;
}

const char *smGetCurrentStateName(void) {
    if (!smHelperHasStarted(FN_GET_CURRENT_STATE_NAME)) {
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

int smGetStateCount(void) {
    if (!smHelperHasStarted(FN_GET_STATE_COUNT)) {
        return -1;
    }

    return tracker->stateCount;
}

bool smDeleteState(const char *name) {
    if (!smHelperHasStarted(FN_DELETE_STATE)) {
        return false;
    }

    if (!smHelperIsNameValid(name, FN_DELETE_STATE)) {
        return false;
    }

    if (tracker->currState && strcmp(name, tracker->currState->name) == 0) {
        lgInternalLogWithArg(LOG_ERROR, MODULE,
                             CAUSE_CANNOT_DELETE_CURRENT_STATE,
                             name,FN_DELETE_STATE, CONSEQ_ABORTED);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (!entry) {
        lgInternalLogWithArg(LOG_WARNING, MODULE,CAUSE_STATE_NOT_FOUND,
                             name,
                             FN_DELETE_STATE, CONSEQ_ABORTED);
        return false;
    }

    HASH_DEL(tracker->stateMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->stateCount--;

    lgInternalLogWithArg(LOG_INFO, MODULE, CAUSE_STATE_DELETED, name,
                         FN_DELETE_STATE, CONSEQ_SUCCESSFUL);
    return true;
}

// Lifecycle Functions

bool smUpdate(float dt) {
    if (!smHelperHasStarted(FN_UPDATE)) {
        return false;
    }

    if (!tracker->currState) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NULL_CURRENT_STATE,FN_UPDATE,
                      CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->updateFn) {
        lgInternalLogWithArg(LOG_WARNING, MODULE,
                             CAUSE_STATE_HAS_NULL_UPDATE,
                             tracker->currState->name, FN_UPDATE,
                             CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->updateFn(dt);
    return true;
}

bool smDraw(void) {
    if (!smHelperHasStarted(FN_DRAW)) {
        return false;
    }

    if (!tracker->currState) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NULL_CURRENT_STATE,
                      FN_DRAW,CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->drawFn) {
        lgInternalLogWithArg(LOG_WARNING, MODULE,
                             CAUSE_STATE_HAS_NULL_DRAW,
                             tracker->currState->name, FN_DRAW,
                             CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->drawFn();
    return true;
}

// Stop Related

bool smStop(void) {
    if (!smHelperHasStarted(FN_STOP)) {
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

    lgInternalLog(LOG_INFO, MODULE, CAUSE_MODULE_STOPPED, FN_STOP,
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

// -----------------------------------------------------------------------------
// Functions - Helper
// -----------------------------------------------------------------------------

bool smHelperHasStarted(const char *fnName) {
    if (!smHasStarted()) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NOT_STARTED, fnName,
                      CONSEQ_ABORTED);
        return false;
    }

    return true;
}

bool smHelperIsNameValid(const char *stateName, const char *fnName) {
    if (!stateName) {
        lgInternalLogWithArg(LOG_ERROR, MODULE, CAUSE_NULL_ARG, "name",
                             fnName,CONSEQ_ABORTED);
        return false;
    }

    // TODO Sanitize name

    if (strlen(stateName) == 0) {
        lgInternalLogWithArg(LOG_ERROR, MODULE, CAUSE_EMPTY_ARG, "name",
                             fnName,CONSEQ_ABORTED);
        return false;
    }

    return true;
}
