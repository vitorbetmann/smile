// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateOne.h"
#include "../../StateMachine.h"
#include "StateTwo.h" /*  Make sure you include the header
                          of the state(s) to transition to.
                          */

// --------------------------------------------------
// Functions
// --------------------------------------------------
void StateOneUpdate(float dt) {
  // Update whatever.

  // Let's say that if the user presses "SPACE" they should change state, so,
  // after they press it:
  StateTwoArgs args = {.someIntData = 0,
                       .someCharData = "going into state two"};
  SM_ChangeState("two", &args); /* Pass in NULL if the next state's enter
                                   function requires no arguments.*/
}

void StateOneDraw(void) {
  // Draw whatever.
}

void StateOneExit(void) {
  // Exit routine
}
