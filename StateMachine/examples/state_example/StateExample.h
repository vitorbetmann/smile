#ifndef STATE_EXAMPLE_H
#define STATE_EXAMPLE_H
// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../../StateMachine.h"

// --------------------------------------------------
// Other defines
// --------------------------------------------------

// --------------------------------------------------
// Data types
// --------------------------------------------------

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void StateExampleEnter(void *args);
void StateExampleUpdate(float dt);
void StateExampleDraw(void);
void StateExampleExit(void);

// --------------------------------------------------
// Variables
// --------------------------------------------------
extern const State stateExample;

#endif