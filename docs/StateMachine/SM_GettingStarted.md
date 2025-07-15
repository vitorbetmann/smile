# SMILE State Machine: Getting Started 🤖

The StateMachine module helps you organize your game's flow using modular state transitions.

It lets you easily transition between different states — like menus, gameplay, or game over screens — using a clean and well-defined lifecycle.

---

## State Lifecycle Overview

The typical lifecycle of a state in the StateMachine module follows these steps: enter → update → draw → exit.

Users define states as `State` structs that contain function pointers for these lifecycle methods. To switch between states, you register transitions using the `SM_ChangeStateTo` function.

Call `SM_ChangeStateTo` to trigger a transition. During the game loop, the update and draw functions of the active state are called each frame.

## 🧪 Example Usage

```c
// main.c
#include "StateMachine.h"
#include "StateOne.h"
#include "StateTwo.h"

int main(void) {
    // Engine setup
    SM_Init();

    // Register your states. Callback functions declared in respective header files.
    SM_RegisterState("one", NULL, StateOneUpdate, StateOneDraw, StateOneExit);
    SM_RegisterState("two", StateTwoEnter, StateTwoUpdate, StateTwoDraw, NULL);

    // Start in the first state
    SM_ChangeStateTo("one", NULL);    // This stat takes no arguments, so we pass in NULL

    float dt = 0.016f;                // Mock delta time, about 60 FPS
    while (true) {
        // Some condition to break
        SM_Update(dt);
        SM_Draw();
    }

    // Shutdown
    SM_Shutdown();
}
```

---

You can pass custom data between states using the `args` parameter of `SM_ChangeStateTo`. Typically, you define a custom struct to hold any data you want to share, and then pass a pointer to this struct as `args`. `SM_ChangeStateTo` will first call the exit function of the current state, followed by the enter function of the next state.

---

Let's say this is StateTwo.h

---

```c
// StateTwo.h
#ifndef STATE_TWO_H
#define STATE_TWO_H

// --------------------------------------------------
// Data types
// --------------------------------------------------
typedef struct {
    int   someIntData;      // Custom integer data
    char *someCharData;     // Custom string data
} StateTwoArgs; /*
    This is the data that will be passed to
    SM_ChangeStateTo, if its enter function
    requires any arguments.
*/

// --------------------------------------------------
// Prototypes
// --------------------------------------------------
void StateTwoEnter(void *args);       // Called when entering StateTwo
void StateTwoUpdate(float dt);        // Called every frame to update StateTwo
void StateTwoDraw(void);              // Called every frame to draw StateTwo

#endif
```

---

And then you can access the StateTwoArgs struct to pass in the necessary args into SM_ChangeStateTo. In StateOne.c:

---

```c
// StateOne.c
#include "StateOne.h"
#include "StateTwo.h"
#include "StateMachine.h"

// No Enter function needed here, so NULL was passed in when registering this state.

void StateOneUpdate(float dt) {
    // Update whatever.

    // Let's say that if the user presses "SPACE" they should change state.
    // So, after they press it:
    StateTwoArgs args = {
        .someIntData = 0,
        .someCharData = "going into state two"
    };
    SM_ChangeStateTo("two", &args);
}

void StateOneDraw(void) {
    // Draw stuff
}

void StateOneExit(void) {
    // Exit cleanup
}
```

---

In the next state's `enter` function, you cast the `void*` argument back to your custom struct type to access the data. This allows flexible communication and state initialization based on dynamic data, and avoids management of global variables.

---

```c
// StateTwo.c
#include "StateTwo.h"
#include "StateMachine.h"

static int   someIntData;
static char *someCharData;

void StateTwoEnter(void *args) {
    const StateTwoArgs *myArgs = (const StateTwoArgs *)args;
    someIntData = myArgs->someIntData;
    someCharData = myArgs->someCharData;
}

void StateTwoUpdate(float dt) {
    // Update whatever.

    // Let's say that if the user presses "BACKSPACE" they should change state.
    // So, after they press it:
    SM_ChangeStateTo("one", NULL);    /* Pass NULL if the next state's enter
                                         function requires no arguments or doesn't exist. */
}

void StateTwoDraw(void) {
    // Draw stuff
}

// No Exit function needed here, so NULL was passed in when registering this state.
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
  Use `SM_ChangeStateTo(const char *name, void *args)` to switch states. It calls the current state's exit function, then the next state's enter function with the provided arguments.

- **State Machine Control:**  
  Use `SM_Init()` to initialize and `SM_Shutdown()` to cleanup the state machine.

For detailed function documentation, see the [State Machine API Reference](./SM_API.md).

---

### 🔍 Quick Reference Table

| Function              | Description                            |
| --------------------- | -------------------------------------- |
| `SM_Init()`           | Initializes the state machine          |
| `SM_RegisterState()`  | Registers a new state                  |
| `SM_ChangeStateTo()`  | Switches to another state by name      |
| `SM_Update(float dt)` | Updates the current state              |
| `SM_Draw()`           | Draws the current state                |
| `SM_Shutdown()`       | Cleans up and resets the state machine |
