# State Machine API 🤖

Lightweight framework for managing states, transitions, and lifecycle callbacks.

The State Machine module provides a simple, flexible system for defining and
controlling game flow through independent states. Each state can specify its own
behavior using enter, update, draw, and exit callback functions.

### 🚨 Warning! This module is not thread-safe

---

## Table of Contents

- [Data Types](#-data-types)
    - [Function Pointers](#_function-pointers_-)
- [Functions](#-functions)
    - [Start Related](#_start-related_)
    - [State Functions](#_state-funcitons_)
    - [Lifecycle Functions](#_lifecycle-functions_)
    - [Stop Related](#_stop-related_)
- [Workflow Examples](#-workflow-examples)

---

<br>

## 📦 Data Types

### _Function Pointers_ ➡

| `void (*smEnterFn)(void *args)` |
|---------------------------------|

Function pointer type for state entry callbacks.

- **Parameters:**
    - `args` — Optional arguments passed when entering the state.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

| `void (*smUpdateFn)(float dt)` |
|--------------------------------|

Function pointer type for state update callbacks.

- **Parameters:**
    - `dt` — Delta time in seconds since the last update.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

| `void (*smDrawFn)(void)` |
|--------------------------|

Function pointer type for state draw callbacks.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

| `void (*smExitFn)(void)` |
|--------------------------|

Function pointer type for state exit callbacks.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

For more, see [Workflow Examples](#-workflow-examples).

---

<br>

## 🔧 Functions

### _Start Related_

| `bool smStart(void)` |
|----------------------|

Initializes the state machine and prepares it for use.

- **Returns:** True if the state machine started successfully, false otherwise.

- **Note:** Calling this function when the state machine is already started will
  log a warning and return false.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

| `bool smHasStarted(void)` |
|---------------------------|

Checks whether the state machine has been initialized.

- **Returns:** True if the state machine has been started, false otherwise.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

For more, see [Workflow Examples](#-workflow-examples).

---

### _State Funcitons_

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
NO EXAMPLE YET
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
NO EXAMPLE YET
```

<br>

| `smSetState(const char *name, void *args) |
|-------------------------------------------|

Sets the current active state by name and triggers its enter function.

- **Parameters:**
    - `name` — Name of the state to switch to.
    - `args` — Optional arguments passed to the state's enter function.

- **Returns:** True if the state was successfully changed, false otherwise.

- **Note:** If a state is active, its exit function is called before switching.

- **Example:**

```c
NO EXAMPLE YET
```

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
NO EXAMPLE YET
```

<br>

| `bool smDeleteState(const char *name)` |
|----------------------------------------|

Deletes a state by name from the state machine.

- **Parameters:**
    - `name` — Name of the state to delete.

- **Returns:** True if the state was successfully deleted, false otherwise.

- **Note:** Attempting to delete the currently active state will fail.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

| `int smGetStateCount(void)` |
|-----------------------------|

Retrieves the total number of registered states.

- **Returns:** The number of registered states, or -1 if the state machine is
  not started.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

For more, see [Workflow Examples](#-workflow-examples).

---

### _Lifecycle Functions_

| `bool smUpdate(float dt)` |
|---------------------------|

Updates the currently active state.

- **Parameters:**
    - `dt` — Delta time in seconds since the last update.

- **Returns:** True if the update function was called successfully, false
  otherwise.

- **Note:** If the current state has no update function, a warning is logged.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

| `bool smDraw(void)` |
|---------------------|

Executes the draw function of the currently active state.

- **Returns:** True if the draw function was called successfully, false
  otherwise.

- **Note:** If the current state has no draw function, a warning is logged.

- **Example:**

```c
NO EXAMPLE YET
```

<br>

For more, see [Workflow Examples](#-workflow-examples).

### _Stop Related_

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
NO EXAMPLE YET
```

<br>

For more, see [Workflow Examples](#-workflow-examples).

---

<br>

## 📖 Workflow Examples

```c
NO EXAMPLE YET
```