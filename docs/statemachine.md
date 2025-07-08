## Welcome to the StateMachine Module 🤖

The StateMachine module helps you organize your game's flow using modular state transitions.

It lets you easily swap between different states like menus, gameplay, game over screens, and more, using a clean and well-defined state lifecycle.

## 🧩 How to Use the State Machine pt. 1

The typical lifecycle of a state in the StateMachine module follows these steps: enter → update → draw → exit.

Users define states as `State` structs that contain function pointers for these lifecycle methods. To switch between states, you register transitions using the `SM_ChangeState` function.

Call `SM_ChangeState(&nextState, args)` to trigger a transition. This will first call the exit function of the current state, followed by the enter function of the next state. During the game loop, the update and draw functions of the active state are called each frame.

```C
// First use example

#include <raylib.h>
#include "StateMachine.h"

extern State example_state;
float dt;

int main(void) {
  // Init window and all

  sm_change_state(&example_state, NULL);

  while (!WindowShouldClose()) {
    dt = GetFrameTime;
    sm_update(dt);
    sm_draw();
  }

  // Unload stuff, close window

  return 0;
}
```

---

## 🧩 How to Use the State Machine pt. 2

You can pass custom data between states using the `args` parameter of `SM_ChangeState`. Typically, you define a custom struct to hold any data you want to share, and then pass a pointer to this struct as `args`. In the next state's `enter` function, you cast the `void*` argument back to your custom struct type to access the data. This allows flexible communication and state initialization based on dynamic data. For working code examples demonstrating this pattern, check out the `StateMachine/examples/usage` folder.

```C
// Passing arguments between states example

typedef struct {
  int score;
  float timeElapsed;
} GameOverArgs;

void gameplay_update(float dt) {
  // Gameplay update logic...

  if (playerLost) {
    GameOverArgs args = { .score = currentScore, .timeElapsed = elapsedTime };
    sm_change_state(&game_over_state, &args);
  }
}
```

---
