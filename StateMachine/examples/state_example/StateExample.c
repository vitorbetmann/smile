// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../../StateMachine.h"
#include "StateExample.h"

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
                       .enter = StateExampleEnter,
                       .update = StateExampleUpdate,
                       .draw = StateExampleDraw,
                       .exit = StateExampleExit};

// --------------------------------------------------
// Functions
// --------------------------------------------------
void StateExampleEnter(void *args) {
  // TODO
}

void StateExampleUpdate(float dt) {
  // TODO
}

void StateExampleDraw(void) {
  // TODO
}

void StateExampleExit(void) {
  // TODO
}
