// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateTwo.h"
#include "../../StateMachine.h"
#include "StateOne.h"
#include <stdlib.h>

// --------------------------------------------------
// Variables
// --------------------------------------------------

int someIntData;
char *someCharData;
// --------------------------------------------------
// Functions
// --------------------------------------------------
void StateTwoEnter(void *args) {
  const StateTwoArgs *myArgs = (const StateTwoArgs *)args;
  someIntData = myArgs->someIntData;
  someCharData = myArgs->someCharData;
}

void StateTwoUpdate(float dt) {
  // Update whatever.

  // Let's say that if the user presses "BACKSPACE" they should change state,
  // so, after they press it:
  SM_ChangeState("one", NULL); /* Pass in NULL if the next state's enter
                                  function requires no arguments.*/
}
void StateTwoDraw(void) {
  // Draw whatever.
}
