#ifndef STATE_TWO_H
#define STATE_TWO_H
// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../../StateMachine.h"

// --------------------------------------------------
// Data types
// --------------------------------------------------
typedef struct {
  int someIntData;
  char *someCharData;
} StateTwoArgs; /*
                This is the data that will be passed to
                SM_ChangeState, if its enter function
                requires any arguments.
                */

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void StateTwoEnter(void *args);
void StateTwoUpdate(float dt);
void StateTwoDraw(void);

// --------------------------------------------------
// Variables
// --------------------------------------------------
extern const State stateTwo;

#endif