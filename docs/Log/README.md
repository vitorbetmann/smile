# Log — Getting Started 📝

`Log` provides an API for writing printf-style messages to the terminal and
configuring fatal error handling.

### 🚨 Warning! This module is not thread-safe!

---

## 📋 Table of Contents

- [Visual Example](#visual-example)
- [Module Header](#module-header)
- [Log Lifecycle](#log-lifecycle-)
- [Quick Reference Table](#-quick-reference-table)
- [Workflow Example](#-workflow-example)

---

## Visual Example

<p align="center">
  <img src="https://raw.githubusercontent.com/vitorbetmann/smile-assets/refs/heads/main/Log/LogDemo.gif" width="90%" alt="GIF of Log module working"/>
</p>

---

## 😊Module Header

The module’s header is `Log.h`. Its full Smile path is: `smile/include/Log.h`

✅ Example

```c
#include <Log.h>
```

---

## Log Lifecycle 🔄

1️⃣ Use `lgLog` to print messages to the terminal and help with debugging.

2️⃣ Use `lgSetFatal` to customize the behavior of fatal errors that
would end the program by default. To do so, you must pass a function pointer to
your custom handler.

---

## 🔍 Quick Reference Table

### Data Types

— Function Pointers

| Signature                      | Description                                            |
|--------------------------------|--------------------------------------------------------|
| `void (*lgFatalHandler)(void)` | Function pointer type for custom fatal error handlers. |

<br>

### Functions

| Signature                                | Description                                                                                                                                                                                                  |
|------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `int lgLog(const char *msg, ...)`        | Logs a formatted message to the terminal. Supports printf-style formatting. Output is written to `stderr`. Returns `0` on success, negative error code on failure.                                           |
| `int lgSetFatal(lgFatalHandler handler)` | Sets a custom handler to be called when a fatal event occurs. If `NULL` or `nullptr`, resets to the default handler which logs the event to the terminal and terminates the program. Returns `0` on success. |

---

## 🧪 Workflow Example

```c
#include "Log.h"

void MyFatalHandler(void)
{
    // Custom handler code here
}

int main()
{
    lgLog("Smile version %.1f is out!", 1.0f);
    // Outputs: 01:23:45 [User LOG] - Smile version 1.0 is out!

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

Safety note:

- `lgLog` uses `printf`-style formatting. Do not pass untrusted text directly
  as the first argument; use `lgLog("%s", externalText)` instead.
