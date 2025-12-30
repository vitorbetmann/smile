# Coding Guidelines — Code Organization 🗃️

This document defines Smile’s standard conventions for organizing code and
code-related files.

## Table of Contents

- [General Guidelines](#-general-guidelines)
- [Header Files](#-non-test-header-files)
    - [Public Headers](#-public-headers)
    - [Internal Headers](#-internal-headers)
- [Message Files](#-message-files)
- [Source Files](#-non-test-source-files)

## 🗂 General Guidelines

### — Section Headers

* All code files must be organized into sections, each marked by a standardized
  comment block, referred to as a section header.
    * Section headers open and close with a `//` and a space and are followed by
      77 `—`. This makes each line 80 characters long.
    * In between the opening and closing comments, comes the section name after
      `//` and a space.
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

See [Log.h](../../../../include/Log.h).

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

## 😶 Non-Test Header Files

* All headers must have `include guards`.
* Should never include variables (see Smile's Golden Rule
  in [Code_Style](Code_Style.md)).
* See [4_Testing_Guidelines] (🚧 Under Development) for details on implementing a
  test header file.

### — Public Headers

* Contains declarations of functions and data types of user-facing APIs.
* See [Code_Style](Code_Style.md) for details on naming and declaring functions
  and data types.
* See [3_Documentation_Guidelines] (🚧 Under Development) for details on
  documenting.

✅ Example

See [StateMachine.h](../../../../include/StateMachine.h).

---

### — Internal Headers

* Contains declarations of Internal functions and data types. They should help
  the module in executing its tasks but are not meant for public use.
    * For example, `smInternalGetState()` allows StateMachine to directly call
      the `enter` and `exit` functions of a state, but a user should never have
      access to that. Instead, they can call functions like `smSetState()` and
      `smUpdate()` which automatically handles memory allocation, pointer
      manipulation, and the calling of desired functions.
* Though the code in Internal Header files is not to be shared with other
  modules (e.g., StateMachineInternal.h should only be included in
  StateMachine.c), Internal files exist for the sake of organization. Otherwise,
  if all functions, data types, and variables were to be declared and defined in
  a module's source file, it would grow unnecessarily large and complex.
* See [Code_Style](Code_Style.md) for details on naming and declaring Internal
  functions and data types.
* See [3_Documentation_Guidelines] (🚧 Under Development) for details on
  documenting.

✅ Example

* See
  [StateMachineInternal.h](../../../../src/StateMachine/StateMachineInternal.h).

## ✉️ Message Files

* Contain definitions for messages to be logged to the user in case of operation
  success, warnings, errors, and fatal conditions (i.e. the program should not
  continue beyond that point).
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

✅ Examples

* See
  [CommonInternalMessages.h](../../../../src/_Internal/_Common/CommonInternalMessages.h).

* See
  [StateMachineMessages.h](../../../../src/StateMachine/StateMachineMessages.h).

## 📤 Non-Test Source Files

* Contain the implementation of both Public and Internal header files.
* File-scoped variables and functions are declared here.
    * File-scoped functions are known as Private.
* See [Code_Style](Code_Style.md) for more details on file-scoped variables and
  Private Functions.
* See [4_Testing_Guidelines] (🚧 Under Development) for details on implementing a
  test source file.

✅ Example

* See [StateMachine.c](../../../../src/StateMachine/StateMachine.c).

## Up Next

Learn the standards for [Code_Style](Code_Style.md).

## ✏️ Last Modified

| Last modified | Author (Discord)             | Description       |
|---------------|------------------------------|-------------------|
| Nov 07, 2025  | Vitor Betmann (vitorbetmann) | Created document; |