// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../include/smile.h"

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
static State *currState;

// --------------------------------------------------
// Functions
// --------------------------------------------------
void SmileInit(void) { currState = NULL; }

State *SmileGetCurrState(void) { return currState; }

void SmileChangeState(State *state, void *args) {
  if (currState && currState->Exit) {
    currState->Exit();
  }

  currState = state;

  if (currState && currState->Enter) {
    currState->Enter(args);
  }
}

void SmileUpdate(float dt) {
  if (currState && currState->Update) {
    currState->Update(dt);
  }
}

void SmileDraw(void) {
  if (currState && currState->Draw) {
    currState->Draw();
  }
}

void SmileShutdown(void) {
  if (currState && currState->Exit) {
    currState->Exit();
  }
  currState = NULL;
}
