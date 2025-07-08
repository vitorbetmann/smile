// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "StateExample.h"
#include "../../StateMachine.h"

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
const State stateExample = {.id = "example",
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
