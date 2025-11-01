# State Machine — Getting Started 🤖

The State Machine module provides a simple, flexible system for defining and
controlling game flow through independent states. Each state can specify its own
behavior using enter, update, draw, and exit callback functions.

### 🚨 Warning! This module is not thread-safe!

---

<br>

## Table of Contents

- [Visual Example](#visual-example)
- [Module Header](#module-header)
- [State Machine Lifecycle](#state-machine-lifecycle-)
- [Quick Reference Table](#-quick-reference-table)
- [Workflow Example](#-workflow-example)

---

<br>

## Visual Example

![StateMachine Demo](docs/_Internal/__Assets/StateMachine/StateMachineDemo.gif)

---

<br>

## 😊Module Header

The module’s header is `StateMachine.h`. Its full Smile path is:
`smile/include/StateMachine.h`

✅ Example

```c
#include <StateMachine.h>
```

---

<br>

## State Machine Lifecycle 🔄

1️⃣ Begin with `smStart`. This initializes the internal system, allowing you to
register, switch, and manage states. No other function except `smIsRunning` will
work until this step is complete.

2️⃣ Define each state in its own header/source files to represent a
self-contained scene (e.g., a main menu, level, or pause screen). Each state
must define at least one, and up to four, lifecycle callbacks (enter, update,
draw, and/or exit).

3️⃣ `smCreateState` is used to register uniquely named states with its callbacks
into memory. You can create as many states as you like.

4️⃣ Use `smSetState` to transition into a new state, optionally passing data to
it.

5️⃣ `smUpdate` and `smDraw` should be called every frame (typically inside your
game loop) to run the current state’s logic and rendering.

6️⃣ Always call `smStop` when the state machine is no longer needed. This
ensures all registered states are properly cleaned up, preventing memory leaks
and dangling pointers.

---

<br>

## 🔍 Quick Reference Table

For detailed documentation, see
the [State Machine API Reference](StateMachineAPI.md).

### Data Types

— _Function Pointers_

| Signature                       | Description                                                                                                      |
|---------------------------------|------------------------------------------------------------------------------------------------------------------|
| `void (*smEnterFn)(void *args)` | Runs once when entering the state, often for loading assets or initialize data. Can take in optional parameters. |     
| `void (*smUpdateFn)(float dt)`  | Runs every frame to update game logic. `dt` is the delta time since the last frame.                              |
| `void (*smDrawFn)(void)`        | Runs every frame to render visuals.                                                                              |
| `void (*smExitFn)(void)`        | Runs once when leaving the state. Often used for freeing memory and unloading resources.                         |

### Functions

| Signature                                                                                                        | Description                                                                                                                                      |
|------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| `bool smStart(void)`                                                                                             | Initializes the state machine and prepares it for use. Returns `true` if successful.                                                             |
| `bool smHasStarted(void)`                                                                                        | Checks whether the state machine has been initialized.                                                                                           |
| `bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn)` | Registers a new named state with its lifecycle callbacks. Returns `true` if created successfully.                                                |
| `bool smStateExists(const char *name)`                                                                           | Checks if a state with the given name exists.                                                                                                    |
| `bool smSetState(const char *name, void *args)`                                                                  | Sets the active state by name and calls its `enter` function. Calls the previous state’s `exit` function before switching.                       |
| `const char *smGetCurrentStateName(void)`                                                                        | Returns the name of the current active state, or `nullptr` if none is active.                                                                    |
| `bool smDeleteState(const char *name)`                                                                           | Deletes a state by name. Fails if it’s currently active.                                                                                         |
| `int smGetStateCount(void)`                                                                                      | Returns the total number of registered states, or `-1` if the state machine is not started.                                                      |
| `bool smUpdate(float dt)`                                                                                        | Calls the update function of the active state. Returns `true` if successful.                                                                     |
| `float smGetDt(void)`                                                                                            | Returns the delta time (in seconds) since the last frame. Useful for maintaining consistent time-based updates across frames.                    |
| `bool smDraw(void)`                                                                                              | Calls the draw function of the active state. Returns `true` if successful.                                                                       |
| `bool smStop(void)`                                                                                              | Stops the state machine and frees all registered states. Calls the current state’s `exit` function before cleanup. Returns `true` if successful. |

--- 

<br>

## 🧪 Workflow Example

In your main file you can begin like so:

```c
#include <StateMachine.h>
#include "Menu.h"
#include "LevelOne.h"
#include "LevelTwo.h"

int main(void) {
    // Other State Machine functions will not work if smStart is not called.
    smStart();
    
    // Create your states. Callback functions declared in respective header files.
    smCreateState("menu", nullptr, menuUpdate, menuDraw, menuExit);
    smCreateState("level 1", nullptr, levelOneUpdate, levelOneDraw, levelOneExit);
    smCreateState("level 2", levelTwoEnter, levelTwoUpdate, levelTwoDraw, nullptr);
    
    /* Start in the menu. In this example it requires no arguments, so we pass
     * in nullptr.
     */
    smSetState("menu", nullptr); 
    
    // Below is a typical game loop using Smile 
    while (smIsRunning()) {    
        float dt = smGetDt();
        smUpdate(dt);
        smDraw();
    }
}
```

Make sure you call `smStop` before exiting your program, otherwise you risk
memory leaks!

Calling smStop also calls the current state's exit function and sets
`smIsRunning` to false, breaking the main game loop. Good places to call
`smStop()` include your main menu or pause screen:

```c
#include <StateMachine.h>
#include "menu.h"

void menuUpdate(float dt) {
    // Handle update
    ...
    
    if (PlayButtonPressed()) {
        smSetState("level 1", nullptr);
    }
    
    if (QuitButtonPressed()) {
        smStop();
    }
}

void menuDraw(void) {
    // Handle rendering
}

void menuExit(void) {
    // Handle cleanup
}
```

You can pass custom data between states through the `args` parameter of
`smSetState()`. Typically, this involves defining a custom struct to hold any
data you want to share, then passing a pointer to it as `args`.

`smSetState` will first call the exit function of the current state,
followed by the enter function of the next state. See
the [State Machine API](StateMachineAPI.md) for more information.

Below is a typical state header file using LevelTwo.h as an example:

```c
#ifndef LEVEL_TWO_H
#define LEVEL_TWO_H

/* Below is the data that will be passed to smSetState if its enter function
 * requires any arguments.
 */

typedef struct {
    int score;
    float position;
} PlayerData;

void levelTwoEnter(void *args);   // Called when entering levelTwo
void levelTwoUpdate(float dt);          // Called each frame to update levelTwo
void levelTwoDraw(void);                // Called each frame to draw levelTwo
                                        // levelTwo has no Exit function

#endif
```

You can now access the PlayerData struct to pass in the necessary args into
`smSetState`. In LevelOne.c:

```c
#include <StateMachine.h>
#include "LevelOne.h"
#include "LevelTwo.h" // Must include to have access to PlayerScore

static score = 0;
static float playerPosition = 0;

// LevelOne has no Enter function.

void levelOneUpdate(float dt) {
    // Handle update. For example:
    score += 10;
    playerPosition += 5.0f * dt;

    // Passing player data to next state.    
    if (playerWon) {
        PlayerData playerData = { 
            .score = 100,
            .position = 200.0;
        };
        smSetState("level 2", &playerData);
    }
}

void levelOneDraw(void) {
    // Handle rendering
}

void levelOneExit(void) {
    // Handle cleanup
    score = 0;
    playerPosition = 0.0f;
}
```

In the next state's `enter` function, you cast the `void *` argument back to
your custom struct type to access the data. This enables flexible communication
and state initialization based on dynamic data, and avoids reliance on global
variables.

So, in LevelTwo.c:

```c
#include <StateMachine.h>
#include "LevelTwo.h"

static PlayerData *myPlayerData;

void levelTwoEnter(void *args) {    
    myPlayerData = malloc(sizeof(PlayerData));
    if (!myPlayerData) {
        // Handle malloc fail
    }
    
    PlayerData *myArgs = args;
    
    myPlayerData->score = myArgs->score;
    myPlayerData->position = myArgs->position;
}

void levelTwoUpdate(float dt) {
    // Update whatever. For example:
    myPlayerData->position += 5.0f * dt;
    
    if (gameOver) {
        /* Pass nullptr if the next state's enter function requires no arguments
        * or doesn't exist.
        */
        smStop();
    }
                                  
}

void levelTwoDraw(void) {
    // Handle rendering
}

// levelTwo has no Exit function
```

---

<br>

| Last modified | Author (Discord)             | Description       |
|---------------|------------------------------|-------------------|
| Oct 14, 2025  | Vitor Betmann (vitorbetmann) | Created document. |