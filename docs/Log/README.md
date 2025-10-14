# Log — Getting Started 📝

**Version:** 1.0.0 — **Date:** Oct 14, 2025

`Log` provides an API for writing printf-style messages to the terminal and
configuring fatal error handling.

### 🚨 Warning! This module is not thread-safe!

---

<br>

## Table of Contents

- [Visual Example](#visual-example)
- [Log Lifecycle](#log-lifecycle-)
- [Quick Reference Table](#-quick-reference-table)
- [Workflow Example](#-workflow-example)

---

<br>

## Visual Example

---

<br>

## Log Lifecycle 🔄

1️⃣ Use `lgLog` to print messages to the terminal and help with debugging.

2️⃣ Use `lgSetFatalHandler` to customize the behaviour of fatal errors that
would end the program by default. To do so, you must pass a function pointer to
your custom handler.

---

<br>

## 🔍 Quick Reference Table

For detailed documentation, see the [Log API Reference](Log_API.md).

### Data Types

— _Function Pointers_

| Signature                                 | Description                                            |
|-------------------------------------------|--------------------------------------------------------|
| `void lgSetFatal(lgFatalHandler handler)` | Function pointer type for custom fatal error handlers. |

### Functions

| Signature                                 | Description                                                                                                                                                                          |
|-------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `void lgLog(const char *msg, ...)`        | Logs a formatted message to the terminal. Supports printf-style formatting. Output is written to stderr.                                                                             |
| `void lgSetFatal(lgFatalHandler handler)` | Sets a custom handler to be called when a fatal event occurs. If `NULL` or `nullptr`, resets to the default handler which logs the event to the terminal and terminates the program. |

--- 

<br>

## 🧪 Workflow Example

```c
#include "Log.h"

void MyFatalHandler(void) {
    // Custom handler code here
}

int main() {
    lgLog("Smile version %.1f is out!", 1.0f);
    // Outputs: 01:23:45 [Smile Log From User] - Smile version 1.0 is out!

    lgSetFatal(MyFatalHandler); // Set a custom fatal handler
    
    /**
     * If a Smile module triggers a fatal error, it will log it and the 
     * resolution will be handled by MyFatalHandler.
     */
    
    ...
    
    lgSetFatal(nullptr); // Reset to default handler
    
    /**
     * Now, if a Smile fatal error is triggered, it will be logged and the
     * program will terminate with failure status.
     */
}
```

---

<br>

| Version | Author        | Description      | Date         |
|---------|---------------|------------------|--------------|
| 1.0.0   | Vitor Betmann | Created document | Oct 14, 2025 |