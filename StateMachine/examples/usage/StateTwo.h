#ifndef STATE_TWO_H
#define STATE_TWO_H
// --------------------------------------------------
// Data types
// --------------------------------------------------
typedef struct {
  int someIntData;
  char *someCharData;
} StateTwoArgs; /*
                This is the data that will be passed to
                SM_ChangeStateTo, if its enter function
                requires any arguments.
                */

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void StateTwoEnter(void *args);
void StateTwoUpdate(float dt);
void StateTwoDraw(void);

#endif