# LogInternal — API 📝

`LogInternal` provides a logging interface for Smile modules. It contains
internal logging functionality and severity levels for warnings, errors, and
fatal events inside Smile core modules.

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Module Header](#module-header)
- [Data Types](#-data-types)
    - [Enums](#-enums)
- [Functions](#-functions)
    - [Log Related](#-log-related)

## 😊Module Header

The module's header is `LogInternal.h`. Its full Smile path is:
`src/Log/LogInternal.h`

✅ Example

```c
#include "LogInternal.h"
```

## 📦 Data Types

### — Enums

| `lgInternalResult` |
|--------------------|

Log-specific result codes for failures not covered by
[cmResult](CommonAPI.md#-enums).

- Log uses the range `-100..-199` for its module-specific codes.

| Item             | Value  | Summary                             |
|------------------|--------|-------------------------------------|
| `RES_WRITE_FAIL` | `-100` | Logging output write/flush failed.  |
| `RES_TIME_FAIL`  | `-101` | Time acquisition/formatting failed. |

<br>

| `lgInternalLevel` |
|-------------------|

Severity levels for internal Smile logging. Represents the importance of a log
message, ranging from user-defined messages to fatal errors.

| Item    | Summary                                                                         | Color  |
|---------|---------------------------------------------------------------------------------|--------|
| `USER`  | Custom user-generated log. See lgLog() in [Log API](../Log/LogAPI.md).          | Green  |
| `INFO`  | Reports informational Smile events (e.g., start successful).                    | Blue   |
| `WARN`  | Reports non-critical unexpected events (e.g., module started twice).            | Yellow |
| `ERROR` | Reports game-breaking errors (e.g., memory allocation failure).                 | Red    |
| `FATAL` | Reports program-breaking errors (e.g., fatal cleanup failure).                  | Purple |

✅ Example

```c
lgInternalLog(ERROR, ORI, CSE_NOT_RUNNING, fnName, CSQ_ABORT);
```

## 🛠️ Functions

### — Log Related

| `int lgInternalLog(lgInternalLevel lvl, const char *ori, const char *cse, const char *caller, const char *csq)` |
|-----------------------------------------------------------------------------------------------------------------|

Logs an internal Smile event with module, cause, and consequence context.

- Parameters:
    - `lvl` — Severity level (INFO, WARN, ERROR, FATAL).
    - `ori` — Name of the module generating the log.
    - `cse` — Cause of the log event.
    - `caller` — Caller identifier — pass `__func__`, or tool name for
      top-level tool logs.
    - `csq` — Consequence or follow-up description.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Common failures use `cmResult` (for example, `RES_NULL_ARG`).
    - Log-specific failures use `lgInternalResult`:
      `RES_TIME_FAIL`, `RES_WRITE_FAIL`.
    - If `lvl` is `FATAL`, the configured fatal handler is invoked after
      logging.

✅ Example

```c
bool smAddScene(const char *name, smEnterFn enter, smUpdateFn update, smDrawFn draw, smExitFn exit)
{
    if (!cmIsRunning(smIsRunning, ORI, __func__))
        return RES_NOT_RUNNING;

    ...
}
```

<br>

| `int lgInternalLogWithArg(lgInternalLevel lvl, const char *ori, const char *cse, const char *arg, const char *caller, const char *csq)` |
|-----------------------------------------------------------------------------------------------------------------------------------------|

Logs an internal Smile event with an additional context argument.

Identical to `lgInternalLog()` but includes an extra string for context
(for example, the name of the offending argument).

- Parameters:
    - `lvl` — Severity level (INFO, WARN, ERROR, FATAL).
    - `ori` — Name of the module generating the log.
    - `cse` — Cause of the log event.
    - `arg` — Additional context string relevant to the event.
    - `caller` — Caller identifier — pass `__func__`, or tool name for
      top-level tool logs.
    - `csq` — Consequence or follow-up description.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Common failures use `cmResult` (for example, `RES_NULL_ARG`).
    - Log-specific failures use `lgInternalResult`:
      `RES_TIME_FAIL`, `RES_WRITE_FAIL`.
    - If `lvl` is `FATAL`, the configured fatal handler is invoked after
      attempting to log.

✅ Example

```c
bool smPrivateIsNameValid(const char *name, const char *fnName)
{
    if (!name)
    {
        lgInternalLogWithArg(ERROR, ORI, CSE_NULL_ARG, "name", fnName, CSQ_ABORT);
        return false;
    }

    ...
}
```
