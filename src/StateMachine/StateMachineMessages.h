#ifndef STATE_MACHINE_MESSAGES_H
#define STATE_MACHINE_MESSAGES_H

// --------------------------------------------------
// MODULE NAME
// --------------------------------------------------

#define MODULE "StateMachine"

// --------------------------------------------------
// FUNCTIONS NAMES
// --------------------------------------------------

#define REGISTER_STATE "RegisterState"
#define IS_STATE_REGISTERED "IsStateRegistered"
#define CHANGE_STATE_TO "ChangeStateTo"
#define GET_CURR_STATE_NAME "GetCurrStateName"
#define GET_STATE_COUNT "GetStateCount"

#define UPDATE "Update"
#define DRAW "Draw"

// --------------------------------------------------
// INFO W/ ARGS
// --------------------------------------------------

#define INFO_WITH_ARGS_CHANGED_STATE_TO "Changed state to:"

// --------------------------------------------------
// CAUSE NO ARGS
// --------------------------------------------------

#define CAUSE_NULL_CURRENT_STATE "Current state is Null."

// --------------------------------------------------
// CAUSE W/ ARGS
// --------------------------------------------------

#define CAUSE_WITH_ARGS_ALREADY_EXISTS "State already exists:"
#define CAUSE_WITH_ARGS_NO_VALID_FUNCTIONS "State has no valid functions:"
#define CAUSE_WITH_ARGS_STATE_NOT_FOUND "State not found:"

#define CAUSE_WITH_ARGS_NULL_UPDATE_FUNCTION "Null Update function from state:"
#define CAUSE_WITH_ARGS_NULL_DRAW_FUNCTION "Null Draw function from state:"

#endif
