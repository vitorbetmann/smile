#ifndef SMILE_H
#define SMILE_H
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
typedef struct {
  const char *id;
  void (*enter)(void *args);
  void (*update)(float dt);
  void (*draw)();
  void (*exit)();
} State;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Initializes the SMILE engine.
 * @details Sets current state to NULL.
 *
 * @return void
 *
 * @warning Call only once. Risk of memory leak.
 *
 * @author Vitor Betmann
 **/
void SM_Init(void);

/**
 * @brief Gets the current state the game is in.
 *
 * @return A pointer to the current state, which can be NULL if no state active.
 *
 * @author Vitor Betmann
 **/
State *SM_GetCurrState(void);

/**
 * @brief Changes from current state to a new one.
 * @details Calls Exit() on the old state and Enter(&args) on the new one.
 *Does nothing if any of the states or pointers to the functions are NULL.
 *
 * @param[in] state A pointer to the next state.
 * @param[in] args Optional argument. It can be NULL or a pointer to data
 *(usually a struct) that Enter(&args) of the new state might need.
 *
 * @return void
 *
 * @author Vitor Betmann
 **/
void SM_ChangeState(State *state, void *args);

/**
 * @brief Calls Update(dt) on the current state.
 * @details Does nothing if either the current state or the pointer to Update is
 * NULL.
 *
 * @param[in] dt The elapsed time between the current and previous frame.
 *
 * @return void
 *
 * @author Vitor Betmann
 **/
void SM_Update(float dt);

/**
 * @brief Calls Draw() on the current state.
 *
 * Does nothing if either the current state or the pointer to Draw is NULL.\
 *
 * @return void
 *
 * @author Vitor Betmann
 **/
void SM_Draw(void);

/**
 * @brief Safely ends the State Machine.
 *
 * @details Calls Exit() on the current state if neither the current state nor
 *the pointer to Exit are NULL. Also sets current state to NULL.
 *
 * @return void
 *
 * @author Vitor Betmann
 **/
void SM_Shutdown(void);

#endif