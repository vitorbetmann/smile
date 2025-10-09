# State Machine — API 🤖

Lightweight framework for managing states, transitions, and lifecycle callbacks.

The State Machine module provides a simple, flexible system for defining and
controlling game flow through independent states. Each state can specify its own
behavior using enter, update, draw, and exit callback functions.

### 🚨 Warning! This module is not thread-safe

---

<br>

## Table of Contents

- [Data Types](#-data-types)
    - [Function Pointers](#-_function-pointers_)
- [Functions](#-functions)
    - [Start Related](#-_start-related_)
    - [State Functions](#-_state-funcitons_)
    - [Lifecycle Functions](#-_lifecycle-functions_)
    - [Stop Related](#-_stop-related_)

---

<br>

## 📦 Data Types

### — _Function Pointers_

| `void (*smEnterFn)(const void *args)` |
|---------------------------------------|

Function pointer type for state entry callbacks.

- **Parameters:**
    - `args` — Optional arguments passed when entering the state.

- **Notes:**
    - The enter function should handle its own resource management, such as
      allocating memory, loading assets, or performing initialization steps.
    - If any operation fails during setup (for example, memory allocation or
      file loading), the user is responsible for handling the failure within the
      function.

- **Example:**

```c
typedef struct {
    float xPosition;
} PlayerData;

PlayerData *myPlayerData;

void myStateEnter(const void *args) {
    myPlayerData = malloc(sizeof(PlayerData));
    if (!myPlayerData) {
        // Handle malloc fail
        return;
    }

    myPlayerData->xPosition = ((const PlayerData *)args)->xPosition;
}
```

<br>

| `void (*smUpdateFn)(float dt)` |
|--------------------------------|

Function pointer type for state update callbacks.

- **Parameters:**
    - `dt` — Delta time in seconds since the last update.

- **Example:**

```c
void myStateUpdate(float dt) {
    myPlayerData->xPosition += 5.0f * dt;
}
```

<br>

| `void (*smDrawFn)(void)` |
|--------------------------|

Function pointer type for state draw callbacks.

- **Example:**

```c
#include <raylib.h> // This example uses raylib for rendering 

void myStateDraw(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Player is a red circle moving right.    
    int radius = 10;
    int yPosition = 200;
    DrawCircle(myPlayerData->xPosition, yPosition, radius, RED);
    
    EndDrawing();
}
```

<br>

| `void (*smExitFn)(void)` |
|--------------------------|

Function pointer type for state exit callbacks.

- **Example:**

```c
void myStateExit(void) {
    free(myPlayerData);
    myPlayerData = nullptr;
}
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [State Machine Getting Started](README.md) doc.

---

<br>

## 🔧 Functions

### — _Start Related_

| `bool smStart(void)` |
|----------------------|

Initializes the state machine and prepares it for use.

- **Returns:** True if the state machine started successfully, false otherwise.

- **Notes:**
    - Calling this function when the state machine is already started will log a
      warning and return false.

- **Example:**

```c
#include "StateMachine.h"

smStart();
```

<br>

| `bool smHasStarted(void)` |
|---------------------------|

Checks whether the state machine has been initialized.

- **Returns:** True if the state machine has been started, false otherwise.

- **Example:**

```c
if (smHasStarted()) {
    ...
}
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [State Machine Getting Started](README.md) doc.

---

### — _State Funcitons_

| `bool smCreateState(const char *stateName, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn)` |
|-----------------------------------------------------------------------------------------------------------------------|

Creates a new state with the specified name and callback functions.

- **Parameters:**
    - `stateName` — Unique name identifying the state.
    - `enterFn` — Callback executed when entering the state.
    - `updateFn` — Callback executed each frame during update.
    - `drawFn` — Callback executed each frame during rendering.
    - `exitFn` — Callback executed when exiting the state.

- **Returns:** True if the state was created successfully, false otherwise.

- **Notes:**
    - All function pointers are optional, but at least one must be provided.
    - Attempting to create a state that already exists will fail.

- **Example:**

```c
// ----- In levelOne.h -----
#ifndef LEVEL_ONE_H
#define LEVEL_ONE_H

void levelOneUpdate(float dt);        // Called each frame to update levelOne
void levelOneDraw(void);              // Called each frame to draw levelOne
void levelOneExit(void);              // Called when exiting levelOne

#endif


// ----- StateOne.c -----
#include "StateOne.h"
#include "StateMachine.h"

// No Enter function needed here as nullptr passed in when registering this state.

void levelOneUpdate(float dt) {
    // Handle update
    
    if (playerWon) {
        smSetState("level 2", nullptr);
    }
}

void levelOneDraw(void) {
    // Handle rendering
}

void levelOneExit(void) {
    // Handle cleanup
}

// ----- In main.c or in another state. -----
SM_RegisterState("level 1", nullptr, levelOneUpdate, levelOneDraw, levelOneExit);
```

<br>

| `bool smStateExists(const char *name)` |
|----------------------------------------|

Checks whether a state with the given name exists.

- **Parameters:**
    - `name` — Name of the state to check.

- **Returns:** True if the state exists, false otherwise.

- **Example:**

```c
if (!smStateExists("level 1") {
    ...
}
```

<br>

| `bool smSetState(const char *name, void *args)` |
|-------------------------------------------------|

Sets the current active state by name and triggers its enter function.

- **Parameters:**
    - `name` — Name of the state to switch to.
    - `args` — Optional pointer to arguments passed to the state's enter
      function.

- **Returns:** True if the state was successfully changed, false otherwise.

- **Notes:**
    - If a state is already active, its exit function is called before
      switching.
    - The `args` pointer may be `NULL` if no data is required.

- **Example:**

```c
// Switch to "level 1" with no arguments.
smSetState("level 1", NULL);

...

// Passing player data to next state.
typedef struct {
    int currentScore;
    int highScore;
} PlayerScore;

PlayerScore score = { 10, 50 };
smSetState("level 2", &score);
```

<br>

| `const char *smGetCurrentStateName(void)` |
|-------------------------------------------|

Retrieves the name of the currently active state.

- **Returns:** A pointer to the name of the current state, or NULL if none is
  active.

- **Notes:**
    - The returned string is owned by the state machine. The user must not
      attempt to modify or free it and should make a copy of the string if they
      wish to do so.
    - The pointer remains valid until the state is deleted or the state machine
      is stopped.

- **Example:**

```c
const char *currState = smGetCurrentStateName();
```

<br>

| `bool smDeleteState(const char *name)` |
|----------------------------------------|

Deletes a state by name from the state machine.

- **Parameters:**
    - `name` — Name of the state to delete.

- **Returns:** True if the state was successfully deleted, false otherwise.

- **Notes:**
    - Attempting to delete the currently active state will fail.

- **Example:**

```c
smDeleteState("level 1");
```

<br>

| `int smGetStateCount(void)` |
|-----------------------------|

Retrieves the total number of registered states.

- **Returns:** The number of registered states, or -1 if the state machine is
  not started.

- **Example:**

```c
int myStateCount = smGetStateCound();
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [State Machine Getting Started](README.md) doc.

---

### — _Lifecycle Functions_

| `bool smUpdate(float dt)` |
|---------------------------|

Updates the currently active state.

- **Parameters:**
    - `dt` — Delta time in seconds since the last update.

- **Returns:** True if the update function was called successfully, false
  otherwise.

- **Notes:**
    - If the current state has no update function, a warning is logged.

- **Example:**

```c
#include <time.h>

...

clock_t lastTime = clock();

while (true) {    
    clock_t currentTime = clock();
    float dt = (float)(currentTime - lastTime) / CLOCKS_PER_SEC;
    lastTime = currentTime;

    smUpdate(dt);
    ...
}
```

<br>

| `bool smDraw(void)` |
|---------------------|

Executes the draw function of the currently active state.

- **Returns:** True if the draw function was called successfully, false
  otherwise.

- **Notes:**
    - If the current state has no draw function, a warning is logged.

- **Example:**

```c
while (true) {
    ...
    smDraw();
}
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [State Machine Getting Started](README.md) doc.

---

<br>

### — _Stop Related_

| `bool smStop(void)` |
|---------------------|

Stops the state machine and frees all allocated states.

- **Returns:** True if the state machine was successfully stopped, false
  otherwise.

- **Notes:**
    - The exit function of the current state is called before cleanup.
    - After stopping, the state machine must be restarted with smStart().

- **Example:**

```c
int main(void) {

    smStart();
    ...

   while (true) {
      ...
      smUpdate(dt);
      smDraw();
   }

   smStop();
}
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [State Machine Getting Started](README.md) doc.

---

<br>