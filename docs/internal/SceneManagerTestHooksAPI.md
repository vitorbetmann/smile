# SceneManagerTestHooks — API 🎬

`SceneManagerTestHooks` exposes test-only interceptors and mock state for the
SceneManager module. It is only compiled under `SMILE_DEV`.

For public API see: [SceneManager – API](../SceneManager/SceneManagerAPI.md)

For the broader internal API see: [SceneManagerInternal – API](SceneManagerInternalAPI.md)

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#-module-header)
- [Data Types](#-data-types)
    - [Structs](#-structs)
    - [Function Pointers](#-function-pointers)
- [Variables](#-variables)

---

## 😊 Module Header

The module's header is `SceneManagerTestHooks.h`. Its full Smile path is:
`src/SceneManager/SceneManagerTestHooks.h`

Only available when `SMILE_DEV` is defined.

✅ Example

```c
#ifdef SMILE_DEV
#include "SceneManagerTestHooks.h"
#endif
```

---

## 📦 Data Types

### — Structs

| `MockData` |
|------------|

Holds invocation counters shared across all test hook callbacks.

| Field        | Type  | Summary                                             |
|--------------|-------|-----------------------------------------------------|
| `enterCount` | `int` | Incremented each time a scene enter callback fires. |
| `exitCount`  | `int` | Incremented each time a scene exit callback fires.  |

<br>

| `MockArgs` |
|------------|

Carries arbitrary test arguments into enter callbacks.

| Field  | Type   | Summary                                            |
|--------|--------|----------------------------------------------------|
| `flag` | `bool` | Arbitrary boolean carried into the enter callback. |

<br>

### — Function Pointers

| `void (*smTestEnterFn)(MockData *data)` |
|-----------------------------------------|

Function pointer type for intercepting enter callbacks that take no user arguments.

- Parameters:
    - `data` — Shared counter struct; typically increment `data->enterCount`.

<br>

| `void (*smTestEnterWithArgsFn)(MockData *data, MockArgs *args)` |
|-----------------------------------------------------------------|

Function pointer type for intercepting enter callbacks that receive a `MockArgs *`.

- Parameters:
    - `data` — Shared counter struct.
    - `args` — Mock arguments forwarded from `smMockArgs`.

<br>

| `void (*smTestExitFn)(MockData *data)` |
|----------------------------------------|

Function pointer type for intercepting exit callbacks.

- Parameters:
    - `data` — Shared counter struct; typically increment `data->exitCount`.

<br>

---

## 📐 Variables

| Name                      | Type                    | Summary                                                              |
|---------------------------|-------------------------|----------------------------------------------------------------------|
| `smTestEnter`             | `smTestEnterFn`         | Interceptor for enter callbacks that take no args.                   |
| `smTestEnterWithArgs`     | `smTestEnterWithArgsFn` | Interceptor for enter callbacks that receive a `MockArgs *`.         |
| `smTestExit`              | `smTestExitFn`          | Interceptor for exit callbacks.                                      |
| `smMockArgs`              | `MockArgs *`            | Arguments pointer forwarded to `smTestEnterWithArgs`.                |
| `smMockData`              | `MockData *`            | Shared counter struct threaded through all three interceptors.       |
| `smMockCurrTime`          | `struct timespec`       | Overrides the real clock in `smGetDt()` to simulate elapsed time.   |
| `smMockClockGettimeFails` | `bool`                  | When `true`, forces `smGetDt()` to return `RES_CLOCK_GETTIME_FAIL`. |

✅ Example

```c
// Callbacks used as test hooks.
static void onEnter(MockData *data) { data->enterCount++; }
static void onExit(MockData *data)  { data->exitCount++;  }

// Install hooks and shared state before the operation under test.
MockData mockData = {0};
smTestEnter = onEnter;
smTestExit  = onExit;
smMockData  = &mockData;

smSetScene("myScene", nullptr);

assert(mockData.enterCount == 1);

smStop();

assert(mockData.exitCount == 1);

// Reset all hooks after the test.
smTestEnter = nullptr;
smTestExit  = nullptr;
smMockData  = nullptr;
```