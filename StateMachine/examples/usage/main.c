// --------------------------------------------------
// Includes
// --------------------------------------------------
#include "../../StateMachine.h"
#include "StateOne.h"
#include <raylib.h>

// --------------------------------------------------
// Variables
// --------------------------------------------------
extern State stateOne;
float dt;

// --------------------------------------------------
// Program main entry point
// --------------------------------------------------
int main(void) {
  // Init window and all...

  SM_ChangeState(&stateOne, NULL);

  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    SM_Update(dt);

    // Begin Drawing...
    SM_Draw();
    // EndDrawing...
  }

  // Unload stuff, close window, return 0...
}