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

  SM_RegisterState("one", NULL, StateOneUpdate, StateOneDraw, StateOneExit);
  SM_RegisterState("two", StateTwoEnter, StateTwoUpdate, StateTwoDraw, NULL);

  // Start in the state one
  SM_ChangeStateTo("one", NULL);

  float dt = 0.016f; // Mock delta time, about 60 FPS
  while (true) {
    SM_Update(dt);
    SM_Draw();
  }

  // Shutdown
  SM_Shutdown();
}