# LogInternal API 📝

`LogInternal` provides a logging interface for Smile modules. It contains
internal
logging functionality and severity levels for warnings, errors, and fatal events
inside Smile core modules.

For coding guidelines, see
the [Coding Contributing Doc](../_Contributing/1_Coding.md).

### 🚨 Warning! This module is not thread-safe!

---

<br>

## Table of Contents

- [Data Types](#-data-types)
    - [Enums](#-_enums_)
- [Functions](#-functions)
    - [Log Related](#-_log-related_)

---

<br>

## 📦 Data Types

### — _Enums_

| `LogLevel` |
|------------|

LogInternal severity levels used by Smile. Represents the importance of a log
message, ranging from user-defined messages to fatal errors.

| Item          | Summary                                                                                              | Color  |
|---------------|------------------------------------------------------------------------------------------------------|--------|
| `LOG_USER`    | For custom user generated logs. See lgLog in [Log_API](../../Log/Log_API.md).                        | Green  |
| `LOG_INFO`    | Reports informational logs. E.g. "Start Successful."                                                 | Blue   |
| `LOG_WARNING` | Reports non-game breaking or non-intentional events. E.g. calling the Start function twice in a row. | Yellow |
| `LOG_ERROR`   | Reports game-breaking errors. E.g. memory allocation failed.                                         | Red    |
| `LOG_FATAL`   | Reports program breaking errors. E.g. failing to close a file.                                       | Purple |

**Example:**

```c
lgInternalLog(LOG_ERROR, MODULE, CAUSE_NOT_STARTED, fnName, CONSEQ_ABORTED);
```

---

<br>

## 🔧 Functions

### — _Log Related_

| `void lgInternalLog(LogLevel level, const char *module, const char *cause,const char *fnName, const char *conseq)` |
|--------------------------------------------------------------------------------------------------------------------|

Used by Smile modules to log info, warnings, errors, or fatal events.

Provides module name, cause, function name, and consequences for context.

- **Parameters:**
    - `fnName` — Name of the test function or operation that passed.
    - `level` — Severity level of the log (LOG_INFO, LOG_WARNING, etc.).
    - `module` — Name of the module generating the log.
    - `cause` — Description of the cause of the log event.
    - `fnName` — Name of the function where the log is generated.
    - `conseq` — Consequences or additional information about the event.

**Example:**

```c
bool smPrivateHasStarted(const char *fnName) {
    if (!smHasStarted()) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NOT_STARTED, fnName, CONSEQ_ABORTED);
        return false;
    }
    
    return true;
}
```

<br>

| `void lgInternalLogWithArg(LogLevel level, const char *module, const char *cause, const char *arg, const char *fnName,const char *conseq)` |
|--------------------------------------------------------------------------------------------------------------------------------------------|

Used by Smile modules to log info, warnings, errors, or fatal events with
additional context.

Similar to lgInternalLog, but includes an extra argument string for additional
context.

- **Parameters:**
    - `fnName` — Name of the test function or operation that passed.
    - `level` — Severity level of the log (LOG_INFO, LOG_WARNING, etc.).
    - `module` — Name of the module generating the log.
    - `cause` — Description of the cause of the log event.
    - `arg` — Additional context argument relevant to the log event.
    - `fnName` — Name of the function where the log is generated.
    - `conseq` — Consequences or additional information about the event.

**Example:**

```c
bool smPrivateIsNameValid(const char *stateName, const char *fnName) {
    if (!stateName) {
        lgInternalLogWithArg(LOG_ERROR, MODULE, CAUSE_NULL_ARG, "name", fnName, CONSEQ_ABORTED);
        return false;
    }
    
    ...
}
```

For coding guidelines, see
the [Coding Contributing Doc](../_Contributing/1_Coding.md).

---

<br>

| Last modified | Author        | Description       |
|---------------|---------------|-------------------|
| Oct 14, 2025  | Vitor Betmann | Created document. |