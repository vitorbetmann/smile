/**
 * @file
 * @brief Message definitions for the SceneManager module.
 *
 * @see SceneManager.c
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_SCENE_MANAGER_MESSAGES_H
#define SMILE_SCENE_MANAGER_MESSAGES_H


// —————————————————————————————————————————————————————————————————————————————
// Module Name
// —————————————————————————————————————————————————————————————————————————————

#define MODULE "SceneManager"


// —————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————

// Infos
#define CAUSE_SCENE_CREATED "Scene Created"
#define CAUSE_SCENE_SET_TO "Scene Set To"
#define CAUSE_SCENE_DELETED "Scene Deleted"
// Warnings
#define CAUSE_SCENE_ALREADY_EXISTS "Scene already exists"
#define CAUSE_SCENE_NOT_FOUND "Scene not found"
#define CAUSE_NULL_SCENE_UPDATE_FN "Scene Has Null Update"
#define CAUSE_NULL_SCENE_DRAW_FN "Scene Has Null Draw"
// Errors
#define CAUSE_NULL_CURR_SCENE "Current Scene Is Null"
#define CAUSE_NO_VALID_FUNCTIONS "Scene Has No Valid Functions"
#define CAUSE_CANNOT_DELETE_CURR_SCENE "Cannot Delete Current Scene"
#define CAUSE_CLOCK_GETTIME_FAILED "Clock Gettime Failed"
// Fatals
#define CAUSE_FAILED_TO_FREE_ALL_SCENES "Failed to Free All Scenes"


#endif // #ifndef SMILE_SCENE_MANAGER_MESSAGES_H
