## Welcome to the StateMachine Module 🤖

The StateMachine module helps you organize your game's flow using modular state transitions.

It lets you easily swap between different states like menus, gameplay, game over screens, and more, using a clean and well-defined state lifecycle.

For function documentation, see:

---

## 🧩 How to Use the State Machine pt. 1

The typical lifecycle of a state in the StateMachine module follows these steps: enter → update → draw → exit.

Users define states as `State` structs that contain function pointers for these lifecycle methods. To switch between states, you register transitions using the `SM_ChangeState` function.

Call `SM_ChangeState` to trigger a transition. During the game loop, the update and draw functions of the active state are called each frame.

```C
// First use example

#include "StateMachine.h"
#include "StateExample.h"
#include <raylib.h>

extern State stateExample;
float dt;

int main(void) {
  // Init window and all...

  SM_ChangeState(&stateExample, NULL);

  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    SM_Update(dt);

    // Begin Drawing...
    SM_Draw();
    // EndDrawing...
  }

  // Unload stuff, close window, return 0...
}
```

---

## 🧩 How to Use the State Machine pt. 2

You can pass custom data between states using the `args` parameter of `SM_ChangeState`. Typically, you define a custom struct to hold any data you want to share, and then pass a pointer to this struct as `args`. `SM_ChangeState` will first call the exit function of the current state, followed by the enter function of the next state.

```C
// Passing arguments between states example

#include "StatePlay.h"
#include "StateGameOver.h"

typedef struct {
  int score;
  float timeElapsed;
} GameOverArgs;

static GameOverArgs args; // You can also have it as a pointer,
                          // just remember to malloc and free

void StatePlayUpdate(float dt) {
  // Gameplay update logic...

  if (playerLost) {
    SM_ChangeState(&stateGameOver, &args);
  }
}

void StatePlayExit(void){
  args.score = currentScore;
  args.timeElapsed = elapsedTime;
}
```

---

## 🧩 How to Use the State Machine pt. 3

In the next state's `enter` function, you cast the `void*` argument back to your custom struct type to access the data. This allows flexible communication and state initialization based on dynamic data, and avoiding management of global variables. For working code examples demonstrating this pattern, check out the `StateMachine/examples/usage` folder.

```C
// Continuation of passing arguments between states example

#include "StatePlay.h"
#include "StateGameOver.h"

// State struct, other functions...
static int score;
static float elapsedTime;

void StateGameOverEnter(void *args) {
  GameOverArgs *myArgs = (GameOverArgs)args;
  score = myArgs->score;
  elapsedTime = myArgs->elapsedTime;
}

```
