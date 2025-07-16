# SMILE State Machine: Getting Started 🤖

The StateMachine module lets you easily transition between different states—like menus, gameplay, or game over screens—using a clean and well-defined lifecycle.

---

## State Lifecycle Overview

The typical lifecycle of a state in the StateMachine module follows these steps: enter → update → draw → exit.

Users register states by calling `SM_RegisterState` and passing a unique name along with the necessary callback functions. To switch between states, call `SM_ChangeStateTo` with the unique name of the target state and optional arguments.

During the game loop, the update and draw functions of the active state are called on each frame.

## 🧪 Example Usage

```c
// main.c
#include "StateMachine.h"
#include "StateOne.h"
#include "StateTwo.h"

int main(void) {

   // Other SM_ functions will not work if SM_Init not called
    SM_Init();

   // Register your states. Callback functions declared in respective header files.
   SM_RegisterState("one", NULL, StateOneUpdate, StateOneDraw, StateOneExit);
   SM_RegisterState("two", StateTwoEnter, StateTwoUpdate, StateTwoDraw, NULL);

   // Start in the first state
   SM_ChangeStateTo("one", NULL);    // This example state requires no arguments, so we pass in NULL

   float dt = 0.016f;                // Mock delta time, about 60 FPS
   bool isRunning = true;
   while (isRunning) {
      SM_Update(dt);
      SM_Draw();
   }

   // Don't end you program without calling SM_Shutdown. Risk of memory leak.
   SM_Shutdown();
}
```

---

You can pass custom data between states using the `args` parameter of `SM_ChangeStateTo`. Typically, you define a custom struct to hold any data you want to share, and then pass a pointer to this struct as `args`. `SM_ChangeStateTo` will first call the exit function of the current state, followed by the enter function of the next state. See the [State Machine API](./SM_API.md) for more information.

---

Let's say this is StateTwo.h:

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
void StateTwoUpdate(float dt);        // Called each frame to update StateTwo
void StateTwoDraw(void);              // Called each frame to draw StateTwo
// In this example, StateTwo has no Exit function

#endif
```

---

You can then access the StateTwoArgs struct to pass in the necessary args into SM_ChangeStateTo. In StateOne.c:

---

```c
// StateOne.c
#include "StateOne.h"
#include "StateTwo.h" // Must include to have access to StateTwoArgs
#include "StateMachine.h"

// No Enter function needed here. Remember NULL was passed in when registering this state.

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

In the next state's `enter` function, you cast the `void*` argument back to your custom struct type to access the data. This enables flexible communication and state initialization based on dynamic data, avoiding reliance on global variables.

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

// No Exit function needed here. Remember NULL was passed in when registering this state.
```

---

## State Lifecycle and Function Summary

- **State Machine Control:**  
  Use `SM_Init()` to initialize and `SM_Shutdown()` to clean up the state machine.

- **State Registration:**  
  Use `SM_RegisterState(const char *name, EnterFunc enter, UpdateFunc update, DrawFunc draw, ExitFunc exit)` to register a new state with its lifecycle callbacks. The state name must be unique, and at least one callback must be non-NULL.

- **State Lifecycle:**  
  States have four main functions:

  - `enter(void *args)`: called when entering a state, receives optional arguments.
  - `update(float dt)`: called every frame to update the state logic.
  - `draw(void)`: called every frame to render the state.
  - `exit(void)`: called when leaving a state.

- **State Transitions:**  
  Use `SM_ChangeStateTo(const char *name, void *args)` to switch states. It calls the current state's exit function, then the next state's enter function with the provided arguments.

For detailed function documentation, see the [State Machine API Reference](./SM_API.md).

---

### 🔍 Quick Reference Table

| Function                                                                                                                                | Description                                                                        |
| --------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| `bool SM_Init(void)`                                                                                                                    | Initializes the state machine. Returns `true` if successful.                       |
| `bool SM_RegisterState(const char *name, void (*enterFn)(void *), void (*updateFn)(float), void (*drawFn)(void), void (*exitFn)(void))` | Registers a new named state with lifecycle callbacks.                              |
| `bool SM_ChangeStateTo(const char *name, void *args)`                                                                                   | Switches to a different state by name, optionally passing arguments.               |
| `bool SM_Update(float dt)`                                                                                                              | Calls the update function of the current active state. Returns `true` on success.  |
| `bool SM_Draw(void)`                                                                                                                    | Calls the draw function of the current active state. Returns `true` on success.    |
| `bool SM_Shutdown(void)`                                                                                                                | Shuts down the state machine and frees internal memory. Returns `true` on success. |
| `bool SM_IsInitialized(void)`                                                                                                           | Checks if the state machine has been initialized. Returns `true` if yes.           |
| `bool SM_IsStateRegistered(char *name)`                                                                                                 | Checks if a state with the given name is registered.                               |
| `const char *SM_GetCurrStateName(void)`                                                                                                 | Returns the name of the current active state or `NULL` if none.                    |
