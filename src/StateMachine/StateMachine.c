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
        lgInternalEvent(LOG_WARNING,MODULE, cmCauses[ALREADY_STARTED],
                        cmFuncs[START], cmConseqs[ABORTED]);
        return false;
    }

    tracker = TEST_Calloc(1, sizeof(StateMachineTracker));
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[MEM_ALLOC_FAILED],
                        cmFuncs[START], cmConseqs[ABORTED]);
        return false;
    }

    lgInternalEvent(LOG_INFO, MODULE, cmCauses[MODULE_STARTED], cmFuncs[START],
                    cmConseqs[SUCCESSFUL]);
    return true;
}

bool smHasStarted(void) {
    return tracker;
}

// State Functions ----------------------------------

bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        smFuncs[CREATE_STATE], cmConseqs[ABORTED]);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[NULL_ARG],
                               "name", smFuncs[CREATE_STATE],
                               cmConseqs[ABORTED]);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[EMPTY_ARG],
                               "name", smFuncs[CREATE_STATE],
                               cmConseqs[ABORTED]);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (entry) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,
                               smCauses[STATE_ALREADY_EXISTS],
                               name, smFuncs[CREATE_STATE],
                               cmConseqs[ABORTED]);
        return false;
    }

    if (!enterFn && !updateFn && !drawFn && !exitFn) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,
                               smFuncs[NO_VALID_FUNCTIONS], name,
                               smFuncs[CREATE_STATE], cmConseqs[ABORTED]);
        return false;
    }

    State *newState = malloc(sizeof(State));
    if (!newState) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[MEM_ALLOC_FAILED],
                        smFuncs[CREATE_STATE], cmConseqs[ABORTED]);
        return false;
    }

    char *stateName = malloc(strlen(name) + 1);
    if (!stateName) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[MEM_ALLOC_FAILED],
                        smFuncs[CREATE_STATE], cmConseqs[ABORTED]);
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
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[MEM_ALLOC_FAILED],
                        smFuncs[CREATE_STATE], cmConseqs[ABORTED]);
        return false;
    }
    temp->state = newState;
    temp->name = newState->name;
    HASH_ADD_STR(tracker->stateMap, name, temp);

    tracker->stateCount++;

    lgInternalEventWithArg(LOG_INFO, MODULE, smCauses[STATE_CREATED], name,
                           smFuncs[CREATE_STATE], cmConseqs[SUCCESSFUL]);
    return true;
}

bool smStateExists(const char *name) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        smFuncs[STATE_EXISTS], cmConseqs[ABORTED]);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[NULL_ARG],
                               "name", smFuncs[STATE_EXISTS],
                               cmConseqs[ABORTED]);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[EMPTY_ARG],
                               "name", smFuncs[STATE_EXISTS],
                               cmConseqs[ABORTED]);
        return false;
    }

    return smInternalGetEntry(name);
}

bool smSetState(const char *name, void *args) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        smFuncs[SET_STATE], cmConseqs[ABORTED]);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[NULL_ARG],
                               "name", smFuncs[SET_STATE], cmConseqs[ABORTED]);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[EMPTY_ARG],
                               "name", smFuncs[SET_STATE], cmConseqs[ABORTED]);
        return false;
    }

    const State *nextState = smInternalGetState(name);
    if (!nextState) {
        lgInternalEventWithArg(LOG_WARNING, MODULE, smCauses[STATE_NOT_FOUND],
                               name, smFuncs[SET_STATE], cmConseqs[ABORTED]);
        return false;
    }

    if (tracker->currState && tracker->currState->exitFn) {
        tracker->currState->exitFn();
    }

    tracker->currState = nextState;

    if (tracker->currState && tracker->currState->enterFn) {
        tracker->currState->enterFn(args);
    }

    lgInternalEventWithArg(LOG_INFO, MODULE, smCauses[STATE_SET_TO], name,
                           smFuncs[SET_STATE], cmConseqs[SUCCESSFUL]);
    return true;
}

const char *smGetCurrentStateName(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        smFuncs[GET_CURRENT_STATE_NAME], cmConseqs[ABORTED]);
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

int smGetStateCount(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        smFuncs[GET_STATE_COUNT], cmConseqs[ABORTED]);
        return -1;
    }

    return tracker->stateCount;
}

bool smDeleteState(const char *name) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        smFuncs[DELETE_STATE], cmConseqs[ABORTED]);
        return false;
    }

    if (!name) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[NULL_ARG],
                               "name", smFuncs[DELETE_STATE],
                               cmConseqs[ABORTED]);
        return false;
    }

    // TODO Sanitize name

    if (strlen(name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE, cmCauses[EMPTY_ARG],
                               "name", smFuncs[DELETE_STATE],
                               cmConseqs[ABORTED]);
        return false;
    }

    if (tracker->currState && strcmp(name, tracker->currState->name) == 0) {
        lgInternalEventWithArg(LOG_ERROR, MODULE,
                               smCauses[CANNOT_DELETE_CURRENT_STATE], name,
                               smFuncs[DELETE_STATE], cmConseqs[ABORTED]);
        return false;
    }

    StateMap *entry = smInternalGetEntry(name);
    if (!entry) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,
                               smCauses[STATE_NOT_FOUND], name,
                               smFuncs[DELETE_STATE], cmConseqs[ABORTED]);
        return false;
    }

    HASH_DEL(tracker->stateMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->stateCount--;

    lgInternalEventWithArg(LOG_INFO, MODULE, smCauses[STATE_DELETED], name,
                           smFuncs[DELETE_STATE], cmConseqs[SUCCESSFUL]);
    return true;
}

// Lifecycle Functions ------------------------------

bool smUpdate(float dt) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        smFuncs[UPDATE], cmConseqs[ABORTED]);
        return false;
    }
    if (!tracker->currState) {
        lgInternalEvent(LOG_ERROR, MODULE, smCauses[STATE_NOT_FOUND],
                        smFuncs[UPDATE], cmConseqs[ABORTED]);
        return false;
    }

    if (!tracker->currState->updateFn) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,
                               smCauses[UPDATE_FUNCTION_IS_NULL],
                               tracker->currState->name, smFuncs[UPDATE],
                               cmConseqs[ABORTED]);
        return false;
    }

    tracker->currState->updateFn(dt);
    return true;
}

bool smDraw(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED], smFuncs[DRAW],
                        cmConseqs[ABORTED]);
        return false;
    }

    if (!tracker->currState) {
        lgInternalEvent(LOG_ERROR, MODULE, smCauses[CURRENT_STATE_IS_NULL],
                        smFuncs[DRAW], cmConseqs[ABORTED]);
        return false;
    }

    if (!tracker->currState->drawFn) {
        lgInternalEventWithArg(LOG_WARNING, MODULE,
                               smCauses[DRAW_FUNCTION_IS_NULL],
                               tracker->currState->name, smFuncs[DRAW],
                               cmConseqs[ABORTED]);
        return false;
    }

    tracker->currState->drawFn();
    return true;
}

// cmFunctions[STOP] -----------------------------------------

bool smStop(void) {
    if (!smHasStarted()) {
        lgInternalEvent(LOG_ERROR, MODULE, cmCauses[NOT_STARTED],
                        cmFuncs[STOP], cmConseqs[ABORTED]);
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

    lgInternalEvent(LOG_INFO, MODULE, cmCauses[MODULE_STOPPED], cmFuncs[STOP],
                    cmConseqs[SUCCESSFUL]);
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
