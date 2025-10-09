# TestInternal API 🧪

`TestInternal` provides instrumented memory allocation wrappers and fatal hooks
for SMILE.  
These functions can be used in production for safe allocations and logging, and
in unit tests to simulate failures.

For testing guidelines, see
the [Testing Contributing Doc](../_Contributing/3_Testing.md).

### 🚨 Warning! This module is not thread-safe

---

## Table of Contents

- [Data Types](#-data-types)
    - [MemAllocFn](#memallocfn)
- [Functions](#-functions)
    - [Test Suites Related:](#_test-suites-related_)
        - [tsInternalPass](#void-tsinternalpassconst-char-fnname)
        - [tsInternalDisable](#bool-tsinternaldisablememallocfn-fnname-unsigned-int-at)
    - [Memory Allocation Related:](#_memory-allocation-related_)
        - [tsInternalMalloc](#void-tsinternalmallocsize_t-size)
        - [tsInternalCalloc](#void-tsinternalcallocsize_t-nitems-size_t-size)
        - [tsInternalRealloc](#void-tsinternalreallocvoid-ptr-size_t-size)
- [Workflow Examples](#-workflow-examples)

---

## 📦 Data Types

---

### _Enums_

---

### `MemAllocFn`

Identifies allocation functions for failure simulation.

Used
with [tsInternalDisable](#bool-tsinternaldisablememallocfn-fnname-unsigned-int-at)
to specify which type of allocation should be forced to fail.

| Enum      | Simulates |
|-----------|-----------|
| `MALLOC`  | malloc()  |
| `CALLOC`  | calloc()  |
| `REALLOC` | realloc() |

---

## 🔧 Functions

---

### _Test Suites Related_

---

### `void tsInternalPass(const char *fnName)`

Logs a `[PASS]` message for a successful test or operation.

- **Parameters:**
    - `fnName` — Name of the test function or operation that passed.

**Example:**

```c
NO EXAMPLE YET
```

For more, see [Workflow Examples](#-workflow-examples).

---

### `bool tsInternalDisable(MemAllocFn fnName, unsigned int at)`

Temporarily disables a memory allocation function, causing it to fail at the
specified call count. After the failure
occurs, normal behavior resumes.

- **Parameters:**
    - `fnName` — Allocation function to disable (`MALLOC`, `CALLOC`, `REALLOC`).
    - `at` — Call count at which failure occurs.
- **Returns:**
    - `true` if successfully disabled.
    - `false` if an invalid function type is given.

**Example:**

```c
NO EXAMPLE YET
```

For more, see [Workflow Examples](#-workflow-examples).

---

### _Memory Allocation Related:_

---

### `void *tsInternalMalloc(size_t size)`

Wrapper around `malloc()` with optional failure simulation.

- **Parameters:**
    - `size` — Number of bytes to allocate.
- **Returns:** Pointer to allocated memory, or `nullptr` if failure is
  simulated.

**Example:**

```c
NO EXAMPLE YET
```

For more, see [Workflow Examples](#-workflow-examples).

---

### `void *tsInternalCalloc(size_t nitems, size_t size)`

Wrapper around `calloc()` with optional failure simulation.

- **Parameters:**
    - `nitems` — Number of elements to allocate.
    - `size` — Size of each element in bytes.
- **Returns:** Pointer to allocated memory, or `nullptr` if failure is
  simulated.

**Example:**

```c
NO EXAMPLE YET
```

For more, see [Workflow Examples](#-workflow-examples).

---

### `void *tsInternalRealloc(void *ptr, size_t size)`

Wrapper around `realloc()` with optional failure simulation.

- **Parameters:**
    - `ptr` — Pointer to an existing memory block.
    - `size` — Number of bytes to allocate.
- **Returns:** Pointer to reallocated memory, or `nullptr` if failure is
  simulated.

**Example:**

```c
// NO EXAMPLES YET
```

For more, see [Workflow Examples](#-workflow-examples).

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

For more, see [Workflow Examples](#-workflow-examples).

---

## 📖 Workflow Examples

```c
// NO EXAMPLES YET
```