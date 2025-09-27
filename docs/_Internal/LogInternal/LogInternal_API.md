# LogInternal API📝

For logging guidelines, see
the [Coding Contributing Doc](../_Contributing/1_Coding.md).

### 🚨 Warning! This module is not thread-safe.

---

## Table of Contents

- [Data Types](#-data-types)
    - [LogLevel](#loglevel)
- [Functions](#-functions)
- [Workflow Examples](#-workflow-examples)

---

## 📦 Data Types

---

### _Enums_

---

### `LogLevel`

LogInternal severity levels used by SMILE.

Represents the importance of a log message, ranging from user-defined messages
to fatal errors.

| Enum          | Summary                                                                                              | Color  |
|---------------|------------------------------------------------------------------------------------------------------|--------|
| `LOG_USER`    | For custom user generated logs. See lgLog in [Log_API](../../Log/Log_API.md).                        | Green  |
| `LOG_INFO`    | Reports informational logs. E.g. "Start Successful."                                                 | Blue   |
| `LOG_WARNING` | Reports non-game breaking or non-intentional events. E.g. calling the Start function twice in a row. | Yellow |
| `LOG_ERROR`   | Reports game-breaking errors. E.g. memory allocation failed.                                         | Red    |
| `LOG_FATAL`   | Reports program breaking errors. E.g. failing to close a file.                                       | Purple |

---

## 🔧 Functions

---

###

`void lgInternalLog(LogLevel level, const char *module, const char *cause,const char *fnName, const char *conseq)`

Used by SMILE modules to log info, warnings, errors, or fatal events.

Provides module name, cause, function name, and consequences for context.

- **Parameters:**
    - `fnName` — Name of the test function or operation that passed.
    - `level` — Severity level of the log (LOG_INFO, LOG_WARNING, etc.).
    - `module` — Name of the module generating the log.
    - `cause` — Description of the cause of the log event.
    - `fnName` — Name of the function where the log is generated.
    - `conseq` — Consequences or additional information about the event.
- **Returns:** Nothing.

**Example:**

```c
NO EXAMPLE YET
```

For more, see [Workflow Examples](#-workflow-examples).

---

###

`void lgInternalLogWithArg(LogLevel level, const char *module, const char *cause, const char *arg, const char *fnName,const char *conseq)`

Used by SMILE modules to log info, warnings, errors, or fatal events with
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
- **Returns:** Nothing.

**Example:**

```c
NO EXAMPLE YET
```

For more, see [Workflow Examples](#-workflow-examples).

---

## 📖 Workflow Examples

```c
// NO EXAMPLES YET
```