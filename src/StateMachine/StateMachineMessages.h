#ifndef STATE_MACHINE_MESSAGES_H
#define STATE_MACHINE_MESSAGES_H

// --------------------------------------------------
// INFO no arguments
// --------------------------------------------------
#define LOG_INFO_DRAW_FUNCTION_NULL "Draw function is NULL."
#define LOG_INFO_UPDATE_FUNCTION_NULL "Update function is NULL."

// --------------------------------------------------
// INFO w/ arguments
// --------------------------------------------------
#define LOG_INFO_STATE_CREATION_SUCCESSFUL "Successful state creation:"
#define LOG_INFO_STATE_CHANGE_SUCCESSFUL "Successful state change to:"

// --------------------------------------------------
// CAUSE no arguments
// --------------------------------------------------
#define LOG_CAUSE_CURRENT_STATE_NULL "Current state is NULL."

// --------------------------------------------------
// CAUSE w/ arguments
// --------------------------------------------------
#define LOG_CAUSE_ALREADY_EXISTS "State already exists: "
#define LOG_CAUSE_NO_VALID_FUNCTIONS "State has no valid functions: "
#define LOG_CAUSE_STATE_NOT_FOUND "State not found: "

// --------------------------------------------------
// CONSEQes - Public
// --------------------------------------------------
#define LOG_CONSEQ_DRAW_ABORTED "Draw aborted."
#define LOG_CONSEQ_UPDATE_ABORTED "Update aborted."
#define LOG_CONSEQ_REGISTER_STATE_ABORTED "RegisterState aborted."
#define LOG_CONSEQ_GET_CURR_STATE_NAME_ABORTED "GetCurrStateName aborted."
#define LOG_CONSEQ_CHANGE_STATE_TO_ABORTED "ChangeStateTo aborted."
#define LOG_CONSEQ_IS_STATE_REGISTERED_ABORTED "IsStateRegistered aborted."

// --------------------------------------------------
// CONSEQes - Internal
// --------------------------------------------------
#define LOG_CONSEQ_INTERNAL_GET_TRACKER_ABORTED "Internal_GetTracker aborted."
#define LOG_CONSEQ_INTERNAL_SET_CURR_STATE_ABORTED                             \
  "Internal_SetCurrState aborted."
#define LOG_CONSEQ_INTERNAL_GET_CURR_STATE_ABORTED                             \
  "Internal_GetCurrState aborted."
#define LOG_CONSEQ_INTERNAL_GET_STATE_ABORTED "Internal_GetState aborted."

// --------------------------------------------------
// CONSEQes - Tests
// --------------------------------------------------
#define LOG_CONSEQ_TEST_GET_STATE_COUNT_ABORTED                                \
  "Internal_GetStateCount aborted."

#endif