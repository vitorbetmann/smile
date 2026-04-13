# Common — API 🧰

`Common` aggregates internal data types and functions that can be shared
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
    - [Filesystem Related](#-filesystem-related)

--

## 😊Module Header

The module’s header is `Common.h`. Its full Smile path is:
`src/internal/Common/Common.h`

✅ Example

```c
#include "Common.h"
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
| `CM_RESULT_INVALID_PATH`         | `-7`  | Path is absolute, too long, or contains `..` segments.    |
| `CM_RESULT_INVALID_NAME`         | `-8`  | Name contains invalid characters or exceeds the limit.    |
| `CM_RESULT_FILE_NOT_FOUND`       | `-9`  | File does not exist at the specified path.                |
| `CM_RESULT_FAIL_TO_CREATE_DIR`   | `-10` | Directory creation failed.                                |
| `CM_RESULT_FAIL_TO_DELETE_FILE`  | `-11` | File exists but could not be deleted.                     |
| `CM_RESULT_FAIL_TO_CREATE_FILE`  | `-12` | File could not be created or written.                     |
| `CM_RESULT_DIR_NOT_FOUND`        | `-13` | Directory does not exist at the specified path.           |
| `CM_RESULT_FAIL_TO_DELETE_DIR`   | `-14` | Directory exists but could not be deleted.                |

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

### — Filesystem Related

| `bool cmInternalDirExists(const char *path)` |
|----------------------------------------------|

Checks whether a directory exists at the given path.

- Parameters:
    - `path` — The directory path to check.
- Returns: `true` if a directory exists at `path`, `false` otherwise.

<br>

| `int cmInternalValidatePath(const char *path)` |
|------------------------------------------------|

Validates that a path is safe and within acceptable bounds. Rejects absolute
paths, paths exceeding `CM_PATH_MAX`, and paths containing bare `..` segments
to prevent directory traversal.

- Parameters:
    - `path` — The path string to validate.
- Returns: `CM_RESULT_OK` on success, or a negative error code on failure.
- Notes:
    - Fails with `CM_RESULT_NULL_ARG` if `path` is NULL.
    - Fails with `CM_RESULT_EMPTY_ARG` if `path` is empty.
    - Fails with `CM_RESULT_INVALID_PATH` if the path is absolute, exceeds
      `CM_PATH_MAX` characters, or contains a bare `..` segment.

<br>

| `int cmInternalCreateDir(const char *path)` |
|---------------------------------------------|

Recursively creates a directory and all necessary parent directories. Validates
the path via `cmInternalValidatePath`, then creates each missing path segment
in order. Treats an already-existing directory as success.

- Parameters:
    - `path` — Relative path of the directory to create.
- Returns: `CM_RESULT_OK` on success, or a negative error code on failure.
- Notes:
    - Fails with any code returned by `cmInternalValidatePath`.
    - Fails with `CM_RESULT_FAIL_TO_CREATE_DIR` if a directory segment cannot
      be created.

<br>

| `bool cmInternalFileExists(const char *filename)` |
|---------------------------------------------------|

Checks whether a file exists and is readable at the given path.

- Parameters:
    - `filename` — The file path to check.
- Returns: `true` if the file exists and can be opened for reading,
  `false` otherwise.

<br>

| `int cmInternalDeleteFile(const char *path)` |
|----------------------------------------------|

Deletes a file at the specified path. Validates the path, confirms the file
exists, then removes it.

- Parameters:
    - `path` — Relative path of the file to delete.
- Returns: `CM_RESULT_OK` on success, or a negative error code on failure.
- Notes:
    - Fails with any code returned by `cmInternalValidatePath`.
    - Fails with `CM_RESULT_FILE_NOT_FOUND` if the file does not exist.
    - Fails with `CM_RESULT_FAIL_TO_DELETE_FILE` if deletion fails.
    - Side effects: permanently removes the file from the filesystem.

<br>

| `int cmInternalDeleteDir(const char *path)` |
|---------------------------------------------|

Deletes an empty directory at the specified path. Validates the path, confirms
the directory exists, then removes it.

- Parameters:
    - `path` — Relative path of the directory to delete.
- Returns: `CM_RESULT_OK` on success, or a negative error code on failure.
- Notes:
    - Fails with any code returned by `cmInternalValidatePath`.
    - Fails with `CM_RESULT_DIR_NOT_FOUND` if the directory does not exist.
    - Fails with `CM_RESULT_FAIL_TO_DELETE_DIR` if deletion fails.
    - The directory must be empty; non-empty directories will fail.
    - Side effects: permanently removes the directory from the filesystem.
