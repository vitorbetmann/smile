// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateOne.h"
#include "StateTwo.h" /*
                        Make sure you include the header 
                        of the state(s) to transition to.
                        */

// --------------------------------------------------
// Variables
// --------------------------------------------------
State stateOne = {.id = "one",
                  .enter = StateOneEnter,
                  .update = StateOneUpdate,
                  .draw = StateOneDraw,
                  .exit = StateOneExit};

static StateTwoArgs *stateTwoArgs;

// --------------------------------------------------
// Functions
// --------------------------------------------------
void StateOneEnter(void *args) {
  // Init whatever.
  stateTwoArgs = malloc(sizeof(StateTwoArgs));
}

void StateOneUpdate(float dt) {
  // Update whatever.

  // Let's say that if the user presses "UP_ARROW"
  // they should change state, so, after they press it:
  SM_ChangeState(&stateTwo,
                 stateTwoArgs); /*
                                       Pass in NULL if the next state's
                                       enter function requires no arguments.
                                       */
  free(stateTwoArgs);
}

void StateOneDraw(void) {
  // Draw whatever.
}

void StateOneExit(void) {
  stateTwoArgs->someIntData = 0;
  stateTwoArgs->someCharData = "going into state two";
}
