// TODO create internal function to trim leading and trailing whitespace
/* TODO trim state name and reject whitespace only when:
 * -- registering state
 * -- changing states
 * -- checking if state exists
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <string.h>
#include <time.h>
#include <external/uthash.h>

#include "StateMachine.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"

#include "src/Log/LogInternal.h"
#include "src/_Internal/Test/TestInternal.h"
#include "src/_Internal/Common/CommonInternalMessages.h"

// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

static StateMachineTracker *tracker;

// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

static bool smPrivateHasStarted(const char *fnName);

static bool smPrivateIsNameValid(const char *stateName, const char *fnName);

// -----------------------------------------------------------------------------
// Functions - Public
// -----------------------------------------------------------------------------

// Start Related

bool smStart(void) {
    if (tracker) {
        lgInternalLog(LOG_WARNING, MODULE, CAUSE_ALREADY_STARTED, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(StateMachineTracker));
    if (!tracker) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    lgInternalLog(LOG_INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return true;
}

bool smIsRunning(void) {
    return tracker;
}

// State Functions

bool smCreateState(const char *stateName, smEnterFn enterFn,
                   smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn) {
    if (!smPrivateHasStarted(FN_CREATE_STATE)) {
        return false;
    }

    if (!smPrivateIsNameValid(stateName, FN_CREATE_STATE)) {
        return false;
    }

    StateMap *entry = smInternalGetEntry(stateName);
    if (entry) {
        lgInternalLogWithArg(LOG_WARNING, MODULE, CAUSE_STATE_ALREADY_EXISTS,
                             stateName,FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    if (!enterFn && !updateFn && !drawFn && !exitFn) {
        lgInternalLogWithArg(LOG_ERROR, MODULE, CAUSE_NO_VALID_FUNCTIONS,
                             stateName,FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    State *state = tsInternalMalloc(sizeof(State));
    if (!state) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                      FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    char *nameCopy = tsInternalMalloc(strlen(stateName) + 1);
    if (!nameCopy) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                      FN_CREATE_STATE, CONSEQ_ABORTED);
        goto nameCopyError;
    }
    strcpy(nameCopy, stateName);

    state->name = nameCopy;
    state->enterFn = enterFn;
    state->updateFn = updateFn;
    state->drawFn = drawFn;
    state->exitFn = exitFn;

    StateMap *mapEntry = tsInternalMalloc(sizeof(StateMap));
    if (!mapEntry) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                      FN_CREATE_STATE, CONSEQ_ABORTED);
        goto mapEntryError;
    }
    mapEntry->state = state;
    mapEntry->name = state->name;
    HASH_ADD_STR(tracker->stateMap, name, mapEntry);

    tracker->stateCount++;

    lgInternalLogWithArg(LOG_INFO, MODULE, CAUSE_STATE_CREATED, stateName,
                         FN_CREATE_STATE, CONSEQ_SUCCESSFUL);
    return true;

mapEntryError:
    free(nameCopy);
nameCopyError:
    free(state);
    return false;
}

bool smStateExists(const char *name) {
    if (!smPrivateHasStarted(FN_STATE_EXISTS)) {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_STATE_EXISTS)) {
        return false;
    }

    return smInternalGetEntry(name);
}

bool smSetState(const char *name, const void *args) {
    if (!smPrivateHasStarted(FN_SET_STATE)) {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_SET_STATE)) {
        return false;
    }

    const State *nextState = smInternalGetState(name);
    if (!nextState) {
        lgInternalLogWithArg(LOG_WARNING, MODULE, CAUSE_STATE_NOT_FOUND,
                             name,FN_SET_STATE, CONSEQ_ABORTED);
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
    if (!smPrivateHasStarted(FN_GET_CURRENT_STATE_NAME)) {
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

int smGetStateCount(void) {
    if (!smPrivateHasStarted(FN_GET_STATE_COUNT)) {
        return -1;
    }

    return tracker->stateCount;
}

bool smDeleteState(const char *name) {
    if (!smPrivateHasStarted(FN_DELETE_STATE)) {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_DELETE_STATE)) {
        return false;
    }

    if (tracker->currState && strcmp(name, tracker->currState->name) == 0) {
        lgInternalLogWithArg(LOG_ERROR, MODULE,
                             CAUSE_CANNOT_DELETE_CURR_STATE,
                             name, FN_DELETE_STATE, CONSEQ_ABORTED);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (!entry) {
        lgInternalLogWithArg(LOG_WARNING, MODULE, CAUSE_STATE_NOT_FOUND,
                             name,FN_DELETE_STATE, CONSEQ_ABORTED);
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
    if (!smPrivateHasStarted(FN_UPDATE)) {
        return false;
    }

    if (!tracker->currState) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NULL_CURR_STATE, FN_UPDATE,
                      CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->updateFn) {
        lgInternalLogWithArg(LOG_WARNING, MODULE,
                             CAUSE_NULL_STATE_UPDATE_FN,
                             tracker->currState->name, FN_UPDATE,
                             CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->updateFn(dt);
    return true;
}

float smGetDt(void) {
    if (!smPrivateHasStarted(FN_GET_DT)) {
        return -1.0f;
    }

    clock_t currentTime = clock();
    float dt = (float) (currentTime - tracker->lastTime) / CLOCKS_PER_SEC;
    tracker->lastTime = currentTime;

    return dt;
}

bool smSetFPS(int fps) {
    return false;
}

bool smDraw(void) {
    if (!smPrivateHasStarted(FN_DRAW)) {
        return false;
    }

    if (!tracker->currState) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NULL_CURR_STATE,
                      FN_DRAW, CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->drawFn) {
        lgInternalLogWithArg(LOG_WARNING, MODULE,
                             CAUSE_NULL_STATE_DRAW_FN,
                             tracker->currState->name, FN_DRAW,
                             CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->drawFn();
    return true;
}

// Stop Related

bool smStop(void) {
    if (!smPrivateHasStarted(FN_STOP)) {
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

bool smPrivateHasStarted(const char *fnName) {
    if (!smIsRunning()) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NOT_STARTED, fnName,
                      CONSEQ_ABORTED);
        return false;
    }

    return true;
}

bool smPrivateIsNameValid(const char *stateName, const char *fnName) {
    if (!stateName) {
        lgInternalLogWithArg(LOG_ERROR, MODULE, CAUSE_NULL_ARG, "name",
                             fnName, CONSEQ_ABORTED);
        return false;
    }

    if (strlen(stateName) == 0) {
        lgInternalLogWithArg(LOG_ERROR, MODULE, CAUSE_EMPTY_ARG, "name",
                             fnName, CONSEQ_ABORTED);
        return false;
    }

    return true;
}
