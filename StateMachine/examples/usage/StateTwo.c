// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateTwo.h"
#include "../../StateMachine.h"
#include "StateOne.h"

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
State stateTwo = {.id = "two",
                  .enter = StateTwoEnter,
                  .update = StateTwoUpdate,
                  .draw = StateTwoDraw,
                  .exit = StateTwoExit};

int someIntData;
char *someCharData;
// --------------------------------------------------
// Functions
// --------------------------------------------------
void StateTwoEnter(void *args) {
  StateTwoArgs *myArgs = (StateTwoArgs *)args;
  someIntData = myArgs->someIntData;
  someCharData = myArgs->someCharData;
}

void StateTwoUpdate(float dt) {
  // Update whatever.

  // Let's say that if the user presses "DOWN_ARROW"
  // they should change state, so, after they press it:
  SM_ChangeState(&stateOne, NULL); // Pass in NULL if the next state's enter
                                   // function requires no arguments.
}
void StateTwoDraw(void) {
  // Draw whatever.
}

void StateTwoExit(void) {
  // Reset whatever.
}
