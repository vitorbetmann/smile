# Log — API 📝

The `Log` module provides functions for writing printf-style messages to the
terminal and configuring fatal error handling.

### 🚨 Warning! This module is not thread-safe!

---

<br>

## Table of Contents

- [Module Header](#module-header)
- [Data Types](#-data-types)
    - [Function Pointers](#-_function-pointers_)
- [Functions](#-functions)
    - [Logging Related](#-_logging-related_)
    - [Fatal Handling Related](#-_logging-related_)

---

<br>

## 😊Module Header

The module’s header is `Log.h`. Its full Smile path is: `smile/include/Log.h`

✅ Example

```c
#include <Log.h>
```

---

<br>

## 📦 Data Types

### — _Function Pointers_

| `void (*lgFatalHandler)(void)` |
|--------------------------------|

Function pointer type for custom fatal error handlers.

**Example:**

```c
void myFatalHandler(void)
{
    // Log error reason to file
    exit(EXIT_FAILURE);
}
```

---

<br>

## 🔧 Functions

### — _Logging Related_

| `void lgLog(const char *msg, ...)` |
|------------------------------------|

Logs a message to the terminal ending in a new line. Supports printf-style
formatting.

- **Parameters:**
    - `msg` — Format string for the message to log.
    - `...` — Additional arguments related to the format specifiers in msg.

- **Note:**
    - Output is written to stderr.

**Example:**

```c
lgLog("Smile version %.1f is out!", 1.0f);
// Outputs: 01:23:45 [Smile Log From User] - Smile version 1.0 is out!
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [Log Getting Started](README.md) doc.

---

### — _Fatal Handling Related_

### `void lgSetFatal(lgFatalHandler handler)`

Sets a custom handler to be called when a fatal event occurs.

- **Parameters:**
    - `handler` — Function pointer to the custom fatal handler. If `NULL` or
      `nullptr`, the default handler is set, which terminates the program with
      failure status after logging the event to the terminal.

- **Note:** It's recommended your custom handler terminates the program.

**Example:**

```c
// Define a handler
void myFatalHandler(void)
{
    // Custom handler code here
}

int main()
{
    lgSetFatal(MyFatalHandler);
    ... // More code
}
```

<br>

For more, see [Workflow Example](README.md#-workflow-example) in
the [Log Getting Started](README.md) doc.

---

<br>

| Last modified | Author        | Description       |
|---------------|---------------|-------------------|
| Oct 14, 2025  | Vitor Betmann | Created document. |