# Coding Guidelines 🧑‍💻

This document defines Smile’s C coding style and file organization conventions
to ensure consistency, readability, and maintainability across all modules.

> **Note:**  
> Smile follows the **C23 standard** for all C source code and headers.  
> All examples and conventions in this document assume C23-compatible compilers
> (e.g., Clang ≥ 17, GCC ≥ 13, or MSVC ≥ 19.3).  
> Features like `nullptr` and `bool` are used accordingly.


---

<br>

## Table of Contents

- [General Conventions](#-general-conventions)
    - [Writing Style](#-_writing-style_)
    - [Code Organization]()
    - [Naming Conventions]()
- [Public Headers]()
- [Private Headers]()
- [Message Files]()
- [Source Files]()

---

<br>

## 😊 General Conventions

### — _Writing Style_

Variables use `camelCase`:

✅ Do

```c
int myInt = 1;
```

❌ Don't

```c
int my_int = 1;    // snake_case
int MyInt = 1;     // PascalCase
int myint = 1;     // lowercase
```

<br>

Variable names must not be a single character long unless it is the iteration
variable of a loop:

✅ Do

```c
int max = 10;

for (int i = 0; i < max; i++) {
    ...
}
```

❌ Don't

```c
int n = 10;

for (int i = 0; i < n; i++) {
    ...
}
```

<br>

Use const for local constants inside functions only. For file- or module-scoped
constants, always use #define. Both must use SCREAMING_SNAKE_CASE.

✅ Do

```c
#define MAX_BUFFER_SIZE 1024

void myFunc(void) {
    const float GRAVITY = 9.81f;
    char buffer[MAX_BUFFER_SIZE];
    ...
}
```

❌ Don't

```c
const int MAX_BUFFER_SIZE = 1024;

void myFunc(void) {
    const float PI = 3.14159f;
    ...
}
```

<br>

For file-scoped variables, use the `static` keyword.

✅ Do

```c
static int myInt;
```

❌ Don't

```c
const int myInt;
```

<br>

Only group variable declarations when they are conceptually related, not just
because they share the same type.

✅ Do

```c
int width, length, height;
int price, change;
```

❌ Don't

```c
int width, length, height, price, change;
```

<br>

Opening brace must be on the same line as conditional or function, and closing
brace should be one line below the last statement:

✅ Do

```c
int main(void) {
    ...
}

if (true) {
    ...
}
```

❌ Don't

```c
int main(void)
{
    ...
}

if (true)
{ ... }
```

<br>

Leave one space after each comma and around operators.

✅ Do

```c
int arrLen = 5;
int myArray[] = { 0, 1, 2, 3, 4 };

for (int i = 0; i < arrLen; i++) {
    int sum = myArray[i] + myArray[i];
    printf("%d", sum);
}
```

❌ Don't

```c
int arrLen = 5;
int myArray[] = {0,1,2,3,4};

for (int i = 0;i < arrLen;i++) {
    int sum = myArray[i]+myArray[i];
    printf("%d",sum);
}
```

<br>

Indentation occurs in increments of 4 spaces. Never use tabs.

✅ Do

```c
while (true) {
    for (int i = 0; i < max; i++) {
        ...
    }
}
```

❌ Don't

```c
while (true) {
for (int i = 0; i < max; i++) {
...
}
}
```

<br>

Use nullptr (C23 standard) instead of NULL for safety and consistency.

✅ Do

```c
void *myFunction(void) {
    ...
    
    return nullptr;
}
```

❌ Don't

```c
void *myFunction(void) {
    ...
    
    return NULL;
}
```

<br>

Dereference pointers to structs using the arrow operator (->). Do not use the
dereference-dot pattern ((*p).member), and do not include spaces around the
arrow.

✅ Do

```c
Player *p = &player;
int score = p->score;
```

❌ Don't

```c
Player *p = &player;
int playerScore = (*p).score;
int playerId = p -> id;
```

<br>

A line of code shouldn't extend over 80 characters.

✅ Do

```c
bool smCreateState(const char *stateName, smEnterFn enterFn,
                   smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn) {
    ...
}
```

❌ Don't

```c
bool smCreateState(const char *stateName, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn) {
    ...
}
```

<br>

Avoid magic numbers unless they’re self-explanatory (e.g., loop indices starting
at i = 0, or mathematical constants in equations). Define all reused or
meaningful numbers as constants.

✅ Do

```c
#define PI 3.14159f
#define MAX_ENEMIES 10

float getCircumference(float radius) {
    return 2 * PI * radius;
}

void spawnEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        ...
    }
}
```

❌ Don't

```c
float getCircumference(float radius) {
    return 2 * 3.14159f * radius;
}

void spawnEnemies(void) {
    for (int i = 0; i < 10; i++) {
        ...
    }
}
```

<br>

Use goto only for cleanup paths to simplify error handling and prevent memory
leaks. Never use it for control flow or looping.

✅ Do

```c
bool smCreateState(const char *stateName, smEnterFn enterFn,
                   smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn) {
    StateMap *entry = smInternalGetEntry(stateName);
    State *state = malloc(sizeof(State));
    ...
    
    char *nameCopy = malloc(strlen(stateName) + 1);
    if (!nameCopy) {
        ...
        goto nameCopyError;
    }
    
    ...

    StateMap *mapEntry = tsInternalMalloc(sizeof(StateMap));
    if (!mapEntry) {
        ...
        goto mapEntryError;
    }
    
    ...
    return true;

mapEntryError:
    free(nameCopy);
nameCopyError:
    free(state);
    return false;
}
```

❌ Don't

```c
void processState(State *state) {
    if (!state) {
        goto handleError;
    }

    if (!state->isActive) {
        goto handleInactive;
    }

    printf("Processing state...\n");
    goto done;

handleInactive:
    printf("State inactive.\n");
    goto done;

handleError:
    printf("Invalid state!\n");

done:
    printf("Finished.\n");
}
```

<br>

Always use braces after loops and conditionals, even if they're only one line
long. No inline if statements or "dangling" blocks.

✅ Do

```c
if (false) {
    return;
}
```

❌ Don't

```c
if (false) return;

if (false) 
    return;
```

<br>

---

### — _Code Organization_

All code-related Smile files should be split into sections that follow the
pattern below.

Each section should be separated by a "section header" like the following:

```c
// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
```

After the last line of a given section, skip 2 lines until the header of the
next one. Sections that might not be relevant to a certain file can be omitted.

Below is a structural example:

```c
// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <> — External libraries (e.g., stdlib.h, string.h) 

#include "" — Related to the current module (e.g., StateMachine.h, StateMachineInternal.h)

#include "" — Files that support the current module (e.g., LogInternal, TestInternal.h)


// -----------------------------------------------------------------------------
// Defines
// -----------------------------------------------------------------------------
 
#defines 


// -----------------------------------------------------------------------------
// Data Types
// -----------------------------------------------------------------------------

Data Types (e.g., enums, structs)


// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

Variables


// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

Prototypes

// -----------------------------------------------------------------------------
// Functions — Public
// -----------------------------------------------------------------------------

Public Functions definitions


// -----------------------------------------------------------------------------
// Functions — Internal
// -----------------------------------------------------------------------------

Internal Functions definitions


// -----------------------------------------------------------------------------
// Functions — Private
// -----------------------------------------------------------------------------

Private Functions definitions


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

main() Function
```

Below is an example using the StateMachine module:

```c
// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <string.h>
#include <../external/uthash.h>

#include "../include/StateMachine.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"

#include "../src/Log/LogInternal.h"
#include "../src/_Internal/Common/CommonInternalMessages.h"
#include "../src/_Internal/Test/TestInternal.h"

// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------

static StateMachineTracker *tracker;


// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------

static bool smPrivateHasStarted(const char *fnName);

static bool smPrivateIsNameValid(const char *stateName, const char *fnName);


// -----------------------------------------------------------------------------
// Functions - Public
// -----------------------------------------------------------------------------

// Start Related

bool smStart(void) {
    ...
}

bool smIsRunning(void) {
    ...
}

// State Functions

bool smCreateState(const char *stateName, smEnterFn enterFn,
```

<br>

---

## 😊 Functions

### — _Naming Conventions_

Naming Conventions for Functions
Public API Functions: [modulePrefix][Verb][Object]
Internal Functions: [modulePrefix]Internal[Verb][Object]
Private Functions: [modulePrefix]Private[Description]

Common Verbs Used
start, isRunning, stop, set, get, create, delete, exists

### — _Function content_

```c
void lgPrivateLogV(LogLevel level, const char *origin, const char *msg, va_list args) {
    if (!lgPrivateIsLevelEnabled(level)) {
        return;
    }

    const char *color = nullptr;
    const char *prefix = nullptr;
    lgPrivateGetColorAndPrefix(level, &color, &prefix);

    const time_t epochTime = time(nullptr);
    struct tm localTime;
    localtime_r(&epochTime, &localTime);
    char timeBuf[LOG_TIME_BUFFER_LEN];
    strftime(timeBuf, sizeof(timeBuf), LOG_TIME_FMT, &localTime);

    fprintf(stderr, "%s%s [Smile %s From %s] - ", color, timeBuf, prefix,
            origin);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", SMILE_WHITE); // Reset color

    if (level == LOG_FATAL) {
        fatalHandler();
    }
}
```

### — _Code Organization_

Don't reinvent the wheel! Reusability: CommonMessages.h for shared messages and
common.h for shared functions

**Example:**

```c
```

<br>

### — _Code Organization_

Additional Notes

- Consistency: Each module should have its own <ModuleName>Messages.h
- Reusability: CommonMessages.h for shared messages.
- Clarity: Function names describe action and target.
- Maintainability: Internal functions go in ModuleNameInternal.h, helpers inside
  .c files only.

**Example:**

```c
```

---


<br>

### — _func naming conventions_

**Example:**

```c
```

### — _func naming conventions_

Naming Conventions for Functions
Public API Functions: [modulePrefix][Verb][Object]
Internal Functions: [modulePrefix]Internal[Verb][Object]
Private Functions: [modulePrefix]Private[Description]

**Example:**

```c
```

---

<br>

## 😊 Public Headers

### — _naming_

Public API Functions: [modulePrefix][Verb][Object]

**Example:**

```c
```

<br>

For documenting Public Headers, see [Documenting Public Headers]() in
the [Documentation Guidelines]() doc.

---

<br>

## 😊 Internal Headers

### — _Logging Related_

Internal Functions: [modulePrefix]Internal[Verb][Object]

**Example:**

```c
```

<br>

For documenting Internal Headers, see [Documenting Internal Headers]() in
the [Documentation Guidelines]() doc.

---

<br>

## 😊 Message Files

### — _Logging Related_

2. Module Message Files
   Every module has a <ModuleName>Messages.h file containing:

- Module name
- Function names (FN_* defines)
- Causes
- Consequences
  CommonMessages.h contains cross-module messages.

**Example:**

```c
```

<br>

For documenting Message Files, see [Documenting Message Files]() in
the [Documentation Guidelines]() doc.

---

<br>

## 😊 Source Files

### — _logs_

Log Message Pattern
Every log for errors, warnings, or info follows a structured pattern:
logLevel, moduleName, cause, optionalArgument, functionName, consequence

**Example:**

```c
```

<br>

### — _private funcs_

Private Functions: [modulePrefix]Private[Description]

**Example:**

```c
```

<br>

For documenting Source Files, see [Documenting Source Files]() in
the [Documentation Guidelines]() doc.

---

<br>

Authored by: Vitor Betmann