#ifndef STATE_MACHINE_MESSAGES_H
#define STATE_MACHINE_MESSAGES_H

// --------------------------------------------------
// INFO no arguments
// --------------------------------------------------
#define INFO_DRAW_FUNCTION_NULL "Draw function is NULL."
#define INFO_UPDATE_FUNCTION_NULL "Update function is NULL."

// --------------------------------------------------
// INFO w/ arguments
// --------------------------------------------------
#define INFO_STATE_CREATION_SUCCESSFUL "Successful state creation:"
#define INFO_STATE_CHANGE_SUCCESSFUL "Successful state change to:"

// --------------------------------------------------
// CAUSE no arguments
// --------------------------------------------------

#define CAUSE_CURRENT_STATE_NULL "Current state is NULL."
#define CAUSE_STATE_NAME_NULL "State name is NULL."
#define CAUSE_STATE_NAME_EMPTY "State name is empty."


// --------------------------------------------------
// CAUSE w/ arguments
// --------------------------------------------------
#define CAUSE_ALREADY_EXISTS "State already exists: "
#define CAUSE_NO_VALID_FUNCTIONS "State has no valid functions: "
#define CAUSE_STATE_NOT_FOUND "State not found: "

// --------------------------------------------------
// CONSEQes - Public
// --------------------------------------------------
#define CONSEQ_DRAW_ABORTED "Draw aborted."
#define CONSEQ_UPDATE_ABORTED "Update aborted."
#define CONSEQ_REGISTER_STATE_ABORTED "RegisterState aborted."
#define CONSEQ_GET_CURR_STATE_NAME_ABORTED "GetCurrStateName aborted."
#define CONSEQ_CHANGE_STATE_TO_ABORTED "ChangeStateTo aborted."
#define CONSEQ_IS_STATE_REGISTERED_ABORTED "IsStateRegistered aborted."

// --------------------------------------------------
// CONSEQes - Internal
// --------------------------------------------------
#define CONSEQ_INTERNAL_GET_TRACKER_ABORTED "Internal_GetTracker aborted."
#define CONSEQ_INTERNAL_SET_CURR_STATE_ABORTED "Internal_SetCurrState aborted."
#define CONSEQ_INTERNAL_GET_CURR_STATE_ABORTED "Internal_GetCurrState aborted."
#define CONSEQ_INTERNAL_GET_STATE_ABORTED "Internal_GetState aborted."

// --------------------------------------------------
// CONSEQes - Tests
// --------------------------------------------------
#define CONSEQ_TEST_GET_STATE_COUNT_ABORTED "Internal_GetStateCount aborted."

#endif
