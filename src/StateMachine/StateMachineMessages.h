/**
 * @file
 * @brief Message definitions for the StateMachine module.
 *
 * Defines the module name, function identifiers, causes, and consequences
 * used when logging from the StateMachine.
 *
 * @see StateMachine.c
 * @see StateMachine.h
 * @see StateMachineInternal.h
 *
 * @note Check CommonInternalMessages.h before adding new messages.
 *
 * @author Vitor Betmann
 * @date 2025-11-03
 * @version 1.0.0
 */

#ifndef SMILE_STATE_MACHINE_MESSAGES_H
#define SMILE_STATE_MACHINE_MESSAGES_H

// —————————————————————————————————————————————————————————————————————————————
// Module Name
// —————————————————————————————————————————————————————————————————————————————

#define MODULE "StateMachine"


// —————————————————————————————————————————————————————————————————————————————
// Functions Names
// —————————————————————————————————————————————————————————————————————————————

// State Functions
#define FN_CREATE_STATE "CreateState"
#define FN_STATE_EXISTS "StateExists"
#define FN_SET_STATE "SetState"
#define FN_GET_CURRENT_STATE_NAME "GetCurrentStateName"
#define FN_GET_STATE_COUNT "GetStateCount"
#define FN_DELETE_STATE "DeleteState"
// Lifecycle Functions
#define FN_UPDATE "Update"
#define FN_GET_DT "GetDt"
#define FN_DRAW "Draw"


// —————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————

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
// Fatals
#define CAUSE_FAILED_TO_FREE_ALL_STATES "Failed to Free All States"

#endif // #ifndef SMILE_STATE_MACHINE_MESSAGES_H
