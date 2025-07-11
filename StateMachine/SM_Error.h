#ifndef SM_ERROR_H
#define SM_ERROR_H
// --------------------------------------------------
// Otehr defines
// --------------------------------------------------
// Error msgs
#define SM_NO_INIT_CALLED_MSG "[SMILE ERROR] State Machine not initialized.\n"
#define SM_MEMALLOC_FAILED "[SMILE ERROR] Failed to allocate memory\n"
#define SM_MAX_STATES_REACHED                                                  \
  "[SMILE ERROR] Maximum number of states reached. Failed to create new "      \
  "State.\n"
// Warning msgs
#define SM_ALREADY_INIT_MSG                                                    \
  "[SMILE WARNING] State Machine already initialized.\n"

// Error functions
#define SM_REQUIRE_TRACKER_OR_RETURN_VOID()                                    \
  if (!tracker) {                                                              \
    fprintf(stderr, SM_NO_INIT_CALLED_MSG);                                    \
    return;                                                                    \
  }

#define SM_REQUIRE_TRACKER_OR_RETURN_NULL()                                    \
  if (!tracker) {                                                              \
    fprintf(stderr, SM_NO_INIT_CALLED_MSG);                                    \
    return NULL;                                                               \
  }

#define SM_MEMALLOC_FAILED_MSG() fprintf(stderr, SM_MEMALLOC_FAILED);

#define SM_ASSERT_STATE_OVERLOAD()                                             \
  if (tracker->stateCount >= tracker->maxStates) {                             \
    fprintf(stderr, SM_MAX_STATES_REACHED);                                    \
    return NULL;                                                               \
  }

#define SM_ASSERT_TRACKER_INIT()                                               \
  if (tracker) {                                                               \
    fprintf(stderr, SM_ALREADY_INIT_MSG);                                      \
    return;                                                                    \
  }

#endif