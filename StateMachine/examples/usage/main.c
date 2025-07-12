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
  SM_Init(NULL);

  NewState("one", NULL, StateOneUpdate, StateOneDraw, StateOneExit);
  NewState("two", StateTwoEnter, StateTwoUpdate, StateTwoDraw, NULL);

  // Start in the state one
  SM_ChangeStateByName("one", NULL); // or SM_ChangeStateByNum(0, NULL);

  float dt;
  // Main loop
  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    SM_Update(dt);
    SM_Draw();
  }

  // Shutdown
  SM_Shutdown();
}