#ifndef STATE_MACHINE_MESSAGES_H
#define STATE_MACHINE_MESSAGES_H

// --------------------------------------------------
// Module Name
// --------------------------------------------------

#define MODULE "StateMachine"

// --------------------------------------------------
// Function Names
// --------------------------------------------------

#define REGISTER_STATE "RegisterState"
#define IS_STATE_REGISTERED "IsStateRegistered"
#define CHANGE_STATE_TO "ChangeStateTo"
#define GET_CURR_STATE_NAME "GetCurrStateName"

#define UPDATE "Update"
#define DRAW "Draw"

// --------------------------------------------------
// INFO w/ args
// --------------------------------------------------

#define INFO_WITH_ARGS_CHANGED_STATE_TO "Changed state to:"

// --------------------------------------------------
// CAUSE no args
// --------------------------------------------------

#define CAUSE_NULL_CURRENT_STATE "Current state is Null."

// --------------------------------------------------
// CAUSE w/ args
// --------------------------------------------------

#define CAUSE_WITH_ARGS_ALREADY_EXISTS "State already exists:"
#define CAUSE_WITH_ARGS_NO_VALID_FUNCTIONS "State has no valid functions:"
#define CAUSE_WITH_ARGS_STATE_NOT_FOUND "State not found:"

#define CAUSE_WITH_ARGS_NULL_UPDATE_FUNCTION "Null Update function from state:"
#define CAUSE_WITH_ARGS_NULL_DRAW_FUNCTION "Null Draw function from state:"

#endif
