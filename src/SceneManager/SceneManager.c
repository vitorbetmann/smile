/**
 * @file
 * @brief Implementation of the SceneManager module.
 *
 * @see SceneManager.h
 * @see SceneManagerInternal.h
 * @see SceneManagerMessages.h
 *
 * @bug No known bugs.
 *
 * @note TODO #16 [Feature] for [SceneManager] - Create a function to limit the
 *       game's FPS to a max value
 * @note TODO #27 [Feature] for [SceneManager] - Create Internal Trim Function
 *       and Integrate into SceneManager Name Validation
 *
 * @author Vitor Betmann
 * @date 2025-10-29
 * @version 1.0.0
 */


// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <string.h>
#include <time.h>
#include <uthash.h>

#include "SceneManager.h"
#include "SceneManagerInternal.h"
#include "SceneManagerMessages.h"

#include "LogInternal.h"
#include "CommonInternal.h"
#include "CommonInternalMessages.h"
#include "TestInternal.h"
#include "SceneManagerAPITest.h"


// —————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————

static smInternalTracker *tracker;


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

static bool smPrivateIsNameValid(const char *name, const char *fnName);

/* This function was created to preserve API consistency because the argument
 * "name" from smCreateScene collided with the expected item "name" from tracker
 * and neither should be changed.
 */
static void smPrivateAddScene(smInternalSceneMap *mapEntry);


// —————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————

// Start Related

bool smStart(void)
{
    if (tracker)
    {
        lgInternalLog(WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(smInternalTracker));
    if (!tracker)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker->fps = DEFAULT_FPS; /* This is just for future reference.
                                 * For now, Smile doesn't cap the FPS.
                                 */

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return true;
}

bool smIsRunning(void)
{
    return tracker;
}

// State Functions

bool smCreateScene(const char *name, smEnterFn enter, smUpdateFn update,
                   smDrawFn draw, smExitFn exit)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_CREATE_SCENE))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_CREATE_SCENE))
    {
        return false;
    }

    smInternalSceneMap *entry = smInternalGetEntry(name);
    if (entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_ALREADY_EXISTS, name,
                             FN_CREATE_SCENE, CONSEQ_ABORTED);
        return false;
    }

    if (!enter && !update && !draw && !exit)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_NO_VALID_FUNCTIONS, name,
                             FN_CREATE_SCENE, CONSEQ_ABORTED);
        return false;
    }

    smInternalScene *state = tsInternalMalloc(sizeof(smInternalScene));
    if (!state)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_SCENE,
                      CONSEQ_ABORTED);
        return false;
    }

    const size_t NAME_SIZE = strlen(name) + 1;
    char *nameCopy = tsInternalMalloc(NAME_SIZE);
    if (!nameCopy)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_SCENE,
                      CONSEQ_ABORTED);
        goto nameCopyError;
    }
#ifdef _WIN32
    strcpy_s(nameCopy, NAME_SIZE, name); // Safe version of strcpy. Non-standard
#elif defined(__APPLE__) || defined (__linux__)
    strcpy(nameCopy, name);
#endif

    state->name = nameCopy;
    state->enter = enter;
    state->update = update;
    state->draw = draw;
    state->exit = exit;

    smInternalSceneMap *mapEntry = tsInternalMalloc(sizeof(smInternalSceneMap));
    if (!mapEntry)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_SCENE,
                      CONSEQ_ABORTED);
        goto mapEntryError;
    }
    mapEntry->state = state;
    mapEntry->name = state->name;
    smPrivateAddScene(mapEntry);

    tracker->sceneCount++;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_CREATED, name,
                         FN_CREATE_SCENE, CONSEQ_SUCCESSFUL);
    return true;

mapEntryError:
    free(nameCopy);
nameCopyError:
    free(state);
    return false;
}

bool smSceneExists(const char *name)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_SCENE_EXISTS))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_SCENE_EXISTS))
    {
        return false;
    }

    return smInternalGetEntry(name);
}

bool smSetScene(const char *name, void *args)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_SET_SCENE))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_SET_SCENE))
    {
        return false;
    }

    const smInternalScene *NEXT_STATE = smInternalGetScene(name);
    if (!NEXT_STATE)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND, name,
                             FN_SET_SCENE, CONSEQ_ABORTED);
        return false;
    }

    if (tracker->currScene && tracker->currScene->exit)
    {
#ifdef SMILE_DEVELOPER
        if (smTestExit)
        {
            smTestExit(smMockData);
        }
#endif
        tracker->currScene->exit();
    }

    tracker->currScene = NEXT_STATE;

    if (tracker->currScene && tracker->currScene->enter)
    {
#ifdef SMILE_DEVELOPER
        if (args && smTestEnterWithArgs)
        {
            smTestEnterWithArgs(smMockData, args);
        } else if (smTestEnter)
        {
            smTestEnter(smMockData);
        }
#endif
        tracker->currScene->enter(args);
    }

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_SET_TO, name,FN_SET_SCENE,
                         CONSEQ_SUCCESSFUL);
    return true;
}

const char *smGetCurrentSceneName(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_GET_CURRENT_SCENE_NAME))
    {
        return nullptr;
    }

    return tracker->currScene ? tracker->currScene->name : nullptr;
}

int smGetSceneCount(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_GET_SCENE_COUNT))
    {
        return -1;
    }

    return tracker->sceneCount;
}

bool smDeleteScene(const char *name)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_DELETE_SCENE))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_DELETE_SCENE))
    {
        return false;
    }

    if (tracker->currScene && strcmp(name, tracker->currScene->name) == 0)
    {
        lgInternalLogWithArg(ERROR, MODULE,CAUSE_CANNOT_DELETE_CURR_SCENE, name,
                             FN_DELETE_SCENE, CONSEQ_ABORTED);
        return false;
    }

    smInternalSceneMap *entry = smInternalGetEntry(name);
    if (!entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND, name,
                             FN_DELETE_SCENE, CONSEQ_ABORTED);
        return false;
    }

    HASH_DEL(tracker->sceneMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->sceneCount--;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_DELETED, name,
                         FN_DELETE_SCENE, CONSEQ_SUCCESSFUL);
    return true;
}

// Lifecycle Functions

bool smUpdate(float dt)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_UPDATE))
    {
        return false;
    }

    if (!tracker->currScene)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_SCENE, FN_UPDATE,
                      CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currScene->update)
    {
        lgInternalLogWithArg(WARNING, MODULE,CAUSE_NULL_SCENE_UPDATE_FN,
                             tracker->currScene->name, FN_UPDATE,
                             CONSEQ_ABORTED);
        return false;
    }

    tracker->currScene->update(dt);
    return true;
}

float smGetDt(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_GET_DT))
    {
        return -1.0f;
    }

    float dt;
    struct timespec currentTime;

#ifdef SMILE_DEVELOPER
    currentTime = smMockCurrTime;
#else
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
#endif

    /* On the first call, lastTime is zero-initialized, so we default to a delta
     * time based on the target FPS. This prevents an abnormally large dt, since
     * we don't know how long after program start clock_gettime() is invoked.
     */
    if (tracker->lastTime.tv_sec == 0 && tracker->lastTime.tv_nsec == 0)
    {
        dt = (float) (1.0 / tracker->fps);
    } else
    {
        double tempDt = currentTime.tv_sec - tracker->lastTime.tv_sec +
                        (currentTime.tv_nsec - tracker->lastTime.tv_nsec) / 1e9;
        dt = (float) tempDt;
    }

    tracker->lastTime = currentTime;

    return dt;
}

bool smSetFPS(int fps)
{
    return false;
}

bool smDraw(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_DRAW))
    {
        return false;
    }

    if (!tracker->currScene)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_SCENE,FN_DRAW,
                      CONSEQ_ABORTED);
        return false;
    }

    if (!tracker->currScene->draw)
    {
        lgInternalLogWithArg(WARNING, MODULE,CAUSE_NULL_SCENE_DRAW_FN,
                             tracker->currScene->name, FN_DRAW,CONSEQ_ABORTED);
        return false;
    }

    tracker->currScene->draw();
    return true;
}

// Stop Related

bool smStop(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_STOP))
    {
        return false;
    }

    if (tracker->currScene && tracker->currScene->exit)
    {
#ifdef SMILE_DEVELOPER
        if (smTestExit)
        {
            smTestExit(smMockData);
        }
#endif
        tracker->currScene->exit();
    }
    tracker->currScene = nullptr;

    smInternalSceneMap *el, *tmp;
    HASH_ITER(hh, tracker->sceneMap, el, tmp)
    {
        HASH_DEL(tracker->sceneMap, el);
        free(el->state->name);
        free(el->state);
        free(el);
        tracker->sceneCount--;
    }

    bool isFatal = false;
    if (smGetSceneCount() != 0)
    {
        isFatal = true;
    }

    free(tracker);
    tracker = nullptr;

    if (isFatal)
    {
        lgInternalLog(FATAL, MODULE, CAUSE_FAILED_TO_FREE_ALL_SCENES, FN_STOP,
                      CONSEQ_ABORTED);
        return false;
    }

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STOPPED, FN_STOP,
                  CONSEQ_SUCCESSFUL);
    return true;
}

// —————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————

const smInternalScene *smInternalGetScene(const char *name)
{
    smInternalSceneMap *entry = smInternalGetEntry(name);
    return entry ? entry->state : nullptr;
}

smInternalSceneMap *smInternalGetEntry(const char *name)
{
    smInternalSceneMap *entry;
    HASH_FIND_STR(tracker->sceneMap, name, entry);
    return entry;
}

// —————————————————————————————————————————————————————————————————————————————
// Functions - Private
// —————————————————————————————————————————————————————————————————————————————

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

void smPrivateAddScene(smInternalSceneMap *mapEntry)
{
    HASH_ADD_STR(tracker->sceneMap, name, mapEntry);
}
