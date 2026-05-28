#pragma once

// Module Name —————————————————————————————————————————————————————————————————————————————————————

#define ORI "SceneManager"

// Log - Causes ————————————————————————————————————————————————————————————————————————————————————

// Infos

static const char CSE_SCENE_CREATED[] = "Scene Created";
static const char CSE_SCENE_SET_TO[] = "Scene Set To";
static const char CSE_SCENE_DELETED[] = "Scene Deleted";

// Warnings

static const char CSE_SCENE_ALREADY_EXISTS[] = "Scene already exists";
static const char CSE_SCENE_NOT_FOUND[] = "Scene not found";
static const char CSE_NULL_SCENE_UPDATE_FN[] = "Scene Has Null Update";
static const char CSE_NULL_SCENE_DRAW_FN[] = "Scene Has Null Draw";

// Errors

static const char CSE_NULL_CURR_SCENE[] = "Current Scene Is Null";
static const char CSE_NO_VALID_FUNCTIONS[] = "Scene Has No Valid Functions";
static const char CSE_CANT_DEL_CURR_SCENE[] = "Cannot Delete Current Scene";
static const char CSE_CLOCK_GETTIME_FAILED[] = "Clock Gettime Failed";

// Fatals

static const char CSE_FAILED_TO_FREE_ALL_SCENES[] = "Failed to Free All Scenes";
