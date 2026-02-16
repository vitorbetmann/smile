# CommonInternal — API 🧰

`CommonInternal` aggregates internal data types and functions that can be shared
throughout all modules.

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#module-header)
- [Data Types](#-data-types)
    - [Function Pointers](#-function-pointers)
- [Functions](#-functions)
    - [Running Related](#-running-related)

## 😊Module Header

The module’s header is `CommonInternal.h`. Its full Smile path is:
`src/_Internal/Common/CommonInternal.h`

✅ Example

```c
#include "CommonInternal.h"
```

## 📦 Data Types

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
bool smStateExists(const char *name)
{
    if (!cmInternalIsRunning(smIsRunning, MODULE, FN_STATE_EXISTS))
    {
        return false;
    }

    ... 
}
```

## ✏️ Last Modified

| Last modified | Author (Discord)             | Description  |
|---------------|------------------------------|--------------|
| Dec 30, 2025  | Vitor Betmann (vitorbetmann) | Created doc; |