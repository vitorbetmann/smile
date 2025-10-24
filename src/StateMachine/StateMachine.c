// TODO #16 [Feature] for [StateMachine] - Create a function to limit the game's FPS to a max value

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <string.h>
#include <../external/uthash.h>

#include "../include/StateMachine.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"

#include "../src/Log/LogInternal.h"
#include "../src/_Internal/Common/CommonInternalMessages.h"
#include "../src/_Internal/Test/TestInternal.h"


// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

static InternalTracker *tracker;


// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

static bool smPrivateIsRunning(const char *fnName);

static bool smPrivateIsNameValid(const char *name, const char *fnName);

/* This function was created to preserve API consistency because the argument
 * "name" from smCreateState collided with the expected item "name" from tracker
 * and neither should be changed.
 */
static void smPrivateAddState(InternalStateMap *mapEntry);

// -----------------------------------------------------------------------------
// Functions - Public
// -----------------------------------------------------------------------------

// Start Related

bool smStart(void)
{
    if (tracker)
    {
        lgInternalLog(WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(InternalTracker));
    if (!tracker)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker->fps = DEFAULT_FPS;

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return true;
}

bool smIsRunning(void)
{
    return tracker;
}

// State Functions

bool smCreateState(const char *name, smEnterFn enter, smUpdateFn update,
                   smDrawFn draw, smExitFn exit)
{
    if (!smPrivateIsRunning(FN_CREATE_STATE))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_CREATE_STATE))
    {
        return false;
    }

    InternalStateMap *entry = smInternalGetEntry(name);
    if (entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_STATE_ALREADY_EXISTS, name,
                             FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    if (!enter && !update && !draw && !exit)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_NO_VALID_FUNCTIONS, name,
                             FN_CREATE_STATE, CONSEQ_ABORTED);
        return false;
    }

    InternalState *state = tsInternalMalloc(sizeof(InternalState));
    if (!state)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_STATE,
                      CONSEQ_ABORTED);
        return false;
    }

    char *nameCopy = tsInternalMalloc(strlen(name) + 1);
    if (!nameCopy)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_STATE,
                      CONSEQ_ABORTED);
        goto nameCopyError;
    }
    strcpy(nameCopy, name);

    state->name = nameCopy;
    state->enter = enter;
    state->update = update;
    state->draw = draw;
    state->exit = exit;

    InternalStateMap *mapEntry = tsInternalMalloc(sizeof(InternalStateMap));
    if (!mapEntry)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_STATE,
                      CONSEQ_ABORTED);
        goto mapEntryError;
    }
    mapEntry->state = state;
    mapEntry->name = state->name;
    smPrivateAddState(mapEntry);

    tracker->stateCount++;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_STATE_CREATED, name,
                         FN_CREATE_STATE, CONSEQ_SUCCESSFUL);
    return true;

mapEntryError:
    free(nameCopy);
nameCopyError:
    free(state);
    return false;
}

bool smStateExists(const char *name)
{
    if (!smPrivateIsRunning(FN_STATE_EXISTS))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_STATE_EXISTS))
    {
        return false;
    }

    return smInternalGetEntry(name);
}

bool smSetState(const char *name, const void *args)
{
    if (!smPrivateIsRunning(FN_SET_STATE))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_SET_STATE))
    {
        return false;
    }

    const InternalState *nextState = smInternalGetState(name);
    if (!nextState)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_STATE_NOT_FOUND, name,
                             FN_SET_STATE, CONSEQ_ABORTED);
        return false;
    }

    if (tracker->currState && tracker->currState->exit)
    {
        tracker->currState->exit();
    }

    tracker->currState = nextState;

    if (tracker->currState && tracker->currState->enter)
    {
        tracker->currState->enter(args);
    }

    lgInternalLogWithArg(INFO, MODULE, CAUSE_STATE_SET_TO, name,FN_SET_STATE,
                         CONSEQ_SUCCESSFUL);
    return true;
}

const char *smGetCurrentStateName(void)
{
    if (!smPrivateIsRunning(FN_GET_CURRENT_STATE_NAME))
    {
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

int smGetStateCount(void)
{
    if (!smPrivateIsRunning(FN_GET_STATE_COUNT))
    {
        return -1;
    }

    return tracker->stateCount;
}

bool smDeleteState(const char *name)
{
    if (!smPrivateIsRunning(FN_DELETE_STATE))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_DELETE_STATE))
    {
        return false;
    }

    if (tracker->currState && strcmp(name, tracker->currState->name) == 0)
    {
        lgInternalLogWithArg(ERROR, MODULE,CAUSE_CANNOT_DELETE_CURR_STATE, name,
                             FN_DELETE_STATE, CONSEQ_ABORTED);
        return false;
    }

    InternalStateMap *entry = smInternalGetEntry(name);
    if (!entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_STATE_NOT_FOUND, name,
                             FN_DELETE_STATE, CONSEQ_ABORTED);
        return false;
    }

    HASH_DEL(tracker->stateMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->stateCount--;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_STATE_DELETED, name,
                         FN_DELETE_STATE, CONSEQ_SUCCESSFUL);
    return true;
}

// Lifecycle Functions

bool smUpdate(float dt)
{
    if (!smPrivateIsRunning(FN_UPDATE))
    {
        return false;
    }

    if (!tracker->currState)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_STATE, FN_UPDATE,
                      CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->update)
    {
        lgInternalLogWithArg(WARNING, MODULE,CAUSE_NULL_STATE_UPDATE_FN,
                             tracker->currState->name, FN_UPDATE,
                             CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->update(dt);
    return true;
}

float smGetDt(void)
{
    if (!smPrivateIsRunning(FN_GET_DT))
    {
        return -1.0f;
    }

    float dt;

#if defined(_WIN32)
    // TODO add Windows support
#elif defined(__APPLE__) || defined(__linux__)
    struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    if (tracker->lastTime.tv_sec == 0 && tracker->lastTime.tv_nsec == 0)
    {
        dt = 1.0f / tracker->fps;
    } else
    {
        dt = (currentTime.tv_sec - tracker->lastTime.tv_sec)
             + (currentTime.tv_nsec - tracker->lastTime.tv_nsec) / 1e9f;
    }
    tracker->lastTime = currentTime;
#endif

    return dt;
}

bool smSetFPS(int fps)
{
    return false;
}

bool smDraw(void)
{
    if (!smPrivateIsRunning(FN_DRAW))
    {
        return false;
    }

    if (!tracker->currState)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_STATE,FN_DRAW,
                      CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currState->draw)
    {
        lgInternalLogWithArg(WARNING, MODULE,CAUSE_NULL_STATE_DRAW_FN,
                             tracker->currState->name, FN_DRAW,CONSEQ_ABORTED);
        return false;
    }

    tracker->currState->draw();
    return true;
}

// Stop Related

bool smStop(void)
{
    if (!smPrivateIsRunning(FN_STOP))
    {
        return false;
    }

    if (tracker->currState && tracker->currState->exit)
    {
        tracker->currState->exit();
    }
    tracker->currState = nullptr;

    InternalStateMap *el, *tmp;
    HASH_ITER(hh, tracker->stateMap, el, tmp)
    {
        HASH_DEL(tracker->stateMap, el);
        free(el->state->name);
        free(el->state);
        free(el);
    }

    free(tracker);
    tracker = nullptr;

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STOPPED, FN_STOP,
                  CONSEQ_SUCCESSFUL);
    return true;
}

// -----------------------------------------------------------------------------
// Functions - Internal
// -----------------------------------------------------------------------------

const InternalState *smInternalGetState(const char *name)
{
    InternalStateMap *entry = smInternalGetEntry(name);
    return entry ? entry->state : nullptr;
}

InternalStateMap *smInternalGetEntry(const char *name)
{
    InternalStateMap *entry;
    HASH_FIND_STR(tracker->stateMap, name, entry);
    return entry;
}

// -----------------------------------------------------------------------------
// Functions - Helper
// -----------------------------------------------------------------------------

bool smPrivateIsRunning(const char *fnName)
{
    if (!smIsRunning())
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NOT_RUNNING, fnName,CONSEQ_ABORTED);
        return false;
    }

    return true;
}

bool smPrivateIsNameValid(const char *name, const char *fnName)
{
    if (!name)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_NULL_ARG, "name", fnName,
                             CONSEQ_ABORTED);
        return false;
    }

    if (strlen(name) == 0)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_EMPTY_ARG, "name", fnName,
                             CONSEQ_ABORTED);
        return false;
    }

    return true;
}

void smPrivateAddState(InternalStateMap *mapEntry)
{
    HASH_ADD_STR(tracker->stateMap, name, mapEntry);
}
