#ifndef STATE_MACHINE_MESSAGES_H
#define STATE_MACHINE_MESSAGES_H

// -----------------------------------------------------------------------------
// MODULE NAME
// -----------------------------------------------------------------------------

#define MODULE "StateMachine"

// -----------------------------------------------------------------------------
// Enums
// -----------------------------------------------------------------------------

typedef enum {
    // State Functions
    CREATE_STATE,
    STATE_EXISTS,
    SET_STATE,
    GET_CURRENT_STATE_NAME,
    GET_STATE_COUNT,
    DELETE_STATE,
    // Lifecycle Functions
    UPDATE,
    DRAW,
} StateMachineFunctions;

typedef enum {
    // Infos
    STATE_CREATED,
    STATE_SET_TO,
    STATE_DELETED,
    // Warnings
    STATE_ALREADY_EXISTS,
    STATE_NOT_FOUND,
    UPDATE_FUNCTION_IS_NULL,
    DRAW_FUNCTION_IS_NULL,
    // Errors
    NO_VALID_FUNCTIONS,
    CANNOT_DELETE_CURRENT_STATE,
    CURRENT_STATE_IS_NULL,
} StateMachineCauses;

// -----------------------------------------------------------------------------
// String Arrays
// -----------------------------------------------------------------------------

static char *smFuncs[] = {
    // State Functions
    [CREATE_STATE] = "CreateState",
    [STATE_EXISTS] = "StateExists",
    [SET_STATE] = "SetState",
    [GET_CURRENT_STATE_NAME] = "GetCurrentStateName",
    [GET_STATE_COUNT] = "GetStateCount",
    [DELETE_STATE ] = "DeleteState",
    // Lifecycle Functions
    [UPDATE] = "Update",
    [DRAW] = "Draw",
};

static char *smCauses[] = {
    // Infos
    [STATE_CREATED] = "State Created",
    [STATE_SET_TO] = "State Set To",
    [STATE_DELETED] = "State Deleted",
    // Warnings
    [STATE_ALREADY_EXISTS] = "State already exists",
    [STATE_NOT_FOUND] = "State not found",
    // Errors
    [CURRENT_STATE_IS_NULL] = "Current state is Null",
    [NO_VALID_FUNCTIONS] = "State has no valid functions",
    [CANNOT_DELETE_CURRENT_STATE] = "Cannot Delete Current State",
    [UPDATE_FUNCTION_IS_NULL] = "Null Update function from state",
    [DRAW_FUNCTION_IS_NULL] = "Null Draw function from state",
};

#endif
