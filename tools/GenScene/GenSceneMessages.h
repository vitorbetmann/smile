/**
 * @file
 * @brief Message definitions for the GenScene tool.
 *
 * @see GenScene.c
 *
 * @author Vitor Betmann
 */


#ifndef SMILE_GEN_SCENE_MESSAGES_H
#define SMILE_GEN_SCENE_MESSAGES_H


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Tool Name
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define ORIGIN "GenScene"


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Infos
#define CAUSE_FILES_CREATED "Source And Header Files Created"
#define CAUSE_DIR_CREATED "Directory Created"
// Warnings
// Errors
#define CAUSE_FILE_ALREADY_EXISTS "File Already Exists"
#define CAUSE_DIR_DOES_NOT_EXIST "Directory Does Not Exist"
#define CAUSE_FLAG_REQUIRES_PATH_ARG "Flag Requires Path Argument"
// Fatals


// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
// Consequences
// —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

// Stop
#define CONSEQ_STOPPED "Stopped"

#endif
