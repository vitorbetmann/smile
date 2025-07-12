// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../../StateMachine.h"
#include "StateOne.h"
#include "StateTwo.h"
#include <raylib.h>
#include <stdlib.h>

// --------------------------------------------------
// Program main entry point
// --------------------------------------------------
int main(void) {
  // Engine setup
  SM_Init();

  NewState("one", NULL, StateOneUpdate, StateOneDraw, StateOneExit);
  NewState("two", StateTwoEnter, StateTwoUpdate, StateTwoDraw, NULL);

  // Start in the state one
  SM_ChangeState("one", NULL);

  float dt;
  // Main loop
  while (true) {
    // dt = some delta time
    SM_Update(dt);
    SM_Draw();
  }

  // Shutdown
  SM_Shutdown();
}