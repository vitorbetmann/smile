# Coding Guidelines 🧑‍💻

This document defines Smile’s C coding style and file organization conventions
to ensure consistency, readability, and maintainability across all modules.

😊 Style Philosophy

Smile’s C style emphasizes clarity, consistency, and maintainability above all.
Every rule in this document exists to make Smile’s codebase predictable — so
that any developer can read, debug, or extend a module without surprises.

Prefer explicitness to cleverness, readability over compression, and
structure over spontaneity.
Consistent formatting and naming conventions allow the code to communicate
intent faster than comments ever could.

When in doubt, write code that looks like existing Smile code — clean,
consistent, and easy to reason about.

> **Note:**  
> Smile follows the **C23 standard** for all C source code and headers.  
> All examples and conventions in this document assume C23-compatible compilers
> (e.g., Clang ≥ 17, GCC ≥ 13, or MSVC ≥ 19.3).  
> Features like `nullptr` and `bool` are used accordingly.


---

<br>

## Table of Contents

- [Code Organization](#-code-organization)
    - [Public Headers](#-_public-headers_)
    - [Internal Headers](#-_internal-headers_)
    - [Message Files](#-_message-files_)
    - [Source Files](#-_source-files_)
- [General Conventions](#-general-conventions)
    - [Variables and Constants](#-_variables-and-constants_)
    - [Functions](#-_functions_)
    - [Formatting and Layout](#-_formatting-and-layout_)
    - [Miscellaneous](#-_miscellaneous_)

---

<br>

## 🗂 Code Organization

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

### — _Public Headers_

* naming

Public API Functions: [modulePrefix][Verb][Object]

**Example:**

```c
```

<br>

For documenting Public Headers, see [Documenting Public Headers]() in
the [Documentation Guidelines]() doc.

<br>

### — _Internal Headers_

* Logging Related

Internal Functions: [modulePrefix]Internal[Verb][Object]

**Example:**

```c
```

<br>

For documenting Internal Headers, see [Documenting Internal Headers]() in
the [Documentation Guidelines]() doc.

<br>

### — _Message Files_

* Logging Related

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

<br>

### — _Source Files_

* logs

Log Message Pattern
Every log for errors, warnings, or info follows a structured pattern:
logLevel, moduleName, cause, optionalArgument, functionName, consequence

**Example:**

```c
```

<br>

* private funcs

Private Functions: [modulePrefix]Private[Description]

**Example:**

```c
```

<br>

For documenting Source Files, see [Documenting Source Files]() in
the [Documentation Guidelines]() doc.

---

<br>

## 🧭 General Conventions

### — _Variables and Constants_

#### Variable Naming pt. 1

* Variables use `camelCase`:

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

#### Variable Naming pt. 2

* Variable names must not be a single character long unless it is the iteration
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
```

<br>

#### static variables

* For file-scoped variables, use `static`.

✅ Do

```c
static int myInt;
```

❌ Don't

```c
const int myInt;
```

<br>

#### Grouping Variables

* Group variables only when they are conceptually related.
* Avoid grouping unrelated variables just because they share a type.
* ⚠️ Never group pointer declarations.

✅ Do

```c
int width, length, height;
int price, change;

char *src;
char *dst;
```

❌ Don't

```c
int width, length, height, price, change;
char *src, *dst;
```

<br>

#### Constant Values

* Use const for local constants inside functions only.
* Use #define or an enum (for related constants) for file- or module-scoped
  constants.
* All constants should use SCREAMING_SNAKE_CASE.
* ⚠️ Never use #define to create function-like macros.

✅ Do

```c
#define GRAVITY 9.81f

typedef enum {
    LOG_USER,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
} LogLevel;

void myFunc(void) {
    const int MAX_BUFFER_SIZE = 1024;
    char buffer[MAX_BUFFER_SIZE];
    ...
}
```

❌ Don't

```c
#define SQUARE(x) ((x) * (x)) 

const int MAX_BUFFER_SIZE = 1024;
```

<br>

#### Magic Numbers

* Avoid magic numbers unless they’re self-explanatory (e.g., loop indices
  starting at i = 0, or mathematical constants in equations).
* Define all reused or meaningful numbers as constants.

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

### — _Functions_

#### Naming

* All Smile functions begin with a unique two-letter lowercase module prefix,
  followed by a name in PascalCase.
* Common verbs include: Start, IsRunning, Stop, Get, Set, Create, Delete, and
  Exists.
* Using a module-specific prefix prevents naming collisions, while familiar
  verbs make functions self-explanatory and easier to maintain.

✅ Example

```c
// StateMachine module prefix: sm
bool smStart(void);
bool smIsRunning(void);

bool smCreateState(const char *stateName, smEnterFn enterFn,
                   smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn);
bool smStateExists(const char *name);
bool smSetState(const char *name, const void *args);
int smGetStateCount(void);
bool smDeleteState(const char *name);

float smGetDt(void);
bool smUpdate(float dt);
bool smDraw(void);

bool smStop(void);

// Log module prefix: lg
void lgLog(const char *msg, ...);
void lgSetFatal(lgFatalHandler handler);
```

<br>

#### Arguments

* When appropriate, function argument order should “read” like natural language.

✅ Example:

```c
void lgPrivateLogV(LogLevel level, const char *origin, const char *msg, va_list args);
// → "Log (level) from (origin) with (message) and (args)"

bool smDeleteState(const char *name);
// → "Delete state: (name)"
```

<br>

#### Access

* When naming `Public` functions, only include the two-letter module prefix. For
  `Internal` or `Private` functions, add Internal or Private after the prefix.

✅ Example:

```c
bool smStateExists(const char *name);

const State *smInternalGetState(const char *name);

bool smPrivateIsNameValid(const char *stateName, const char *fnName);
```

<br>

#### Flow

* Functions should handle all “failure” or “invalid” conditions first, returning
  early when appropriate. This keeps code readable, avoids deep nesting, and
  makes the function’s
  success path clear.
* Return statements should be visually attached to the logic that produces
  them — never separated by blank lines.

  ✅ Example

```c
bool smStart(void) {
    if (tracker) {
        lgInternalLog(LOG_WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START, CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(StateMachineTracker));
    if (!tracker) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START, CONSEQ_ABORTED);
        return false;
    }

    tracker->fps = DEFAULT_FPS;

    lgInternalLog(LOG_INFO, MODULE, CAUSE_MODULE_STARTED, FN_START, CONSEQ_SUCCESSFUL);
    return true;
}
```

<br>

#### Related Logic

* Lines of code that operate on the same concept or data should appear together
  with no blank lines between them.
* Insert one blank line only when transitioning to a new logical step in the
  function.

```c
void lgPrivateLogV(LogLevel level, const char *origin, const char *msg, va_list args) {
    // Regarding Logging permission.
    if (!lgPrivateIsLevelEnabled(level)) {
        return;
    }

    // Regarding the Log's color and prefix.
    const char *color = nullptr;
    const char *prefix = nullptr;
    lgPrivateGetColorAndPrefix(level, &color, &prefix);

    // Regarding the Log's time.
    const time_t epochTime = time(nullptr);
    struct tm localTime;
    localtime_r(&epochTime, &localTime);
    char timeBuf[LOG_TIME_BUFFER_LEN];
    strftime(timeBuf, sizeof(timeBuf), LOG_TIME_FMT, &localTime);

    // Regarding printing the Log.
    fprintf(stderr, "%s%s [Smile %s From %s] - ", color, timeBuf, prefix,
            origin);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", SMILE_WHITE); // Reset color

    // Regarding handling a fatal condition.
    if (level == LOG_FATAL) {
        fatalHandler();
    }
}
```

<br>

#### goto

* Use `goto` only for cleanup paths to simplify error handling and prevent
  memory leaks.
* ⚠️ Never use `goto` for control flow or looping.

✅ Example

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

<br>

#### Common Module

* Use `CommonInternal.h` for shared functions and `CommonInternalMessages.h` for
  shared messages.
* Make sure a common function or message doesn’t already exist before creating a
  new one.

✅ Example

```c
// From CommonInternalMessages.h
#define CAUSE_NOT_RUNNING  "Module Not Running"
#define CONSEQ_ABORTED  "Aborted"

// From StateMachineMessages.h
#define FN_STATE_EXISTS "StateExists"

// In StateMachine.c
bool smStateExists(const char *name) {
    if (!smIsRunning()) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NOT_RUNNING, fnName, CONSEQ_ABORTED);
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_STATE_EXISTS)) {
        return false;
    }

    return smInternalGetEntry(name);
}
```

<br>

### — _Formatting and Layout_

#### Braces pt. 1

* Place the opening brace on the same line as the function or conditional.
* The closing brace should appear one line below the final statement.

✅ Do

```c
int main(void) {
    ...
    return 0;
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
    return 0;
    
}

if (true)
{ ... }
```

<br>

#### Braces pt. 2

* Use braces after loops and conditionals, even if they're only one line long.
* No inline if statements or "dangling" blocks.

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

#### Comma and Operators

* Leave one space after each comma and around operators.

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

#### Indentation

* Indentation occurs in increments of 4 spaces.
* ⚠️ Never use tabs.

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

#### Line Length

* A line of code shouldn't extend over 80 characters.

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

### — _Miscellaneous_

#### nullptr

* Use nullptr (C23 standard) instead of NULL to avoid implicit conversions and
  ensure type safety.

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

#### Declaring Pointers

* Write the `*` next to the variable, not the data type.

* ✅ Do

```c
Player *p;
```

❌ Don't

```c
Player* p;
```

#### Arrow Operator

* Use the -> operator to dereference struct pointers.
* Don’t use the dereference-dot pattern ((*p).member) or include spaces around
  the arrow.

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

#### Comments

* Use `//` for short inline comments, and `/* ... */` for documentation blocks
  or temporarily disabling code.
* If a variable or function call’s purpose isn’t obvious, add a concise //
  comment. If a section needs heavy commenting to be understood, refactor it
  into a well-named function and document that instead (see
  [3_Documentation_Guidelines](3_Documentation_Guidelines.md) for details).

✅ Do

```c
/*
 * Long comment describing
 * a data structure
 */
typedef struct {
    ...
} Player;

fprintf(stderr, "%s\n", SMILE_WHITE); // Reset Log color
```

❌ Don't

```c
// Comments on multiple lines
...
// either for obvious code or
...
// for what should be a function
...

player.health = 100;  /* Single-line comment using multi-line style */
```

<br>

---

<br>

Next: [3_Documentation_Guidelines](3_Documentation_Guidelines.md)

---

<br>

| Last modified | Author        | Description       |
|---------------|---------------|-------------------|
| Oct 14, 2025  | Vitor Betmann | Created document. |