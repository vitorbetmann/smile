// --------------------------------------------------
// Includes
// --------------------------------------------------

#include "StateMachine.h"
#include "../_Internals/Log/LogInternal.h"
#include "../_Internals/Log/LogMessages.h"
#include "../tests/StateMachine/StateMachineTest.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"
#include <ctype.h>  // For isspace
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h> // For malloc, free
#include <string.h> // For strlen, strncpy

// --------------------------------------------------
// Defines
// --------------------------------------------------

#define MODULE_NAME "StateMachine"

// Helper macro to check initialization and return (with error log)
#define RETURN_FALSE_IF_NOT_INITIALIZED(conseq)                                \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED, conseq);               \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define RETURN_NULL_IF_NOT_INITIALIZED(conseq)                                 \
  do {                                                                         \
    if (!tracker) {                                                            \
      SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED, conseq);               \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

// --------------------------------------------------
// Variables
// --------------------------------------------------
static int stateCount;
static StateTracker *tracker;
static bool canMalloc = true;

// --------------------------------------------------
// Functions - Internal Utilities
// --------------------------------------------------

/**
 * @brief Trims leading and trailing whitespace characters from a string.
 *
 * This function creates a new string by removing any leading or trailing
 * whitespace characters (space, tab, newline, carriage return) from the
 * input `src` string.
 *
 * @param src The source string to trim.
 * @return A newly allocated string with leading and trailing whitespace removed.
 *         Returns `NULL` if `src` is `NULL` or if memory allocation fails.
 *         The caller is responsible for freeing the returned string using `free()`.
 *         If `src` contains only whitespace characters or is an empty string,
 *         an empty string `""` will be returned.
 */
char *smInternalTrimWhitespace(const char *src) {
  if (src == NULL) {
    return NULL;
  }

  // Find start of non-whitespace
  const char *start = src;
  while (*start != '\0' && isspace((unsigned char)*start)) {
    start++;
  }

  // Find end of non-whitespace
  const char *end = src + strlen(src) - 1;
  while (end >= start && isspace((unsigned char)*end)) {
    end--;
  }

  // Calculate the length of the trimmed string
  size_t trimmed_len = (end < start) ? 0 : (size_t)(end - start + 1);

  // Allocate memory for the new string
  char *trimmed_str = malloc(trimmed_len + 1);
  if (!trimmed_str) {
    // Log an error if memory allocation fails
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_GENERAL_FAILURE);
    return NULL;
  }

  // Copy the trimmed content and null-terminate
  if (trimmed_len > 0) {
    strncpy(trimmed_str, start, trimmed_len);
  }
  trimmed_str[trimmed_len] = '\0';

  return trimmed_str;
}

// --------------------------------------------------
// Functions
// --------------------------------------------------

bool SM_Init(void) {

  if (tracker) {
    SMILE_WARN(MODULE_NAME, LOG_CAUSE_ALREADY_INITIALIZED,
               LOG_CONSEQ_INIT_ABORTED);
    return false;
  }

  tracker = SM_Test_Malloc(sizeof(StateTracker));
  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_INIT_ABORTED);
    return false;
  }

  tracker->stateMap = NULL;
  tracker->currState = NULL;
  stateCount = 0;

  SMILE_INFO(MODULE_NAME, LOG_INFO_INIT_SUCCESSFUL);

  return true;
}

bool SM_IsInitialized(void) { return tracker; }

bool SM_RegisterState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void)) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_REGISTER_STATE_ABORTED);

  if (!name) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NULL_NAME,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  char *trimmed_name = smInternalTrimWhitespace(name);
  if (!trimmed_name) { // Memory allocation failed during trim
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  // Use trimmed_name for all subsequent validation and storage
  if (strlen(trimmed_name) == 0) {
    free(trimmed_name); // Free the trimmed string before returning
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_EMPTY_NAME,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  if (SM_IsStateRegistered(trimmed_name)) { // Use trimmed_name for existence check
    free(trimmed_name); // Free the trimmed string before returning
    SMILE_ERR_WITH_NAME(MODULE_NAME, LOG_CAUSE_ALREADY_EXISTS, trimmed_name, // Use trimmed_name in log
                        LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  if (!enterFn && !updateFn && !drawFn && !exitFn) {
    free(trimmed_name); // Free the trimmed string before returning
    SMILE_ERR_WITH_NAME(MODULE_NAME, LOG_CAUSE_NO_VALID_FUNCTIONS, trimmed_name, // Use trimmed_name in log
                        LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  State *newState = malloc(sizeof(State));
  if (!newState) {
    free(trimmed_name); // Free the trimmed string before returning
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }

  // Allocate memory for the state name based on the trimmed length
  char *stateName = malloc(strlen(trimmed_name) + 1);
  if (!stateName) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    free(newState);
    free(trimmed_name); // Free the trimmed string before returning
    return false;
  }
  strcpy(stateName, trimmed_name); // Copy the trimmed name

  newState->name = stateName;
  newState->enter = enterFn;
  newState->update = updateFn;
  newState->draw = drawFn;
  newState->exit = exitFn;

  StateMap *temp = malloc(sizeof(StateMap));
  if (!temp) {
    free((char *)newState->name); // Free the newly allocated stateName
    free(newState);
    free(trimmed_name); // Free the trimmed string before returning
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED,
              LOG_CONSEQ_REGISTER_STATE_ABORTED);
    return false;
  }
  temp->state = newState;
  temp->name = newState->name;
  HASH_ADD_STR(tracker->stateMap, name, temp);

  stateCount++;

  SMILE_INFO_FMT(MODULE_NAME, "%s '%s'. Total states: %d.",
                 LOG_INFO_STATE_CREATION_SUCCESSFUL, trimmed_name, stateCount); // Use trimmed_name in log

  free(trimmed_name); // Free the temporary trimmed string
  return true;
}

bool SM_IsStateRegistered(const char *name) {
  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_IS_STATE_REGISTERED_ABORTED);

  if (!name) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NULL_NAME, LOG_CONSEQ_IS_STATE_REGISTERED_ABORTED);
    return false;
  }

  char *trimmed_name = smInternalTrimWhitespace(name);
  if (!trimmed_name) { // Memory allocation failed during trim
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_IS_STATE_REGISTERED_ABORTED);
    return false;
  }

  if (strlen(trimmed_name) == 0) { // Check for empty string after trim
    free(trimmed_name);
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_EMPTY_NAME, LOG_CONSEQ_IS_STATE_REGISTERED_ABORTED);
    return false;
  }

  StateMap *entry;
  HASH_FIND_STR(tracker->stateMap, trimmed_name, entry); // Use trimmed_name for lookup
  
  bool found = (entry != NULL);
  free(trimmed_name); // Free the temporary trimmed string
  return found;
}

bool SM_ChangeStateTo(const char *name, void *args) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);

  if (!name) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NULL_NAME,
              LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);
    return false;
  }

  char *trimmed_name = smInternalTrimWhitespace(name);
  if (!trimmed_name) { // Memory allocation failed during trim
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_MEM_ALLOC_FAILED, LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);
    return false;
  }

  // Use trimmed_name for all subsequent validation and lookup
  if (strlen(trimmed_name) == 0) {
    free(trimmed_name); // Free the trimmed string before returning
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_EMPTY_NAME,
              LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);
    return false;
  }

  State *nextState = (State *)SM_Internal_GetState(trimmed_name); // Use trimmed_name for lookup
  if (!nextState) {
    free(trimmed_name); // Free the trimmed string before returning
    SMILE_WARN_WITH_NAME(MODULE_NAME, LOG_CAUSE_STATE_NOT_FOUND, trimmed_name, // Use trimmed_name in log
                         LOG_CONSEQ_CHANGE_STATE_TO_ABORTED);
    return false;
  }

  State *currState = (State *)SM_Internal_GetCurrState();
  if (currState && currState->exit) {
    currState->exit();
  }

  SM_Internal_SetCurrState(nextState);

  currState = (State *)SM_Internal_GetCurrState();
  if (currState && currState->enter) {
    currState->enter(args);
  }

  SMILE_INFO_WITH_NAME(MODULE_NAME, LOG_INFO_STATE_CHANGE_SUCCESSFUL, trimmed_name); // Use trimmed_name in log

  free(trimmed_name); // Free the temporary trimmed string
  return true;
}

bool SM_Update(float dt) {
  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_UPDATE_ABORTED);

  State *currState = (State *)SM_Internal_GetCurrState();

  if (!currState) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_CURRENT_STATE_NULL,
              LOG_CONSEQ_UPDATE_ABORTED);
    return false;
  }

  if (!currState->update) {
    SMILE_WARN_WITH_NAME(MODULE_NAME, LOG_INFO_UPDATE_FUNCTION_NULL,
                         currState->name, LOG_CONSEQ_UPDATE_ABORTED);
    return false;
  }

  currState->update(dt);
  return true;
}

bool SM_Draw(void) {
  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_DRAW_ABORTED);

  State *currState = (State *)SM_Internal_GetCurrState();

  if (!currState) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_CURRENT_STATE_NULL,
              LOG_CONSEQ_DRAW_ABORTED);
    return false;
  }

  if (!currState->draw) {
    SMILE_WARN_WITH_NAME(MODULE_NAME, LOG_INFO_DRAW_FUNCTION_NULL,
                         currState->name, LOG_CONSEQ_DRAW_ABORTED);
    return false;
  }

  currState->draw();
  return true;
}

bool SM_Shutdown(void) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_SHUTDOWN_ABORTED);

  State *currState = (State *)SM_Internal_GetCurrState();
  if (currState && currState->exit) {
    currState->exit();
  }
  SM_Internal_SetCurrState(NULL);

  StateMap *el, *tmp;
  HASH_ITER(hh, tracker->stateMap, el, tmp) {
    HASH_DEL(tracker->stateMap, el);
    free((char *)el->state->name);
    free(el->state);
    free(el);
    stateCount--;
  }

  free(tracker);
  tracker = NULL;

  SMILE_INFO(MODULE_NAME, LOG_INFO_SHUTDOWN_SUCCESSFUL);

  return true;
}

const char *SM_GetCurrStateName(void) {

  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED,
              LOG_CONSEQ_GET_CURR_STATE_NAME_ABORTED);
    return NULL;
  }

  return tracker->currState ? tracker->currState->name : NULL;
}

// --------------------------------------------------
// Functions - Internal
// --------------------------------------------------

bool SM_Internal_SetCurrState(const State *state) {

  RETURN_FALSE_IF_NOT_INITIALIZED(LOG_CONSEQ_INTERNAL_SET_CURR_STATE_ABORTED);

  tracker->currState = state;
  return true;
}

const State *SM_Internal_GetCurrState(void) {

  if (!tracker) {
    SMILE_ERR(MODULE_NAME, LOG_CAUSE_NOT_INITIALIZED,
              LOG_CONSEQ_INTERNAL_GET_CURR_STATE_ABORTED);
    return NULL;
  }

  return tracker->currState;
}

const State *SM_Internal_GetState(const char *name) {

  RETURN_NULL_IF_NOT_INITIALIZED(LOG_CONSEQ_INTERNAL_GET_STATE_ABORTED);

  // Note: SM_Internal_GetState is an internal function used by SM_ChangeStateTo.
  // SM_ChangeStateTo already performs trimming on the input 'name' before calling
  // this function. Therefore, 'name' is expected to be already trimmed here.
  StateMap *sm;
  HASH_FIND_STR(tracker->stateMap, name, sm);
  return sm ? sm->state : NULL;
}

// --------------------------------------------------
// Functions - Tests
// --------------------------------------------------

const StateTracker *SM_Test_GetTracker(void) { return tracker; }

const int SM_Test_GetStateCount(void) { return stateCount; }

bool SM_Test_SetCanMalloc(bool toggle) {
  canMalloc = toggle;
  return toggle;
}

void *SM_Test_Malloc(size_t size) {
  if (!canMalloc) {
    return NULL;
  }
  return malloc(size);
}