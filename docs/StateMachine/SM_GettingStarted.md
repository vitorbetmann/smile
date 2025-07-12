# State Machine: Getting Started 🤖

The StateMachine module helps you organize your game's flow using modular state transitions.

It lets you easily swap between different states like menus, gameplay, game over screens, and more, using a clean and well-defined state lifecycle.

---

## State Lifecycle Overview

The typical lifecycle of a state in the StateMachine module follows these steps: enter → update → draw → exit.

Users define states as `State` structs that contain function pointers for these lifecycle methods. To switch between states, you register transitions using the `SM_ChangeState` function.

Call `SM_ChangeState` to trigger a transition. During the game loop, the update and draw functions of the active state are called each frame.

You can pass custom data between states using the `args` parameter of `SM_ChangeState`. Typically, you define a custom struct to hold any data you want to share, and then pass a pointer to this struct as `args`. `SM_ChangeState` will first call the exit function of the current state, followed by the enter function of the next state.

In the next state's `enter` function, you cast the `void*` argument back to your custom struct type to access the data. This allows flexible communication and state initialization based on dynamic data, and avoids management of global variables.

## 🧪 Example Usage (w/ raylib)

```c
// Engine setup
SM_Init(NULL);

const State *menu = NewState("Menu", MenuEnter, MenuUpdate, MenuDraw, MenuExit);
const State *game = NewState("Game", GameEnter, GameUpdate, GameDraw, GameExit);

// Start in the menu state
SM_ChangeState(menu, NULL);

// Main loop
while (!WindowShouldClose()) {
  SM_Update(GetFrameTime());
  SM_Draw();
}

// Shutdown
SM_Shutdown();
```

---

You can pass custom data between states using the `args` parameter of `SM_ChangeState`. Typically, you define a custom struct to hold any data you want to share, and then pass a pointer to this struct as `args`. `SM_ChangeState` will first call the exit function of the current state, followed by the enter function of the next state.

---

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

```C
// Continuation of passing arguments between states example

#include "StatePlay.h"
#include "StateGameOver.h"

// State struct, other functions...
static int score;
static float elapsedTime;

void StateGameOverEnter(void *args) {
  GameOverArgs *myArgs = (GameOverArgs*)args;
  score = myArgs->score;
  elapsedTime = myArgs->timeElapsed;
}
```

---

## State Lifecycle and Function Summary

- **State Lifecycle:**  
  States have four main functions:

  - `enter(void *args)`: called when entering a state, receives optional arguments.
  - `update(float dt)`: called every frame to update the state logic.
  - `draw(void)`: called every frame to render the state.
  - `exit(void)`: called when leaving a state.

- **State Struct:**  
  The `State` struct holds pointers to these lifecycle functions.

- **State Transitions:**  
  Use `SM_ChangeState(State *nextState, void *args)` to switch states. It calls the current state's exit function, then the next state's enter function with the provided arguments.

- **Update and Draw:**  
  Call `SM_Update(float dt)` and `SM_Draw()` every frame to update and render the current state.

- **State Machine Control:**  
  Use `SM_Init()` to initialize and `SM_Shutdown()` to cleanup the state machine.

For detailed function documentation, see the [State Machine API Reference](./SM_API.md).
