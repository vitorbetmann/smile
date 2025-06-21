// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "state_two.h"
#include "smile.h"
#include "state_one.h"

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
State state_two = {.id = "state_two",
                   .enter = state_two_enter,
                   .update = state_two_update,
                   .draw = state_two_draw,
                   .exit = state_two_exit};

// --------------------------------------------------
// Functions
// --------------------------------------------------
void state_two_enter(void *args) {
  // Init whaetver.
}

void state_two_update(float dt) {
  // Update whatever.

  // Let's say that if the user presses "DOWN_ARROW"
  // they should change state, so, after they press it:
  sm_change_state(&state_one, NULL); // Pass in NULL if the next state's enter
                                     // function requires no arguments.
}
void state_two_draw(void) {
  // Draw whatever.
}

void state_two_exit(void) {
  // Reset whatever.
}
