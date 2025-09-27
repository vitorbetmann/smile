# Log API 📝

`Log` provides an API for writing
printf-style messages to the terminal and configuring fatal error handling.

---

## Table of Contents

- [Data Types](#-data-types)
    - [lgFatalHandler](#void-lgfatalhandlervoid)
- [Functions](#-functions)
    - [lgLog](#void-lglogconst-char-msg-)
    - [lgSetFatal](#void-lgsetfatallgfatalhandler-handler)
- [Workflow Examples](#-workflow-examples)
- [Notes](#-notes)

---

## 📦 Data Types

---

### _Function Pointers_

---

### `void (*lgFatalHandler)(void)`

Function pointer type for custom fatal error handlers.

---

## 🔧 Functions

---

### _Log Related_

---

### `void lgLog(const char *msg, ...)`

Logs a message to the terminal ending in a new line. Supports printf-style
formatting.

- **Parameters:**
    - `msg` — Format string for the message to log.
    - `...` — Additional arguments related to the format specifiers in msg.
- **Returns:** Nothing.
- **Note:** Output is written to stderr.

**Example:**

```c
lgLog("SMILE version %.1f is out!", 1.0f);
// Outputs: 01:23:45 [Smile LOG From User] - SMILE version 1.0 is out!
```

For more, see [Workflow Examples](#-workflow-examples).

---

### _Fatal Handling Related_

---

### `void lgSetFatal(lgFatalHandler handler)`

Sets a custom handler to be called when a fatal event occurs.

- **Parameters:**
    - `handler` — Function pointer to the custom fatal handler. If NULL, the
      default handler is set, which terminates the program with failure status
      after logging the event to the terminal.
- **Returns:** Nothing
- **Note:** It's recommended your custom handler terminates the program.

**Example:**

```c
// Define a handler
void myFatalHandler(void) {
    // Custom handler code here
}

int main() {
    lgSetFatal(MyFatalHandler);
    ... // More code
}
```

For more, see [Workflow Examples](#-workflow-examples).

---

## 📖 Workflow Examples

```c
#include "Log.h"

void MyFatalHandler(void) {
    // Custom handler code here
}

int main() {
    lgLog("SMILE version %.1f is out!", 1.0f);
    // Outputs: 01:23:45 [Smile LOG From User] - SMILE version 1.0 is out!

    lgSetFatal(MyFatalHandler); // Set a custom fatal handler
    
    /**
     * If a SMILE module triggers a fatal error, it will log it and the 
     * resolution will be handled by MyFatalHandler.
     */
    
    ... // More code
    
    lgSetFatal(nullptr); // Reset to default handler
    
    /**
     * Now, if a SMILE fatal error is triggered, it will be logged and the
     * program will terminate with failure status.
     */
    
    ... // More code
}
```

---

## 📝 Notes

- Designed for single-threaded environments.
