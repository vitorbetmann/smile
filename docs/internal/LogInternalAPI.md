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

| `lgResult` |
|------------|

Log-specific result codes used when failures are not covered
by [cmResult](CommonInternalAPI.md#-enums).

- Log-specific failures cover the following range: `-100..-199`.

| Item                     | Value  | Summary                             |
|--------------------------|--------|-------------------------------------|
| `LG_RESULT_WRITE_FAILED` | `-100` | Logging output write/flush failed.  |
| `LG_RESULT_TIME_FAILED`  | `-101` | Time acquisition/formatting failed. |

<br>

| `InternalLevel` |
|-----------------|

LogInternal severity levels used by Smile. Represents the importance of a log
message, ranging from user-defined messages to fatal errors.

| Item      | Summary                                                                                              | Color  |
|-----------|------------------------------------------------------------------------------------------------------|--------|
| `USER`    | For custom user generated logs. See lgLog in [Log_API](../Log/LogAPI.md).                            | Green  |
| `INFO`    | Reports informational logs. E.g. "Start Successful."                                                 | Blue   |
| `WARNING` | Reports non-game breaking or non-intentional events. E.g. calling the Start function twice in a row. | Yellow |
| `ERROR`   | Reports game-breaking errors. E.g. memory allocation failed.                                         | Red    |
| `FATAL`   | Reports program-breaking errors. E.g. fatal cleanup failures.                                        | Purple |

✅ Example

```c
lgInternalLog(ERROR, MODULE, CAUSE_NOT_RUNNING, fnName, CONSEQ_ABORTED);
```

## 🛠️ Functions

### — Log Related

| `int lgInternalLog(InternalLevel level, const char *module, const char *cause, const char *fnName, const char *conseq)` |
|-------------------------------------------------------------------------------------------------------------------------|

Used by Smile modules to log info, warnings, errors, or fatal events.

Provides module name, cause, function name, and consequences for context.

- Parameters:
    - `level` — Severity level of the log (`INFO`, `WARNING`, etc.).
    - `module` — Name of the module generating the log.
    - `cause` — Description of the cause of the log event.
    - `fnName` — Name of the function where the log is generated.
    - `conseq` — Consequences or additional information about the event.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Common failures use `cmResult` (for example, `CM_RESULT_NULL_ARG`).
    - Log-specific failures use `lgResult`:
      `LG_RESULT_TIME_FAILED`, `LG_RESULT_WRITE_FAILED`.
    - If `level` is `FATAL`, the configured fatal handler is invoked after
      attempting to log.

✅ Example

```c
bool smPrivateHasStarted(const char *fnName)
{
    if (!smHasStarted())
    {
        lgInternalLog(ERROR, MODULE, CAUSE_NOT_RUNNING, fnName, CONSEQ_ABORTED);
        return false;
    }

    return true;
}
```

<br>

| `int lgInternalLogWithArg(InternalLevel level, const char *module, const char *cause, const char *arg, const char *fnName, const char *conseq)` |
|-------------------------------------------------------------------------------------------------------------------------------------------------|

Used by Smile modules to log info, warnings, errors, or fatal events with
additional context.

Similar to lgInternalLog, but includes an extra argument string for additional
context.

- Parameters:
    - `level` — Severity level of the log (`INFO`, `WARNING`, etc.).
    - `module` — Name of the module generating the log.
    - `cause` — Description of the cause of the log event.
    - `arg` — Additional context argument relevant to the log event.
    - `fnName` — Name of the function where the log is generated.
    - `conseq` — Consequences or additional information about the event.

- Returns: `0` on success, or a negative result code on failure.

- Notes:
    - Common failures use `cmResult` (for example, `CM_RESULT_NULL_ARG`).
    - Log-specific failures use `lgResult`:
      `LG_RESULT_TIME_FAILED`, `LG_RESULT_WRITE_FAILED`.
    - If `level` is `FATAL`, the configured fatal handler is invoked after
      attempting to log.

✅ Example

```c
bool smPrivateIsNameValid(const char *name, const char *fnName)
{
    if (!name)
    {
        lgInternalLogWithArg(ERROR, MODULE, CAUSE_NULL_ARG, "name", fnName, CONSEQ_ABORTED);
        return false;
    }

    ...
}
```
