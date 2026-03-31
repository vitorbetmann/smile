<!-- TODO #19 [Docs][TestInternal] Add example for tsInternalRealloc wrapper function -->

# TestInternal — API 🧪

`TestInternal` provides instrumented memory allocation wrappers and fatal hooks
for SMILE. These functions can be used in production for safe allocations and
logging, and in unit tests to simulate failures.

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#module-header)
- [Data Types](#-data-types)
    - [Enums](#-enums)
- [Functions](#-functions)
    - [Test Suites Related](#-test-suites-related)
    - [Memory Allocation Related](#-memory-allocation-related)

---

## 😊Module Header

The module’s header is `TestInternal.h`. Its full Smile path is:
`src/_Internal/Test/TestInternal.h`

✅ Example

```c
#include "TestInternal.h"
```

---

## 📦 Data Types

### — Enums

| `MemAllocFn` |
|--------------|

Identifies allocation functions for failure simulation. Used
with `tsInternalDisable` to specify which type of allocation should be forced to
fail.

| Item      | Simulates |
|-----------|-----------|
| `MALLOC`  | malloc()  |
| `CALLOC`  | calloc()  |
| `REALLOC` | realloc() |

✅ Example

```c
void Test_smStart_FailsIfCallocFails(void)
{
    tsInternalDisable(CALLOC, 1);
    assert(!smStart());
    tsInternalPass("Test_smStart_FailsIfCallocFails");
}
```

---

## 🔧 Functions

### — Test Suites Related

| `void tsInternalPass(const char *fnName)` |
|-------------------------------------------|

Logs a `[PASS]` message for a successful test or operation.

- Parameters:
    - `fnName` — Name of the test function or operation that passed.

✅ Example

```c
void Test_smHasStarted_FailsPreStart(void)
{
    assert(!smHasStarted());
    tsInternalPass("Test_smHasStarted_FailsPreStart");
}
```

<br>

| `bool tsInternalDisable(MemAllocFn fnName, unsigned int at)` |
|--------------------------------------------------------------|

Temporarily disables a memory allocation function, causing it to fail at the
specified call count. After the failure
occurs, normal behaviour resumes.

- Parameters:
    - `fnName` — Allocation function to disable (`MALLOC`, `CALLOC`, `REALLOC`).
    - `at` — Call count at which failure occurs.
- Returns: `true` if successfully disabled, `false` if an invalid function
  type is given.

✅ Example

```c
void Test_smStart_FailsIfCallocFails(void)
{
    tsInternalDisable(CALLOC, 1);
    assert(!smStart());
    tsInternalPass("Test_smStart_FailsIfCallocFails");
}
```

<br>

### — Memory Allocation Related

| `void *tsInternalMalloc(size_t size)` |
|---------------------------------------|

Wrapper around `malloc()` with optional failure simulation.

- Parameters:
    - `size` — Number of bytes to allocate.
- Returns: Pointer to allocated memory, or `nullptr` if failure is
  simulated.

✅ Example

```c
State *state = tsInternalMalloc(sizeof(State));
if (!state)
{
    lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED,
                  FN_CREATE_SCENE, CONSEQ_ABORTED);
    return false;
}
```

<br>

| `void *tsInternalCalloc(size_t nitems, size_t size)` |
|------------------------------------------------------|

Wrapper around `calloc()` with optional failure simulation.

- Parameters:
    - `nitems` — Number of elements to allocate.
    - `size` — Size of each element in bytes.
- Returns: Pointer to allocated memory, or `nullptr` if failure is
  simulated.

✅ Example

```c
tracker = tsInternalCalloc(1, sizeof(SceneManagerTracker));
if (!tracker)
{
    lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                  CONSEQ_ABORTED);
    return false;
}
```

<br>

| `void *tsInternalRealloc(void *ptr, size_t size)` |
|---------------------------------------------------|

Wrapper around `realloc()` with optional failure simulation.

- Parameters:
    - `ptr` — Pointer to an existing memory block.
    - `size` — Number of bytes to allocate.

- Returns: Pointer to reallocated memory, or `nullptr` if failure is
  simulated.

✅ Example

```c
// NO EXAMPLE YET
```
