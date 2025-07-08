// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateMachine.h"
#include <string.h>

// --------------------------------------------------
// Variables
// --------------------------------------------------
static const State *currState;

// --------------------------------------------------
// Functions
// --------------------------------------------------
void SM_Init(void) { currState = NULL; }

const State *SM_GetCurrState(void) { return currState; }

void SM_ChangeState(const State *state, void *args) {
  if (currState && currState->exit) {
    currState->exit();
  }

  currState = state;

  if (currState && currState->enter) {
    currState->enter(args);
  }
}

void SM_Update(float dt) {
  if (currState && currState->update) {
    currState->update(dt);
  }
}

void SM_Draw(void) {
  if (currState && currState->draw) {
    currState->draw();
  }
}

void SM_Shutdown(void) {
  if (currState && currState->exit) {
    currState->exit();
  }
  currState = NULL;
}
