// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "state_one.h"
#include "state_two.h" /*
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
State state_one = {.id = "state_one",
                   .enter = state_one_enter,
                   .update = state_one_update,
                   .draw = state_one_draw,
                   .exit = state_one_exit};

static StateTwoArgs *state_two_args;

// --------------------------------------------------
// Functions
// --------------------------------------------------
void state_one_enter(void *args) {
  // Init whatever.
}

void state_one_update(float dt) {
  // Update whatever.

  // Let's say that if the user presses "UP_ARROW"
  // they should change state, so, after they press it:
  sm_change_state(&state_two,
                  state_two_args); /*
                                        Pass in NULL if the next state's
                                        enter function requires no arguments.
                                        */
}

void state_one_draw(void) {
  // Draw whatever.
}

void state_one_exit(void) {
  state_two_args->some_int_data = 0;
  state_two_args->some_char_data = "going into state two";
}
