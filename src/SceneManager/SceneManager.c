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

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// External
#include <string.h>
#include <time.h>
#include <uthash.h>
// Module Related
#include "SceneManager.h"
#include "SceneManagerInternal.h"
#include "SceneManagerMessages.h"
// Support
#include "Common.h"
#include "CommonMessages.h"
#include "LogInternal.h"
#include "SceneManagerTestHooks.h"
#include "Test.h"

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static smInternalTracker *tracker;

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static int smPrivateIsValidName(const char *name, const char *fnName);

/* Wrapper around uthash insertion to keep hash-key usage localized and keep
 * smCreateScene focused on scene construction and validation.
 */
static void smPrivateAddScene(smInternalSceneMap *mapEntry);

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Start Related

int smStart(void)
{
    if (tracker)
    {
        lgInternalLog(WARNING, MODULE, CSE_ALREADY_RUNNING, __func__,CSQ_ABORT);
        return RES_ALREADY_RUNNING;
    }

    tracker = tsCalloc(1, sizeof(smInternalTracker));
    if (!tracker)
    {
        lgInternalLog(ERROR, MODULE, CSE_MEM_ALLOC_FAIL, __func__,CSQ_ABORT);
        return RES_MEM_ALLOC_FAIL;
    }

    tracker->fps = DEFAULT_FPS; /* Stored target FPS used by smGetDt() first-call fallback.
                                 * Runtime FPS capping is not implemented.
                                 */

    lgInternalLog(INFO, MODULE, CSE_MODULE_START, __func__,CSQ_SUCCESS);
    return RES_OK;
}

bool smIsRunning(void) { return tracker; }

// State Functions

int smCreateScene(const char *name, smEnterFn enter, smUpdateFn update,
                  smDrawFn draw, smExitFn exit)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
    }

    int nameValidationResult = smPrivateIsValidName(name, __func__);
    if (nameValidationResult != RES_OK)
    {
        return nameValidationResult;
    }

    smInternalSceneMap *entry = smInternalGetEntry(name);
    if (entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_ALREADY_EXISTS, name, __func__, CSQ_ABORT);
        return RES_SCENE_ALREADY_EXISTS;
    }

    if (!enter && !update && !draw && !exit)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_NO_VALID_FUNCTIONS, name, __func__, CSQ_ABORT);
        return RES_NO_VALID_FUNCS;
    }

    smInternalScene *scene = tsMalloc(sizeof(smInternalScene));
    if (!scene)
    {
        lgInternalLog(ERROR, MODULE, CSE_MEM_ALLOC_FAIL, __func__,CSQ_ABORT);
        return RES_MEM_ALLOC_FAIL;
    }

    const size_t NAME_SIZE = strlen(name) + 1;
    char *nameCopy = tsMalloc(NAME_SIZE);
    if (!nameCopy)
    {
        lgInternalLog(ERROR, MODULE, CSE_MEM_ALLOC_FAIL, __func__,CSQ_ABORT);
        goto nameCopyError;
    }
    memcpy(nameCopy, name, NAME_SIZE);

    scene->name = nameCopy;
    scene->enter = enter;
    scene->update = update;
    scene->draw = draw;
    scene->exit = exit;

    smInternalSceneMap *mapEntry = tsMalloc(sizeof(smInternalSceneMap));
    if (!mapEntry)
    {
        lgInternalLog(ERROR, MODULE, CSE_MEM_ALLOC_FAIL, __func__,CSQ_ABORT);
        goto mapEntryError;
    }
    mapEntry->state = scene;
    mapEntry->name = scene->name;
    smPrivateAddScene(mapEntry);

    tracker->sceneCount++;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_CREATED, name, __func__,CSQ_SUCCESS);
    return RES_OK;

mapEntryError:
    free(nameCopy);
nameCopyError:
    free(scene);
    return RES_MEM_ALLOC_FAIL;
}

bool smSceneExists(const char *name)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return false;
    }

    int nameValidationResult = smPrivateIsValidName(name, __func__);
    if (nameValidationResult != RES_OK)
    {
        return false;
    }

    return smInternalGetEntry(name);
}

int smSetScene(const char *name, void *args)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
    }

    int nameValidationResult = smPrivateIsValidName(name, __func__);
    if (nameValidationResult != RES_OK)
    {
        return nameValidationResult;
    }

    const smInternalScene *NEXT_STATE = smInternalGetScene(name);
    if (!NEXT_STATE)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND, name, __func__,CSQ_ABORT);
        return RES_SCENE_NOT_FOUND;
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
        }
        else if (smTestEnter)
        {
            smTestEnter(smMockData);
        }
#endif
        tracker->currScene->enter(args);
    }

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_SET_TO, name, __func__,CSQ_SUCCESS);
    return RES_OK;
}

const char *smGetCurrentSceneName(void)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return nullptr;
    }

    return tracker->currScene ? tracker->currScene->name : nullptr;
}

int smGetSceneCount(void)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
    }

    return tracker->sceneCount;
}

int smDeleteScene(const char *name)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
    }

    int nameValidationResult = smPrivateIsValidName(name, __func__);
    if (nameValidationResult != RES_OK)
    {
        return nameValidationResult;
    }

    if (tracker->currScene && strcmp(name, tracker->currScene->name) == 0)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_CANNOT_DELETE_CURR_SCENE, name, __func__, CSQ_ABORT);
        return RES_CANNOT_DEL_CURR_SCENE;
    }

    smInternalSceneMap *entry = smInternalGetEntry(name);
    if (!entry)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND, name, __func__,CSQ_ABORT);
        return RES_SCENE_NOT_FOUND;
    }

    HASH_DEL(tracker->sceneMap, entry);
    free(entry->state->name);
    free(entry->state);
    free(entry);

    tracker->sceneCount--;

    lgInternalLogWithArg(INFO, MODULE, CAUSE_SCENE_DELETED, name, __func__,CSQ_SUCCESS);
    return RES_OK;
}

// Lifecycle Functions

int smUpdate(float dt)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
    }

    if (!tracker->currScene)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_SCENE, __func__,CSQ_ABORT);
        return RES_NO_CURR_SCENE;
    }

    if (!tracker->currScene->update)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_NULL_SCENE_UPDATE_FN, tracker->currScene->name, __func__,
                             CSQ_ABORT);
        return RES_NO_UPDATE_FUNC;
    }

    tracker->currScene->update(dt);
    return RES_OK;
}

float smGetDt(void)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
    }

    float dt;
    struct timespec currentTime;

#ifdef SMILE_DEV
    if (smMockClockGettimeFails)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_CLOCK_GETTIME_FAILED, __func__, CSQ_ABORT);
        return RES_CLOCK_GETTIME_FAIL;
    }
    currentTime = smMockCurrTime;
#else
    if (clock_gettime(CLOCK_MONOTONIC, &currentTime) != 0)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_CLOCK_GETTIME_FAILED, __func__,CSQ_ABORT);
        return RES_CLOCK_GETTIME_FAIL;
    }
#endif

    /* On the first call, lastTime is zero-initialized, so we default to a delta
     * time based on the target FPS. This prevents an abnormally large dt, since
     * we don't know how long after program start clock_gettime() is invoked.
     */
    if (tracker->lastTime.tv_sec == 0 && tracker->lastTime.tv_nsec == 0)
    {
        dt = (float)(1.0 / tracker->fps);
    }
    else
    {
        double tempDt = currentTime.tv_sec - tracker->lastTime.tv_sec
                        + (currentTime.tv_nsec - tracker->lastTime.tv_nsec) / 1e9;
        dt = (float)tempDt;
    }

    tracker->lastTime = currentTime;

    return dt;
}

int smDraw(void)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
    }

    if (!tracker->currScene)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NULL_CURR_SCENE, __func__,CSQ_ABORT);
        return RES_NO_CURR_SCENE;
    }

    if (!tracker->currScene->draw)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_NULL_SCENE_DRAW_FN, tracker->currScene->name, __func__,
                             CSQ_ABORT);
        return RES_NO_DRAW_FUNC;
    }

    tracker->currScene->draw();
    return RES_OK;
}

// Stop Related

int smStop(void)
{
    if (!cmIsRunning(smIsRunning, MODULE, __func__))
    {
        return RES_NOT_RUNNING;
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
        lgInternalLog(FATAL, MODULE, CAUSE_FAILED_TO_FREE_ALL_SCENES, __func__,CSQ_ABORT);
        return RES_FREE_ALL_SCENES_FAIL;
    }

    lgInternalLog(INFO, MODULE, CSE_MODULE_STOP, __func__,CSQ_SUCCESS);
    return RES_OK;
}

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Internal
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

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

// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Functions - Private
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int smPrivateIsValidName(const char *name, const char *fnName)
{
    if (!name)
    {
        lgInternalLogWithArg(ERROR, MODULE, CSE_NULL_ARG, "name", fnName,CSQ_ABORT);
        return RES_NULL_ARG;
    }

    if (strlen(name) == 0)
    {
        lgInternalLogWithArg(ERROR, MODULE, CSE_EMPTY_ARG, "name", fnName,CSQ_ABORT);
        return RES_EMPTY_ARG;
    }

    return RES_OK;
}

void smPrivateAddScene(smInternalSceneMap *mapEntry)
{
    HASH_ADD_STR(tracker->sceneMap, name, mapEntry);
}
