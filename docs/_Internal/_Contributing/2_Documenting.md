# SMILE Documentation Guide 📗

---

<br>

## 1️⃣ Overview

---

<br>

## 2️⃣ Documenting Header Files (`.h`)

---

<br>

## 3️⃣ Documenting Source Files (`.c`)

---

<br>

## 4️⃣ Documenting Markdown Files (`.md`)

### — _4.1 `_GettingStarted.md` Files_

Summary

<br>

### — _4.2 `_API.md` Files_

Summary (sections must be separated by --- followed by \<br>)

| `Header` |
|----------|

Summary

```c
# State Machine API 🤖

Lightweight framework for managing states, transitions, and lifecycle callbacks.

The State Machine module provides a simple, flexible system for defining and
controlling game flow through independent states. Each state can specify its own
behavior using enter, update, draw, and exit callback functions.

### 🚨 Warning! This module is not thread-safe
```

<br>

| `Table of Contents` |
|---------------------|

Summary

```c
## Table of Contents

- [Data Types](#-data-types)
    - [Function Pointers](#-_function-pointers_)
- [Functions](#-functions)
    - [Start Related](#-_start-related_)
    - [State Functions](#-_state-funcitons_)
    - [Lifecycle Functions](#-_lifecycle-functions_)
    - [Stop Related](#-_stop-related_)
- [Workflow Examples](#-workflow-examples)
```

<br>

| `Sections` |
|------------|

Summary (elements of the same subsection must be separated by <br> only)

- Data Types
    - Enums
    - Function Pointers
    - Structs


- Functions

Summary (section structure)

````c
## 🔧 Functions

### — _Start Related_

| `bool smStart(void)` |
|----------------------|

Initializes the state machine and prepares it for use.

- **Returns:** True if the state machine started successfully, false otherwise.

- **Note:** Calling this function when the state machine is already started will
  log a warning and return false.

- **Example:**

```c
smStart();
```

<br>

| `bool smHasStarted(void)` |
|---------------------------|

.
.
.

<br>

For more, see [Workflow Examples](#-workflow-examples).

---

### — _State Funcitons_

````

Summary (a single func. omit params and return if void, as well as notes if not
needed. always provide summary and example. At the end of a subsection, have a
link to the
respective Workflow example)

````c
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
````

- Workflow Examples

Summary


