// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateMachine.h"
#include "SM_Error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------
// Data types
// --------------------------------------------------
struct State {
  StateID id;
  void (*enter)(void *args);
  void (*update)(float dt);
  void (*draw)();
  void (*exit)();
};

struct StateNode {
  const State *state;
  struct StateNode *next;
};

typedef enum {
  ARRAY,
  LINKED_LIST,
} ListType;

typedef union {
  StateNode *head;
  State *states;
} StateList;

typedef struct {
  ListType type;
  StateList list;
  const State *currState;
  unsigned int stateCount, maxStates;
} StateTracker;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void ListUnload(void);

// --------------------------------------------------
// Variables
// --------------------------------------------------
static StateTracker *tracker = NULL;

// --------------------------------------------------
// Functions
// --------------------------------------------------
void SM_Init(ConfigArgs *configArgs) {

  SM_ASSERT_TRACKER_INIT();

  tracker = malloc(sizeof(StateTracker));
  SM_REQUIRE_TRACKER_OR_RETURN_VOID();

  tracker->stateCount = 0;
  if (!configArgs) {
    tracker->type = LINKED_LIST;
    tracker->list.head = NULL;
  } else {
    tracker->maxStates = configArgs->maxStates;
    tracker->type = ARRAY;
    tracker->list.states = malloc(tracker->maxStates * sizeof(State));
    if (!tracker->list.states) {
      free(tracker);
      tracker = NULL;
      SM_MEMALLOC_FAILED_MSG();
      return;
    }
  }
  tracker->currState = NULL;
}

const State *NewState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void)) {

  State *newState;

  switch (tracker->type) {
  case ARRAY:
    SM_ASSERT_STATE_OVERLOAD();
    newState = &tracker->list.states[tracker->stateCount];
    break;

  case LINKED_LIST:
    newState = malloc(sizeof(State));
    if (!newState) {
      return NULL;
    }

    StateNode *newNode = malloc(sizeof(StateNode));
    if (!newNode) {
      free(newState);
      return NULL;
    }

    newNode->state = newState;
    newNode->next = tracker->list.head;
    tracker->list.head = newNode;
    break;
  }

  newState->id.name = strdup(name);
  newState->id.num = tracker->stateCount;
  newState->enter = enterFn;
  newState->update = updateFn;
  newState->draw = drawFn;
  newState->exit = exitFn;

  tracker->stateCount++;
  return newState;
}

void SM_ChangeState(const State *state, void *args) {
  if (tracker->currState && tracker->currState->exit) {
    tracker->currState->exit();
  }

  tracker->currState = state;

  if (tracker->currState && tracker->currState->enter) {
    tracker->currState->enter(args);
  }
}

void SM_Update(float dt) {
  if (tracker->currState && tracker->currState->update) {
    tracker->currState->update(dt);
  }
}

void SM_Draw(void) {
  if (tracker->currState && tracker->currState->draw) {
    tracker->currState->draw();
  }
}

void SM_Shutdown(void) {
  SM_REQUIRE_TRACKER_OR_RETURN_VOID();

  if (tracker->currState && tracker->currState->exit) {
    tracker->currState->exit();
  }

  tracker->currState = NULL;

  switch (tracker->type) {
  case ARRAY:
    if (!tracker->list.states) {
      return;
    }

    for (int i = 0; i < tracker->stateCount; i++) {
      free((char *)tracker->list.states[i].id.name);
    }
    free(tracker->list.states);
    break;

  case LINKED_LIST:
    ListUnload();
    break;
  }

  free(tracker);
  tracker = NULL;
}

void ListUnload(void) {
  StateNode *cursor = tracker->list.head;
  while (cursor) {
    StateNode *next = cursor->next;
    free((char *)cursor->state->id.name);
    free((State *)cursor->state);
    free(cursor);
    cursor = next;
  }
}

// Getters
const State *SM_GetCurrState(void) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();
  return tracker->currState ? tracker->currState : NULL;
}

const StateID *SM_GetCurrStateID(void) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();
  return tracker->currState ? &tracker->currState->id : NULL;
}

const State *SM_GetStateByIDName(const char *name) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();

  switch (tracker->type) {
  case ARRAY:
    // TODO improve this, I want faster lookup
    for (unsigned int i = 0; i < tracker->stateCount; i++) {
      State *temp = &tracker->list.states[i];
      if (strcmp(name, temp->id.name) == 0) {
        return temp;
      }
    }
    break;

  case LINKED_LIST:
    for (StateNode *ptr = tracker->list.head; ptr; ptr = ptr->next) {
      State *temp = (State *)ptr->state;
      if (strcmp(name, temp->id.name) == 0) {
        return temp;
      }
    }
    break;
  }

  return NULL;
}
const State *SM_GetStateByIDNum(unsigned int num) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();

  switch (tracker->type) {
  case ARRAY:
    // TODO improve this, I want faster lookup
    for (unsigned int i = 0; i < tracker->stateCount; i++) {
      State *temp = &tracker->list.states[i];
      if (temp->id.num == num) {
        return temp;
      }
    }
    break;

  case LINKED_LIST:
    for (StateNode *ptr = tracker->list.head; ptr; ptr = ptr->next) {
      State *temp = (State *)ptr->state;
      if (temp->id.num == num) {
        return temp;
      }
    }
    break;
  }

  return NULL;
}

const StateID *SM_GetStateID(const State *state) {
  SM_REQUIRE_TRACKER_OR_RETURN_NULL();
  return state ? &state->id : NULL;
}