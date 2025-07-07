// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../include/Smile.h"
#include <string.h>

// --------------------------------------------------
// Defines
// --------------------------------------------------

// --------------------------------------------------
// Data types
// --------------------------------------------------

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

// --------------------------------------------------
// Variables
// --------------------------------------------------
static State *curr_state;

// --------------------------------------------------
// Functions
// --------------------------------------------------
void SM_Init(void) { curr_state = NULL; }

State *SM_GetCurrState(void) { return curr_state; }

void SM_ChangeState(State *state, void *args) {
  if (curr_state && curr_state->exit) {
    curr_state->exit();
  }

  curr_state = state;

  if (curr_state && curr_state->enter) {
    curr_state->enter(args);
  }
}

void SM_Update(float dt) {
  if (curr_state && curr_state->update) {
    curr_state->update(dt);
  }
}

void SM_Draw(void) {
  if (curr_state && curr_state->draw) {
    curr_state->draw();
  }
}

void SM_Shutdown(void) {
  if (curr_state && curr_state->exit) {
    curr_state->exit();
  }
  curr_state = NULL;
}
