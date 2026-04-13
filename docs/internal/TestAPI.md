<!-- TODO #19 [Docs][Test] Add example for tsRealloc wrapper function -->

# Test — API 🧪

`Test` provides instrumented memory allocation wrappers and fatal hooks
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
    - [Allocation and I/O Related](#-allocation-and-io-related)

---

## 😊Module Header

The module’s header is `Test.h`. Its full Smile path is:
`src/internal/Test/Test.h`

✅ Example

```c
#include "Test.h"
```

---

## 📦 Data Types

### — Enums

| `tsSysFn` |
|-----------|

Identifies system functions for failure simulation. Used with `tsDisable`
to specify which function should be forced to fail on a given call count.

| Item      | Simulates |
|-----------|-----------|
| `MALLOC`  | malloc()  |
| `CALLOC`  | calloc()  |
| `REALLOC` | realloc() |
| `FOPEN`   | fopen()   |
| `MKDIR`   | mkdir()   |

✅ Example

```c
void Test_smStart_FailsIfCallocFails(void)
{
    tsDisable(CALLOC, 1);
    assert(!smStart());
    tsPass("Test_smStart_FailsIfCallocFails");
}
```

---

## 🔧 Functions

### — Test Suites Related

| `void tsPass(const char *fnName)` |
|-----------------------------------|

Logs a `[PASS]` message for a successful test or operation.

- Parameters:
    - `fnName` — Name of the test function or operation that passed.

✅ Example

```c
void Test_smHasStarted_FailsPreStart(void)
{
    assert(!smHasStarted());
    tsPass("Test_smHasStarted_FailsPreStart");
}
```

<br>

| `bool tsDisable(tsSysFn fnName, unsigned int at)` |
|---------------------------------------------------|

Temporarily disables a system function, causing it to fail at the specified
call count. After the failure occurs, normal behaviour resumes.

- Parameters:
    - `fnName` — Function to disable (`MALLOC`, `CALLOC`, `REALLOC`, `FOPEN`, `MKDIR`).
    - `at` — Call count at which failure occurs.
- Returns: `true` if successfully disabled, `false` if an invalid function
  type is given or if `at` is `0`.

✅ Example

```c
void Test_smStart_FailsIfCallocFails(void)
{
    tsDisable(CALLOC, 1);
    assert(!smStart());
    tsPass("Test_smStart_FailsIfCallocFails");
}
```

<br>

| `void tsReset(void)` |
|----------------------|

Resets all failure simulation state to its defaults — no failures scheduled,
all system function wrappers pass through normally. Call this at the start of
any test that uses `tsDisable` to guarantee a clean slate, regardless
of what a previous test may have left behind.

> **Note:** `tsDisable` already auto-resets when the simulated failure
> fires, so `tsReset` is only necessary as a defensive guard at the
> top of each test that schedules a failure.

✅ Example

```c
void Test_smStart_FailsIfCallocFails(void)
{
    tsReset();
    tsDisable(CALLOC, 1);
    assert(!smStart());
    tsPass(__func__);
}
```

<br>

### — Allocation and I/O Related

| `void *tsMalloc(size_t size)` |
|-------------------------------|

Wrapper around `malloc()` with optional failure simulation.

- Parameters:
    - `size` — Number of bytes to allocate.
- Returns: Pointer to allocated memory, or `nullptr` if failure is
  simulated.

✅ Example

```c
smInternalScene *scene = tsMalloc(sizeof(smInternalScene));
if (!scene)
{
    lgInternalLog(ERROR, MODULE, CSE_MEM_ALLOC_FAIL, __func__, CSQ_ABORT);
    return false;
}
```

<br>

| `void *tsCalloc(size_t nitems, size_t size)` |
|----------------------------------------------|

Wrapper around `calloc()` with optional failure simulation.

- Parameters:
    - `nitems` — Number of elements to allocate.
    - `size` — Size of each element in bytes.
- Returns: Pointer to allocated memory, or `nullptr` if failure is
  simulated.

✅ Example

```c
tracker = tsCalloc(1, sizeof(SceneManagerTracker));
if (!tracker)
{
    lgInternalLog(ERROR, MODULE, CSE_MEM_ALLOC_FAIL, __func__, CSQ_ABORT);
    return false;
}
```

<br>

| `void *tsRealloc(void *ptr, size_t size)` |
|-------------------------------------------|

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

<br>

| `FILE *tsFopen(const char *path, const char *mode)` |
|-----------------------------------------------------|

Wrapper around `fopen()` with optional failure simulation.

- Parameters:
    - `path` — Path of the file to open.
    - `mode` — Mode string passed to `fopen`.
- Returns: `FILE` pointer, or `nullptr` if failure is simulated.

✅ Example

```c
tsDisable(FOPEN, 1);
assert(gsInternalRun(argc, argv) == RES_CREATE_FILE_FAIL);
tsPass("Test_gsInternalRun_FailsIfFopenFails");
```

<br>

| `int tsMkdir(const char *path)` |
|---------------------------------|

Wrapper around `mkdir()` with optional failure simulation.

- Parameters:
    - `path` — Path of the directory to create.
- Returns: `0` on success, `-1` on failure (real or simulated).

✅ Example

```c
tsDisable(MKDIR, 1);
assert(gsInternalRun(argc, argv) == RES_CREATE_DIR_FAIL);
tsPass("Test_gsInternalRun_FailsIfMkdirFails");
```

<br>

| `char *tsMkdtemp(char *tmpl)` |
|-------------------------------|

Portable wrapper around `mkdtemp()`. Creates a unique temporary directory by
replacing the trailing `"XXXXXX"` in `tmpl` with a unique suffix, then creating
the directory. On POSIX, delegates to `mkdtemp`; on Windows, uses `_mktemp` +
`_mkdir`.

- Parameters:
    - `tmpl` — Template string ending in `"XXXXXX"`, modified in-place.
- Returns: Pointer to `tmpl` on success, `nullptr` on failure.

> **Note:** `tsMkdtemp` calls the platform's directory creation
> primitive directly and does **not** go through `tsMkdir`.
> `tsDisable(MKDIR, n)` has no effect on temp directory creation.
> This is intentional — temp dirs are test infrastructure, not code under
> test, so failure simulation should not interfere with them.

✅ Example

```c
char dir[] = "gstest_src_XXXXXX";
assert(tsMkdtemp(dir) != nullptr);
// dir is now e.g. "gstest_src_a01234" and the directory exists
```
