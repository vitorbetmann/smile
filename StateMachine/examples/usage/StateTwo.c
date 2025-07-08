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
const State stateTwo = {
    .id = "two",
    .enter = StateTwoEnter,
    .update = StateTwoUpdate,
    .draw = StateTwoDraw,
    .exit = NULL /* You can set a function pointer to NULL if no need to do
                    anything inside of it. No need to declare it in the header
                    file either.
                    */
};

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

  // Let's say that if the user presses "BACKSPACE"
  // they should change state, so, after they press it:
  SM_ChangeState(&stateOne,
                 NULL); /* Pass in NULL if the next state's enter
                                       function requires no arguments.
                                       */
}
void StateTwoDraw(void) {
  // Draw whatever.
}
