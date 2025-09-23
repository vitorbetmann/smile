# TestInternal API

`TestInternal` provides instrumented memory allocation wrappers and fatal hooks for SMILE.  
These functions can be used in production for safe allocations and logging, and in unit tests to simulate failures.

---

## Table of Contents

- [Types](#types)
- [Functions](#functions)
    - [TEST_Pass](#test_pass)
    - [TEST_Disable](#test_disable)
    - [TEST_Malloc](#test_malloc)
    - [TEST_Calloc](#test_calloc)
    - [TEST_Realloc](#test_realloc)
    - [TEST_Fatal](#test_fatal)

---

## Types

### `MemAllocFunc`

Identifiers for allocation functions that can be instrumented or simulated. Used with `TEST_Disable` to specify which
allocation function should fail.

| Enum      | Description                                        |
|-----------|----------------------------------------------------|
| `MALLOC`  | malloc() allocation                                |
| `CALLOC`  | calloc() allocation                                |
| `REALLOC` | realloc() allocation (supports failure simulation) |

---

## Functions

### `void TEST_Pass(const char *funcName)`

Logs a `[PASS]` message for a successful test or operation.

- **Parameters:**
    - `funcName` — Name of the test function or operation that passed.
- **Returns:** Nothing.

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

---

### `void *TEST_Malloc(size_t size)`

Wrapper around `malloc()` with optional failure simulation.

- **Parameters:**
    - `size` — Number of bytes to allocate.
- **Returns:** Pointer to allocated memory, or `NULL` if failure is simulated.

---

### `void *TEST_Calloc(size_t nitems, size_t size)`

Wrapper around `calloc()` with optional failure simulation.

- **Parameters:**
    - `nitems` — Number of elements to allocate.
    - `size` — Size of each element in bytes.
- **Returns:** Pointer to allocated memory, or `NULL` if failure is simulated.

---

### `void *TEST_Realloc(void *ptr, size_t size)`

Wrapper around `realloc()` with optional failure simulation.

- **Parameters:**
    - `ptr` — Pointer to an existing memory block.
    - `size` — Number of bytes to allocate.
- **Returns:** Pointer to reallocated memory, or `NULL` if failure is simulated.

---

### `bool TEST_Fatal(void)`

Check whether a fatal condition is being simulated.

- **Returns:**
    - `true` if a fatal condition is active.
    - `false` otherwise.

---

## Notes

- Internal counters track which allocation call should fail.
- Functions are safe for **production use** for logging or controlled allocation.
- Designed for single-threaded environments.
- Use `TEST_Disable` to test error handling paths reliably.  