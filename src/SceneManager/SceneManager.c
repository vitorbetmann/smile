/**
 * @file
 * @brief Implementation of the SceneManager module.
 *
 * @see SceneManager.h
 * @see SceneManagerInternal.h
 * @see SceneManagerMessages.h
 *
 * @note TODO #16 [Feature] for [SceneManager] - Create a function to limit the
 *       game's FPS to a max value
 * @note TODO #27 [Feature] for [SceneManager] - Create Internal Trim Function
 *       and Integrate into SceneManager Name Validation
 *
 * @author Vitor Betmann
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
#include "SceneManagerTestHooks.h"


// —————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————

static smInternalTracker *tracker;


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

static int smPrivateIsNameValid(const char *name, const char *fnName);

/* Wrapper around uthash insertion to keep hash-key usage localized and keep
 * smCreateScene focused on scene construction and validation.
 */
static void smPrivateAddScene(smInternalSceneMap *mapEntry);


// —————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————

// Start Related

int smStart(void)
{
    if (tracker)
    {
        lgInternalLog(WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START,
                      CONSEQ_ABORTED);
        return CM_RESULT_ALREADY_RUNNING;
    }

    tracker = tsInternalCalloc(1, sizeof(smInternalTracker));
    if (!tracker)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return CM_RESULT_MEM_ALLOC_FAILED;
    }

    tracker->fps = DEFAULT_FPS;
    /* Stored target FPS used by smGetDt() first-call
                                    * fallback. Runtime FPS capping is not implemented.
                                    */

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return CM_RESULT_OK;
}

bool smIsRunning(void)
{
    return tracker;
}

// State Functions

int smCreateScene(const char *name, smEnterFn enter, smUpdateFn update,
                  smDrawFn draw, smExitFn exit)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_CREATE_SCENE))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    int nameValidationResult = smPrivateIsNameValid(name, FN_CREATE_SCENE);
    if (nameValidationResult != CM_RESULT_OK)
    {
        return nameValidationResult;
    }

    smInternalSceneMap *entry = smInternalGetEntry(name);
    if (entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_ALREADY_EXISTS, name,
                             FN_CREATE_SCENE, CONSEQ_ABORTED);
        return SM_RESULT_SCENE_ALREADY_EXISTS;
    }

    if (!enter && !update && !draw && !exit)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_NO_VALID_FUNCTIONS, name,
                             FN_CREATE_SCENE, CONSEQ_ABORTED);
        return SM_RESULT_NO_VALID_FUNCTIONS;
    }

    smInternalScene *scene = tsInternalMalloc(sizeof(smInternalScene));
    if (!scene)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_SCENE,
                      CONSEQ_ABORTED);
        return CM_RESULT_MEM_ALLOC_FAILED;
    }

    const size_t NAME_SIZE = strlen(name) + 1;
    char *nameCopy = tsInternalMalloc(NAME_SIZE);
    if (!nameCopy)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_SCENE,
                      CONSEQ_ABORTED);
        goto nameCopyError;
    }
    memcpy(nameCopy, name, NAME_SIZE);

    scene->name = nameCopy;
    scene->enter = enter;
    scene->update = update;
    scene->draw = draw;
    scene->exit = exit;

    smInternalSceneMap *mapEntry = tsInternalMalloc(sizeof(smInternalSceneMap));
    if (!mapEntry)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,FN_CREATE_SCENE,
                      CONSEQ_ABORTED);
        goto mapEntryError;
    }
    mapEntry->state = scene;
    mapEntry->name = scene->name;
    smPrivateAddScene(mapEntry);

    tracker->sceneCount++;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_CREATED, name,
                         FN_CREATE_SCENE, CONSEQ_SUCCESSFUL);
    return CM_RESULT_OK;

mapEntryError:
    free(nameCopy);
nameCopyError:
    free(scene);
    return CM_RESULT_MEM_ALLOC_FAILED;
}

bool smSceneExists(const char *name)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_SCENE_EXISTS))
    {
        return false;
    }

    int nameValidationResult = smPrivateIsNameValid(name, FN_SCENE_EXISTS);
    if (nameValidationResult != CM_RESULT_OK)
    {
        return false;
    }

    return smInternalGetEntry(name);
}

int smSetScene(const char *name, void *args)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_SET_SCENE))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    int nameValidationResult = smPrivateIsNameValid(name, FN_SET_SCENE);
    if (nameValidationResult != CM_RESULT_OK)
    {
        return nameValidationResult;
    }

    const smInternalScene *NEXT_STATE = smInternalGetScene(name);
    if (!NEXT_STATE)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND, name,
                             FN_SET_SCENE, CONSEQ_ABORTED);
        return SM_RESULT_SCENE_NOT_FOUND;
    }

    if (tracker->currScene && tracker->currScene->exit)
    {
#ifdef SMILE_DEV
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
#ifdef SMILE_DEV
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
    return CM_RESULT_OK;
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
        return CM_RESULT_NOT_RUNNING;
    }

    return tracker->sceneCount;
}

int smDeleteScene(const char *name)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_DELETE_SCENE))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    int nameValidationResult = smPrivateIsNameValid(name, FN_DELETE_SCENE);
    if (nameValidationResult != CM_RESULT_OK)
    {
        return nameValidationResult;
    }

    if (tracker->currScene && strcmp(name, tracker->currScene->name) == 0)
    {
        lgInternalLogWithArg(ERROR, MODULE,CAUSE_CANNOT_DELETE_CURR_SCENE, name,
                             FN_DELETE_SCENE, CONSEQ_ABORTED);
        return SM_RESULT_CANNOT_DELETE_CURRENT_SCENE;
    }

    smInternalSceneMap *entry = smInternalGetEntry(name);
    if (!entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND, name,
                             FN_DELETE_SCENE, CONSEQ_ABORTED);
        return SM_RESULT_SCENE_NOT_FOUND;
    }

    HASH_DEL(tracker->sceneMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->sceneCount--;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_DELETED, name,
                         FN_DELETE_SCENE, CONSEQ_SUCCESSFUL);
    return CM_RESULT_OK;
}

// Lifecycle Functions

int smUpdate(float dt)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_UPDATE))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    if (!tracker->currScene)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_SCENE, FN_UPDATE,
                      CONSEQ_ABORTED);
        return SM_RESULT_NO_CURRENT_SCENE;
    }

    if (!tracker->currScene->update)
    {
        lgInternalLogWithArg(WARNING, MODULE,CAUSE_NULL_SCENE_UPDATE_FN,
                             tracker->currScene->name, FN_UPDATE,
                             CONSEQ_ABORTED);
        return SM_RESULT_NO_UPDATE_FUNCTION;
    }

    tracker->currScene->update(dt);
    return CM_RESULT_OK;
}

float smGetDt(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_GET_DT))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    float dt;
    struct timespec currentTime;

#ifdef SMILE_DEV
    if (smMockClockGettimeFails)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_CLOCK_GETTIME_FAILED, FN_GET_DT,
                      CONSEQ_ABORTED);
        return CM_RESULT_CLOCK_GETTIME_FAILED;
    }
    currentTime = smMockCurrTime;
#else
    if (clock_gettime(CLOCK_MONOTONIC, &currentTime) != 0)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_CLOCK_GETTIME_FAILED, FN_GET_DT,
                      CONSEQ_ABORTED);
        return CM_RESULT_CLOCK_GETTIME_FAILED;
    }
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

int smDraw(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_DRAW))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    if (!tracker->currScene)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_SCENE,FN_DRAW,
                      CONSEQ_ABORTED);
        return SM_RESULT_NO_CURRENT_SCENE;
    }

    if (!tracker->currScene->draw)
    {
        lgInternalLogWithArg(WARNING, MODULE,CAUSE_NULL_SCENE_DRAW_FN,
                             tracker->currScene->name, FN_DRAW,CONSEQ_ABORTED);
        return SM_RESULT_NO_DRAW_FUNCTION;
    }

    tracker->currScene->draw();
    return CM_RESULT_OK;
}

// Stop Related

int smStop(void)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_STOP))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    if (tracker->currScene && tracker->currScene->exit)
    {
#ifdef SMILE_DEV
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
        return SM_RESULT_FREE_ALL_SCENES_FAILED;
    }

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STOPPED, FN_STOP,
                  CONSEQ_SUCCESSFUL);
    return CM_RESULT_OK;
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

int smPrivateIsNameValid(const char *name, const char *fnName)
{
    if (!name)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_NULL_ARG, "name", fnName,
                             CONSEQ_ABORTED);
        return CM_RESULT_NULL_ARG;
    }

    if (strlen(name) == 0)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_EMPTY_ARG, "name", fnName,
                             CONSEQ_ABORTED);
        return CM_RESULT_EMPTY_ARG;
    }

    return CM_RESULT_OK;
}

void smPrivateAddScene(smInternalSceneMap *mapEntry)
{
    HASH_ADD_STR(tracker->sceneMap, name, mapEntry);
}
