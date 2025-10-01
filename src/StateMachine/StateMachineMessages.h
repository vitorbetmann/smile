#ifndef STATE_MACHINE_MESSAGES_H
#define STATE_MACHINE_MESSAGES_H

// -----------------------------------------------------------------------------
// Module Name
// -----------------------------------------------------------------------------

#define MODULE "StateMachine"

// -----------------------------------------------------------------------------
// Functions Names
// -----------------------------------------------------------------------------

// State Functions
#define FN_CREATE_STATE "CreateState"
#define FN_STATE_EXISTS "StateExists"
#define FN_SET_STATE "SetState"
#define FN_GET_CURRENT_STATE_NAME "GetCurrentStateName"
#define FN_GET_STATE_COUNT "GetStateCount"
#define FN_DELETE_STATE "DeleteState"
// Lifecycle Functions
#define FN_UPDATE "Update"
#define FN_DRAW "Draw"

// -----------------------------------------------------------------------------
// Causes
// -----------------------------------------------------------------------------

// Infos
#define CAUSE_STATE_CREATED "State Created"
#define CAUSE_STATE_SET_TO "State Set To"
#define CAUSE_STATE_DELETED "State Deleted"
// Warnings
#define CAUSE_STATE_ALREADY_EXISTS "State already exists"
#define CAUSE_STATE_NOT_FOUND "State not found"
#define CAUSE_NULL_STATE_UPDATE_FN "State Has Null Update"
#define CAUSE_NULL_STATE_DRAW_FN "State Has Null Draw"
// Errors
#define CAUSE_NULL_CURR_STATE "Current State Is Null"
#define CAUSE_NO_VALID_FUNCTIONS "State Has No Valid Functions"
#define CAUSE_CANNOT_DELETE_CURR_STATE "Cannot Delete Current State"

#endif
