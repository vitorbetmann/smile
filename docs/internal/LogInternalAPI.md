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

The module’s header is `LogInternal.h`. Its full Smile path is:
`src/Log/LogInternal.h`

✅ Example

```c
#include "LogInternal.h"
```

## 📦 Data Types

### — Enums

| `lgInternalResult` |
|--------------------|

Log-specific result codes used when failures are not covered
by [cmResult](CommonAPI.md#-enums).

- Log-specific failures cover the following range: `-100..-199`.

| Item             | Value  | Summary                             |
|------------------|--------|-------------------------------------|
| `RES_WRITE_FAIL` | `-100` | Logging output write/flush failed.  |
| `RES_TIME_FAIL`  | `-101` | Time acquisition/formatting failed. |

<br>

| `lgInternalLevel` |
|-------------------|

LogInternal severity levels used by Smile. Represents the importance of a log
message, ranging from user-defined messages to fatal errors.

| Item    | Summary                                                                                              | Color  |
|---------|------------------------------------------------------------------------------------------------------|--------|
| `USER`  | For custom user generated logs. See lgLog in [Log_API](../Log/LogAPI.md).                            | Green  |
| `INFO`  | Reports informational logs. E.g. "Start Successful."                                                 | Blue   |
| `WARN`  | Reports non-game breaking or non-intentional events. E.g. calling the Start function twice in a row. | Yellow |
| `ERROR` | Reports game-breaking errors. E.g. memory allocation failed.                                         | Red    |
| `FATAL` | Reports program-breaking errors. E.g. fatal cleanup failures.                                        | Purple |

✅ Example

```c
lgInternalLog(ERROR, ORI, CSE_NOT_RUNNING, fnName, CSQ_ABORT);
```

## 🛠️ Functions

### — Log Related

| `int lgInternalLog(lgInternalLevel lvl, const char *ori, const char *cse, const char *caller, const char *csq)` |
|-----------------------------------------------------------------------------------------------------------------|

Used by Smile modules to log info, warnings, errors, or fatal events.

Provides module name, cause, caller identifier, and consequences for context.

- Parameters:
    - `lvl` — Severity level of the log (`INFO`, `WARN`, etc.).
    - `ori` — Name of the module or tool generating the log.
    - `cse` — Description of the cause of the log event.
    - `caller` — Identifier of the caller — typically the function name (e.g.
      `__func__`), or the tool name for top-level tool logs.
    - `csq` — Consequences or additional information about the event.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Common failures use `cmResult` (for example, `RES_NULL_ARG`).
    - Log-specific failures use `lgInternalResult`:
      `RES_TIME_FAIL`, `RES_WRITE_FAIL`.
    - If `lvl` is `FATAL`, the configured fatal handler is invoked after
      attempting to log.

✅ Example

```c
bool smPrivateHasStarted(const char *fnName)
{
    if (!smHasStarted())
    {
        lgInternalLog(ERROR, ORI, CSE_NOT_RUNNING, fnName, CSQ_ABORT);
        return false;
    }

    return true;
}
```

<br>

| `int lgInternalLogWithArg(lgInternalLevel lvl, const char *ori, const char *cse, const char *arg, const char *caller, const char *csq)` |
|-----------------------------------------------------------------------------------------------------------------------------------------|

Used by Smile modules to log info, warnings, errors, or fatal events with
additional context.

Similar to lgInternalLog, but includes an extra argument string for additional
context.

- Parameters:
    - `lvl` — Severity level of the log (`INFO`, `WARN`, etc.).
    - `ori` — Name of the module or tool generating the log.
    - `cse` — Description of the cause of the log event.
    - `arg` — Additional context argument relevant to the log event.
    - `caller` — Identifier of the caller — typically the function name (e.g.
      `__func__`), or the tool name for top-level tool logs.
    - `csq` — Consequences or additional information about the event.

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
