#ifndef STATE_ONE_H
#define STATE_ONE_H
// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../../StateMachine.h"

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void StateOneEnter(void *args);
void StateOneUpdate(float dt);
void StateOneDraw(void);
void StateOneExit(void);

// --------------------------------------------------
// Variables
// --------------------------------------------------
extern const State stateOne;

#endif