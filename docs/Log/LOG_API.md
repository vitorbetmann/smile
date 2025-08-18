# SMILE Log: API 📝

The SMILE Logging module provides a simple, flexible system for recording user-level messages and handling fatal errors.

---

## 🌀 Lifecycle Overview

1. **Logging Messages**

   - Use `LOG()` to output messages from a specified origin. Supports `printf`-style formatting. Each call automatically appends a new line.

2. **Custom Fatal Handlers**
   - Use `LOG_SetFatalHandler()` to override the default behavior for fatal log events. Default handler immediately terminates the program.

---

## 🧪 Example Usage

```c
#include "Log.h"

void MyFatalHandler(void) {
    // Custom cleanup code here
}

int main(void) {
    // Log a normal message
    LOG("main", "Starting my new game. Version: %d.%d", 1, 0);
    // Will print to terminal:
    // [09:10:11] [SMILE USER from main]: Starting my new game. Version: 1.0

    // Set a custom fatal handler
    LOG_SetFatalHandler(MyFatalHandler);

    // Use other SMILE modules. If a fatal error is triggered, it will still be logged in SMILE style.
    // The resolution, however, will be handled by MyFatalHandler.

    return 0;
}
```

---

## 📚 Function Reference

### `void LOG(const char *origin, const char *msg, ...);`

**Logs a formatted SMILE-style message to the terminal.**  
Each message includes a timestamp, the SMILE USER tag, the origin, and the user-provided text. Output is written to standard error with coloring for readability. **A newline is automatically appended.**

Example output:  
[09:10:11] [SMILE USER from main]: Starting my new game.

- `origin`: A string identifying the source of the message (e.g., function name or point in the program).
- `msg`: Format string, `printf`-style.
- `...`: Arguments matching the format string.

**Returns:**  
Nothing.

---

### `void LOG_SetFatalHandler(SMILE_FatalHandler handler);`

**Sets a custom handler for fatal log events.**  
By default, fatal logs terminate the program immediately via `SMILE_DefaultFatalHandler()`. Passing `NULL` restores the default handler.

- `handler`: Pointer to a function with no parameters and no return value.

**Returns:**  
Nothing.

---

## 🏷️ Type Definitions

### `typedef void (*SMILE_FatalHandler)(void);`

**Function pointer type for fatal log handlers.**  
Used with `LOG_SetFatalHandler()` to define custom behavior when a fatal event occurs.
