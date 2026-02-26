# CommonInternal — API 🧰

`CommonInternal` aggregates internal data types and functions that can be shared
throughout all modules.

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#module-header)
- [Data Types](#-data-types)
    - [Enums](#-enums)
    - [Function Pointers](#-function-pointers)
- [Functions](#-functions)
    - [Running Related](#-running-related)

--

## 😊Module Header

The module’s header is `CommonInternal.h`. Its full Smile path is:
`src/_Internal/Common/CommonInternal.h`

✅ Example

```c
#include "CommonInternal.h"
```

---

## 📦 Data Types

### — Enums

| `cmResult` |
|------------|

Common result codes shared across Smile modules.

- The following range is exclusive to Common failures: `-1..-99`.

| Item                             | Value | Summary                                                   |
|----------------------------------|-------|-----------------------------------------------------------|
| `CM_RESULT_OK`                   | `0`   | Operation completed successfully.                         |
| `CM_RESULT_ALREADY_RUNNING`      | `-1`  | Module is already running when start/init is requested.   |
| `CM_RESULT_NOT_RUNNING`          | `-2`  | Module is not running when an operation requires it.      |
| `CM_RESULT_MEM_ALLOC_FAILED`     | `-3`  | Memory allocation failed.                                 |
| `CM_RESULT_NULL_ARG`             | `-4`  | A required pointer argument is null.                      |
| `CM_RESULT_EMPTY_ARG`            | `-5`  | A required string argument is empty.                      |
| `CM_RESULT_CLOCK_GETTIME_FAILED` | `-6`  | System clock query failed (for example, `clock_gettime`). |

<br>

### — Function Pointers

| `bool (*cmIsRunningFn)(void)` |
|-------------------------------|

Function pointer used to check whether a module is currently running.

✅ Example

```c
bool smIsRunning(void)
{
    return tracker;
}
```

---

## 🛠️ Functions

### — Running Related

| `bool cmInternalIsRunning(cmIsRunningFn cmIsRunning, const char *module, const char *fnName)` |
|-----------------------------------------------------------------------------------------------|

Guard function that verifies that a module is running before a public API
continues, logging an error if it is not.

- Parameters:
    - `cmIsRunning` — Callback that checks whether the module is running.
    - `module` — Name of the module performing the check (for logging).
    - `fnName` — Name of the calling function (for logging).
- Returns: `true` if the module is running, false otherwise.

✅ Example

```c
bool smSceneExists(const char *name)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_SCENE_EXISTS))
    {
        return false;
    }

    ... 
}
```

---

## ✏️ Last Modified

| Last modified | Author (Discord)             | Description                            |
|---------------|------------------------------|----------------------------------------|
| Feb 26, 2026  | Vitor Betmann (vitorbetmann) | Added `cmResult` enum and value table. |
