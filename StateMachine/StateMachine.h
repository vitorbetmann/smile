#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// --------------------------------------------------
// Data types
// --------------------------------------------------
typedef struct State State;
typedef struct StateNode StateNode;

typedef struct {
  const char *name;
  unsigned int num;
} StateID;

typedef struct {
  unsigned int maxStates;
} ConfigArgs;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void SM_Init(ConfigArgs *configArgs);

const State *NewState(const char *name, void (*enterFn)(void *),
                      void (*updateFn)(float), void (*drawFn)(void),
                      void (*exitFn)(void));

void SM_ChangeState(const State *state, void *args);

void SM_Update(float dt);
void SM_Draw(void);

void SM_Shutdown(void);

// Getters
const State *SM_GetCurrState(void);
const StateID *SM_GetCurrStateID(void);

const State *SM_GetStateByIDName(const char *name);
const State *SM_GetStateByIDNum(unsigned int num);
const StateID *SM_GetStateID(const State *state);

#endif