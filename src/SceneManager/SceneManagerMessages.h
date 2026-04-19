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


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Module Name
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define ORI "SceneManager"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Infos
#define CSE_SCENE_CREATED "Scene Created"
#define CSE_SCENE_SET_TO "Scene Set To"
#define CSE_SCENE_DELETED "Scene Deleted"
// Warnings
#define CSE_SCENE_ALREADY_EXISTS "Scene already exists"
#define CSE_SCENE_NOT_FOUND "Scene not found"
#define CSE_NULL_SCENE_UPDATE_FN "Scene Has Null Update"
#define CSE_NULL_SCENE_DRAW_FN "Scene Has Null Draw"
// Errors
#define CSE_NULL_CURR_SCENE "Current Scene Is Null"
#define CSE_NO_VALID_FUNCTIONS "Scene Has No Valid Functions"
#define CSE_CANT_DEL_CURR_SCENE "Cannot Delete Current Scene"
#define CSE_CLOCK_GETTIME_FAILED "Clock Gettime Failed"
// Fatals
#define CSE_FAILED_TO_FREE_ALL_SCENES "Failed to Free All Scenes"


#endif
