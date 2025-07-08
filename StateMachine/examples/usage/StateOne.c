// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateOne.h"
#include "StateTwo.h" /*
                        Make sure you include the header 
                        of the state(s) to transition to.
                        */

// --------------------------------------------------
// Defines
// --------------------------------------------------

// --------------------------------------------------
// Data types
// --------------------------------------------------
typedef struct {
  int some_int_data;
  char *some_char_data;
} StateTwoArgs; /*
                This is the data that will be passed to
                sm_change_state, if its enter function
                requires any arguments.
                */

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

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
}

void StateOneDraw(void) {
  // Draw whatever.
}

void StateOneExit(void) {
  stateTwoArgs->some_int_data = 0;
  stateTwoArgs->some_char_data = "going into state two";
}
