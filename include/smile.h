#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
// --------------------------------------------------
// Includes
// --------------------------------------------------
#include <stdlib.h>

// --------------------------------------------------
// Other defines
// --------------------------------------------------

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef union {
  char *name;
  size_t num;
} StateID;

typedef struct state {
  StateID id;
  void (*Enter)(void *args);
  void (*Update)(float dt);
  void (*Draw)();
  void (*Exit)();
} State;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void SmileInit(void);
State *SmileGetCurrState(void);
void SmileChangeState(State *state, void *args);
void SmileUpdate(float dt);
void SmileDraw(void);
void SmileShutdown(void);

#endif