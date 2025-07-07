// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "state_example.h"
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
State state_example = {.id = "example",
                       .enter = state_example_enter,
                       .update = state_example_update,
                       .draw = state_example_draw,
                       .exit = state_example_exit};

// --------------------------------------------------
// Functions
// --------------------------------------------------
void state_example_enter(void *args) {
  // TODO
}

void state_example_update(float dt) {
  // TODO
}

void state_example_draw(void) {
  // TODO
}

void state_example_exit(void) {
  // TODO
}
