# Coding Guidelines — Code Organization

This document defines Smile’s standard conventions for organizing code and
code-related files.

## Table of Contents

- [General Guidelines](#-general-guidelines)
- [Header Files](#-header-files)
    - [Public Headers](#-public-headers)
    - [Internal Headers](#-internal-headers)
- [Message Files](#-message-files)
- [Source Files](#-source-files)
- [Test Files](#-test-files)

## 🗂 General Guidelines

### — Section Headers

* All code files must be organized into sections, each marked by a standardized
  comment block, referred to as a section header.
* Leave 1 blank line between a section header and the first line of code in that
  section.
* Leave 2 blank lines between the end of one section and the start of the next
  section.
* See [Message Files](#-message-files) for its particular case.

✅ Example

```c
// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <stdlib.h>


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#define GRAVITY 10
```

---

### — Section Order

* Sections in code files should have the following order and be named
  accordingly.

| Section Order | Section Element                             | Section Name         | 
|---------------|---------------------------------------------|----------------------|
| 1             | File Description Block                      | [Not Named]          |
| 2             | Include Guard                               | [Not Named]          |
| 3             | #include                                    | Includes             |
| 4             | #define                                     | Defines              |
| 5             | Data Types (e.g., enums, structs, typedefs) | Data Types           |
| 6             | Variables                                   | Variables            |
| 7             | Function Prototypes                         | Prototypes           |
| 8             | Public Functions                            | Functions — Public   |
| 9             | Internal Functions                          | Functions — Internal |
| 10            | Private Functions                           | Functions — Private  |
| 11            | Code Entry Point (i.e. main function)       | Main                 |

* Sections that might not be relevant to a certain file should be omitted.
* See [Message Files](#-message-files) for its particular case.
* See [3_Documentation_Guidelines] (🚧 Under Development) for details on writing
  File Description Blocks.

✅ Example

```c
/**
 * @file
 * @brief Public declarations of data types and functions for the Log module.
 * ...
 */


#ifndef SMILE_LOG_H
#define SMILE_LOG_H


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Function pointer type for custom fatal error handlers.
 * ...
 */
typedef void (*lgFatalHandler)(void);


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Logs a message to the terminal ending in a new line. Supports
 * ...
 */
void lgLog(const char *msg, ...);

/**
 * @brief Sets a custom handler to be called when a fatal event occurs.
 * ...
 */
void lgSetFatal(lgFatalHandler handler);


#endif // #ifndef SMILE_LOG_H
```

### — Subheaders

* Separate long sequences of messages, data types, and functions into
  subsections with `// <Subsection Name>`.
* Separate the subsection comment from functions and data types by 1 blank line.
* Do add blank lines between subsection comments and messages.
* See [Message Files](#-message-files) for its particular case.

✅ Example

```c
-- In StateMachine.h (function documentation omitted for clarity)
// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

// Start Related

bool smStart(void);

bool smIsRunning(void);

// State Functions  -- Discretly but clearly separates subsections.  

bool smCreateState(const char *name, smEnterFn enter, smUpdateFn update,
                   smDrawFn draw, smExitFn exit);

bool smStateExists(const char *name);


-- In StateMachineMessages.h
// —————————————————————————————————————————————————————————————————————————————
// Functions Names
// —————————————————————————————————————————————————————————————————————————————

// State Functions
#define FN_CREATE_STATE "CreateState"
#define FN_STATE_EXISTS "StateExists"
#define FN_SET_STATE "SetState"
#define FN_GET_CURRENT_STATE_NAME "GetCurrentStateName"
#define FN_GET_STATE_COUNT "GetStateCount"
#define FN_DELETE_STATE "DeleteState"
// Lifecycle Functions
#define FN_UPDATE "Update"
#define FN_GET_DT "GetDt"
#define FN_DRAW "Draw"
```

---

### — #include's

* `#include` statements should be organized in the following order and separated
  by 1 blank line:
    * #include <> — External libraries (e.g., stdlib.h, string.h)
    * #include "" — Related to the current module (e.g., StateMachine.h,
      StateMachineInternal.h)
    * #include "" — Files that support the current module (e.g., LogInternal,
      TestInternal.h)
* `#include` statements should be in alphabetical order.
* Only provide the file name, never the absolute or relative path to it.

✅ Example

```c
// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <string.h>
#include <time.h>
#include <uthash.h>

#include "StateMachine.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"

#include "CommonInternalMessages.h"
#include "LogInternal.h"
#include "StateMachineAPITest.h"
#include "TestInternal.h"
```

---

### — #ifndef's

* All Smile include guards begin with a `SMILE_` prefix.
* When defining, separate english words with an underscore (`_`).
* Leave 2 blank lines before the closing statement (i.e., `#endif`).
* Always comment what the closing statement is closing.

✅ Example

```c
#ifndef SMILE_STATE_MACHINE_H
#define SMILE_STATE_MACHINE_H


...


#endif // #ifndef SMILE_STATE_MACHINE_H
```

---

### — Header Dependency

* Never imply dependencies. Every file should include everything it needs.

```c
-- In StateMachineInternal.h
#include <time.h>
#include <uthash.h>
...


-- In StateMachine.c
#include <string.h>
#include <uthash.h>  -- Include again, even if already in StateMachineInternal.h
...
#include "StateMachineInternal.h"
```

## 😶 Header Files

* All headers must have `include guards`.

### — Public Headers

* Contains declarations of functions and data types of user-facing API's.
* See [Code_Style](Code_Style.md) for details on naming and declaring functions
  and data types.
* See [3_Documentation_Guidelines] (🚧 Under Development) for details on
  documenting.

✅ Example

```c
/**
 * @file
 * @brief Public declarations of data types and functions for the StateMachine
 * ...
 */

#ifndef SMILE_STATE_MACHINE_H
#define SMILE_STATE_MACHINE_H


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Function pointer type for state entry callbacks.
 * ...
 */
typedef void (*smEnterFn)(void *args);


/**
 * @brief Function pointer type for state update callbacks.
 * ...
 */
typedef void (*smUpdateFn)(float dt);

/**
 * @brief Function pointer type for state draw callbacks.
 * ...
 */
typedef void (*smDrawFn)(void);

/**
 * @brief Function pointer type for state exit callbacks.
 * ...
 */
typedef void (*smExitFn)(void);


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

// Start Related

/**
 * @brief Initializes the state machine and prepares it for use.
 * ...
 */
bool smStart(void);

/**
 * @brief Checks whether the state machine has been initialized.
 * ...
 */
bool smIsRunning(void);

// State Functions

/**
 * @brief Creates a new state with the specified name and callback functions.
 * ...
 */
bool smCreateState(const char *name, smEnterFn enter, smUpdateFn update,
                   smDrawFn draw, smExitFn exit);

/**
 * @brief Checks whether a state with the given name exists.
 * ...
 */
bool smStateExists(const char *name);

/**
 * @brief Sets the current active state by name and triggers its enter function.
 * ...
 */
bool smSetState(const char *name, void *args);

/**
 * @brief Retrieves the name of the currently active state.
 * ...
 */
const char *smGetCurrentStateName(void);

/**
 * @brief Deletes a state by name from the state machine.
 * ...
 */
bool smDeleteState(const char *name);

/**
 * @brief Retrieves the total number of registered states.
 * ...
 */
int smGetStateCount(void);

// Lifecycle Related

/**
 * @brief Updates the currently active state.
 * ...
 */
bool smUpdate(float dt);

/**
 * @brief Calculates the delta time, in seconds, since last invoked.
 * ...
 */
float smGetDt(void);

/**
 * @brief Executes the draw function of the currently active state.
 * ...
 */
bool smDraw(void);

// Stop Related

/**
 * @brief Stops the state machine and frees all allocated states.
 * ...
 */
bool smStop(void);


#endif // #ifndef SMILE_STATE_MACHINE_H
```

---

### — Internal Headers

* Contains declarations of internal functions and data types. They should help
  the module in executing its tasks but are not meant for public use.
    * For example, `smInternalGetState()` allows StateMachine to directly call
      the `enter` and `exit` functions of a state, but a user should never have
      access to that. Instead, they can call functions like `smSetState()` and
      `smUpdate()` which automatically handles memory allocation, pointer
      manipulation, and the calling of desired functions.
* See [Code_Style](Code_Style.md) for details on naming and declaring internal
  functions and data types.
* See [3_Documentation_Guidelines] (🚧 Under Development) for details on
  documenting.

✅ Example

```c
/**
 * @file
 * @brief Internal declarations of data structures and functions for the
 * ...
 */


#ifndef SMILE_STATE_MACHINE_INTERNAL_H
#define SMILE_STATE_MACHINE_INTERNAL_H


// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <time.h>
#include <uthash.h>

#include "StateMachine.h"


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#define DEFAULT_FPS 60


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Represents an individual state within the state machine.
 * ...
 */
typedef struct {
  char *name;
  smEnterFn enter;
  smUpdateFn update;
  smDrawFn draw;
  smExitFn exit;
} InternalState;

/**
 * @brief Maps a state name to its corresponding State structure.
 * ...
 */
typedef struct {
  char *name;
  InternalState *state;
  UT_hash_handle hh;
} InternalStateMap;

/**
 * @brief Tracks the current state machine context.
 * ...
 */
typedef struct {
  InternalStateMap *stateMap;
  const InternalState *currState;
  int stateCount;
  int fps;
  struct timespec lastTime;
} InternalTracker;


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

/**
 * @brief Retrieves a pointer to a State by name.
 * ...
 */
const InternalState *smInternalGetState(const char *name);

/**
 * @brief Retrieves a pointer to a StateMap entry by name.
 * ...
 */
InternalStateMap *smInternalGetEntry(const char *name);


#endif // #ifndef SMILE_STATE_MACHINE_INTERNAL_H
```

---

## ✉️ Message Files

* Message files contains definitions for messages to be logged to the user in
  case of operation success, warnings, errors, and fatal conditions (i.e. the
  program should not continue beyond that point).
* Sections in message files should have the following order and be named
  accordingly. Elements in certain sections should also be prefixed.

| Section Order | Section Element        | Section Name    | Prefix  |
|---------------|------------------------|-----------------|---------|
| 1             | File Description Block | [Not Named]     | [N/A]   |
| 2             | Include Guard          | [Not Named]     | [N/A]   |
| 3             | Module Name            | Module Name     | [N/A]   |
| 4             | Functions Names        | Functions Names | FN_     |
| 5             | Causes                 | Causes          | CAUSE_  |
| 6             | Consequences           | Consequences    | CONSEQ_ |

* The definition of the module's name should be called `MODULE`.
* Subsections for causes should follow the order below.

| Subsection Order | Name     |
|------------------|----------|
| 1                | Infos    |
| 2                | Warnings |
| 3                | Errors   |
| 4                | Fatals   |

* Subsections for Consequences should follow the order below.

| Subsection Order | Name    |
|------------------|---------|
| 1                | Success |
| 2                | Failure |

* The file `CommonInternalMessages.h` contains cross-module messages such as
  `FN_START` and `CAUSE_MODULE_STARTED`. It should always be checked before new
  messages are added to a given module.

✅ Example

```c
-- CommonInternalMessages.h
/**
 * @file
 * @brief Common definitions for shared log messages.
 * ...
 */


#ifndef SMILE_COMMON_MESSAGES_H
#define SMILE_COMMON_MESSAGES_H


// —————————————————————————————————————————————————————————————————————————————
// Functions Names
// —————————————————————————————————————————————————————————————————————————————

// Start Functions
#define FN_START  "Start"
...
// Stop Functions
#define FN_STOP  "Stop"


// —————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————

// Infos
#define CAUSE_MODULE_STARTED  "Module Started"
...
// Warnings
#define CAUSE_ALREADY_RUNNING  "Module Already Running"
// Errors
#define CAUSE_MEM_ALLOC_FAILED  "Memory Allocation Failed"
...


// —————————————————————————————————————————————————————————————————————————————
// Consequences
// —————————————————————————————————————————————————————————————————————————————

// Success
#define CONSEQ_SUCCESSFUL  "Successful"
// Failure
#define CONSEQ_ABORTED  "Aborted"


#endif // #ifndef SMILE_COMMON_MESSAGES_H


-- StateMachineMessages.h
/**
 * @file
 * @brief Message definitions for the StateMachine module.
 * ...
 */


#ifndef SMILE_STATE_MACHINE_MESSAGES_H
#define SMILE_STATE_MACHINE_MESSAGES_H


// —————————————————————————————————————————————————————————————————————————————
// Module Name
// —————————————————————————————————————————————————————————————————————————————

#define MODULE "StateMachine"


// —————————————————————————————————————————————————————————————————————————————
// Functions Names
// —————————————————————————————————————————————————————————————————————————————

// State Functions
#define FN_CREATE_STATE "CreateState"
...
// Lifecycle Functions
#define FN_UPDATE "Update"
...


// —————————————————————————————————————————————————————————————————————————————
// Causes
// —————————————————————————————————————————————————————————————————————————————

// Infos
#define CAUSE_STATE_CREATED "State Created"
...
// Warnings
#define CAUSE_STATE_ALREADY_EXISTS "State already exists"
...
// Errors
#define CAUSE_NULL_CURR_STATE "Current State Is Null"
...
// Fatals
#define CAUSE_FAILED_TO_FREE_ALL_STATES "Failed to Free All States"


#endif // #ifndef SMILE_STATE_MACHINE_MESSAGES_H
```

---

## 📤 Source Files

* logs

Log Message Pattern
Every log for errors, warnings, or info follows a structured pattern:
logLevel, moduleName, cause, optionalArgument, functionName, consequence

static void helperFunc(void) { // File scope function

**Example**

```c
```

<br>

* private funcs

Private Functions: [modulePrefix]Private[Description]

**Example**

```c
```

## 🧪 Test Files

* logs

Log Message Pattern
Every log for errors, warnings, or info follows a structured pattern:
logLevel, moduleName, cause, optionalArgument, functionName, consequence

static void helperFunc(void) { // File scope function

**Example**

```c
```

<br>

* private funcs

Private Functions: [modulePrefix]Private[Description]

**Example**

```c
```

<br>

For documenting Source Files, see [Documenting Source Files]() in
the [Documentation Guidelines]() doc.

## Up Next

[Code_Style](Code_Style.md)

## ✏️ Last Modified

| Last modified | Author (Discord)             | Description       |
|---------------|------------------------------|-------------------|
| Nov 07, 2025  | Vitor Betmann (vitorbetmann) | Created document; |