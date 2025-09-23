# TestInternal API

`TestInternal` provides instrumented memory allocation wrappers and fatal hooks for SMILE.  
These functions can be used in production for safe allocations and logging, and in unit tests to simulate failures.

---

## Table of Contents

- [Data Types](#data-types)
- [Functions](#functions)
- [Workflow Examples](#workflow-examples)

---

---

## Data Types

### `MemAllocFunc` (enum)

Specifies which memory allocation function should be simulated using `TEST_Disable`.

| Enum      | Simulates |
|-----------|-----------|
| `MALLOC`  | malloc()  |
| `CALLOC`  | calloc()  |
| `REALLOC` | realloc() |

---

## Functions

### `void TEST_Pass(const char *funcName)`

Logs a `[PASS]` message for a successful test or operation.

- **Parameters:**
    - `funcName` — Name of the test function or operation that passed.
- **Returns:** Nothing.

**Example:**

```c
void Test_SL_Init_ReturnsTrue(void) {
  assert(SL_Init());
  TEST_Pass("Test_SL_Init_ReturnsTrue");
}
```

For more, see [Workflow Examples](#workflow-examples).

---

### `bool TEST_Disable(MemAllocFunc funcName, int at)`

Temporarily disables a memory allocation function, causing it to fail at the specified call count. After the failure
occurs, normal behavior resumes.

- **Parameters:**
    - `funcName` — Allocation function to disable (`MALLOC`, `CALLOC`, `REALLOC`).
    - `at` — Call count at which failure occurs.
- **Returns:**
    - `true` if successfully disabled.
    - `false` if an invalid function type is given.

**Example:**

```c
void Test_SL_Init_ReturnsFalseIfMallocFails(void) {
  TEST_Disable(MALLOC, 1);
  assert(!SL_Init());
  TEST_Pass("Test_SL_Init_ReturnsFalseIfMallocFails");
}
```

For more, see [Workflow Examples](#workflow-examples).

---

### `void *TEST_Malloc(size_t size)`

Wrapper around `malloc()` with optional failure simulation.

- **Parameters:**
    - `size` — Number of bytes to allocate.
- **Returns:** Pointer to allocated memory, or `NULL` if failure is simulated.

**Example:**

```c
tracker = TEST_Malloc(sizeof(StateMachineTracker));
if (!tracker) {
    SMILE_ERR(MODULE_NAME, CAUSE_MEM_ALLOC_FAILED, CONSEQ_INIT_ABORTED);
    return false;
}
```

For more, see [Workflow Examples](#workflow-examples).

---

### `void *TEST_Calloc(size_t nitems, size_t size)`

Wrapper around `calloc()` with optional failure simulation.

- **Parameters:**
    - `nitems` — Number of elements to allocate.
    - `size` — Size of each element in bytes.
- **Returns:** Pointer to allocated memory, or `NULL` if failure is simulated.

**Example:**

```c
tracker = TEST_Calloc(1, sizeof(SaveLoadTracker));
if (!tracker) {
    SMILE_ERR(MODULE, CAUSE_MEM_ALLOC_FAILED, CONSEQ_INIT_ABORTED);
    return false;
}
```

For more, see [Workflow Examples](#workflow-examples).

---

### `void *TEST_Realloc(void *ptr, size_t size)`

Wrapper around `realloc()` with optional failure simulation.

- **Parameters:**
    - `ptr` — Pointer to an existing memory block.
    - `size` — Number of bytes to allocate.
- **Returns:** Pointer to reallocated memory, or `NULL` if failure is simulated.

**Example:**

```c
// NO EXAMPLES YET
```

For more, see [Workflow Examples](#workflow-examples).

---

### `bool TEST_Fatal(void)`

Check whether a fatal condition is being simulated.

- **Returns:**
    - `true` if a fatal condition is active.
    - `false` otherwise.

**Example:**

```c
// NO EXAMPLES YET
```

For more, see [Workflow Examples](#workflow-examples).

---

## Workflow Examples

```c
// NO EXAMPLES YET
```

---

## Notes

- Designed for single-threaded environments.
