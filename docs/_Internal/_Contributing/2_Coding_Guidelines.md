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

static bool smPrivateIsNameValid(const char *name, const char *fnName);


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

bool smCreateState(const char *name, smEnterFn enterFn,
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

#### Variable Naming — General

* Use `camelCase` for all variable names.
* Avoid single-character names except for loop counters or short
  mathematical expressions.
* Use descriptive names and avoid non-standard abbreviations unless common (
  e.g., `ptr`, `buf`, `len`).
* ⚠️ Never use globals in Smile.

✅ Do

```c
int maxConnections = 10;
for (int i = 0; i < maxConnections; i++) {
    ...
}
```

❌ Don't

```c
int my_int = 1;           // snake_case
int MyInt = 1;            // PascalCase
int myint = 1;            // lowercase
int n = 10;               // single-letter (not a loop variable)
```

<br>

#### Variable Naming — Booleans

* Boolean variables should read naturally in conditionals and use consistent
  prefixes that indicate their boolean nature. Common prefixes include (but
  aren't limited to):
    * `is`/`are` for states (`isVisible`, `areLightsOn`)
    * `has` for ownership or presence (`hasChildren`, `hasTexture`)
    * `should` for conditional behavior (`shouldRender`, `shouldUpdate`)
    * `can` for capabilities (`canJump`, `canCastSpells`)
    * Present tense for existence checks (`fileExists`, `playerLives`)

✅ Do

```c
bool isRunning = true;
bool hasCollision = false;
bool canShoot = true;

if (isRunning && canShoot && !hasCollision) { // Reads like natural English
    ...
}
```

❌ Don't

```c
bool particles = false;   // doesn't follow boolean naming convention
```

<br>

#### File-Scoped Variables

* Always declare file-scoped variables as `static` to restrict linkage and
  prevent naming conflicts across translation units.
* Avoid using const at file scope without static, since that grants external
  linkage by default and may expose internal data unintentionally.
* If a variable is meant to be constant and private to the file, combine both.

✅ Do

```c
static int playerCount;
static const int MAX_PLAYERS = 10;
```

❌ Don't

```c
const int playerCount;        // Externally linked by default
```

<br>

#### Grouping Variables

* Group variables only when they represent the same concept or logical unit.
* Unrelated variables, even if they share a type, should be declared
  separately.
* ⚠️ Never group pointer declarations.

✅ Do

```c
int width, length, height;
int price, change;

char *source;  // Clear: source is a pointer to char
char *destination;
```

❌ Don't

```c
int width, length, height, price, change;    // Unrelated groups
char *source, destination;                   // Misleading: source is pointer, destination is not!
int age, *agePtr;                            // Mixed types
```

<br>

#### Constant Values

* Use `const` for local constants inside functions only.
* For file- or module-scoped constants:
    * Use `#define` for primitive values and strings
    * Use `enum` for related groups of integer constants
    * Use `static const` for array/struct constants private to a file
* All constants (including `const`) must use SCREAMING_SNAKE_CASE.
* ⚠️ Never use `#define` to create function-like macros.

✅ Do

```c
#define GRAVITY 9.81f

typedef enum {
    LOG_USER,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} LogLevel;

void myFunc(void) {
    const int MAX_BUFFER_SIZE = 1024;
    char buffer[MAX_BUFFER_SIZE];
    ...
}

static const float IDENTITY_MATRIX[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1
};
```

❌ Don't

```c
#define LOG_USER 0      // Use enum for related constants instead!
#define LOG_INFO 1
#define LOG_WARNING 2

const int MAX_BUFFER_SIZE = 1024;  // file-scoped const (wrong scope)
#define SQUARE(x) ((x) * (x))      // function-like macro
```

<br>

#### Magic Numbers

* Avoid magic numbers. All repeated or meaningful values should be defined as
  constants.
* Exceptions:
    * Loop indices (e.g., `for (int i = 0; i < max; i++)`)
    * Simple mathematical identities (e.g., `x * 2`, `array + 1` for pointer
      arithmetic)
    * Zero-initialization (e.g., `memset(buffer, 0, size)`)

✅ Do

```c
// Simple mathematical identities are acceptable as magic numbers
position.x = origin.x + offset * 2;  // '2' is clear in context
nextElement = current + 1;           // '+1' is idiomatic

// But meaningful values should be constants
#define TILE_SIZE 64
if (x % TILE_SIZE == 0) {  // Not: x % 64 == 0
    ...
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

#### Naming — General

* All Smile functions begin with a unique two-letter lowercase module prefix,
  followed by a PascalCase name. The prefix identifies the module (e.g., `sm`
  for StateMachine, `lg` for Log).
* Function names should be verbs or verb phrases that describe the action.
  Common functions names include: `Start`, `IsRunning`, `Stop`, `Get`, `Set`,
  `Create`, `Delete`, and `Exists`.
* Using a consistent prefix prevents naming collisions, while shared names help
  users quickly infer a function’s purpose even in unfamiliar modules.

✅ Example

```c
// StateMachine
bool smStart(void);
bool smIsRunning(void);

bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn);
bool smStateExists(const char *name);
bool smSetState(const char *name, const void *args);
int smGetStateCount(void);
bool smDeleteState(const char *name);

bool smUpdate(float dt);
bool smDraw(void);

bool smStop(void);

// Log
void lgLog(const char *msg, ...);
void lgSetFatal(lgFatalHandler handler);
```

<br>

#### Naming — Access Levels

* For different access levels, include the following after the prefix:
    * Public: Only module prefix.
    * Internal: Add `Internal` after prefix.
    * Private: Add `Private` after prefix.

✅ Example

```c
// Public - in StateMachine.h
bool smStateExists(const char *name);

// Internal - in StateMachine.internal.h  
const State *smInternalGetState(const char *name);

// Private - only in StateMachine.c
bool smPrivateIsNameValid(const char *name, const char *fnName);
```

<br>

#### Arguments

* Function arguments should use `camelCase` like variables.
* Choose descriptive names that make the argument's purpose clear.
* When appropriate, function argument order should make the signature read like
  natural language.

✅ Example:

```c
void lgPrivateLogV(LogLevel level, const char *origin, const char *msg, va_list args);
// "Log (level) from (origin) with (message) using (args)"

bool smDeleteState(const char *name);
// "Delete state: (name)"
```

<br>

#### Type Conversion

* Use implicit conversion for nullptr checks in both return statements and
  conditional expressions.
* Use explicit comparisons for numeric and other non-pointer types to make
  conditions unambiguous.
* This approach maintains clarity while keeping code concise for the most common
  cases, and helps contributors quickly identify pointers without adding
  verbosity.

✅ Do

```c
bool smIsRunning(void) {
    return tracker;  // Clearly checking if tracker is not NULL
}

bool smStart(void) {
    if (tracker) {  // Clearly checking if tracker already exists
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(StateMachineTracker));
    if (!tracker) {  // Clearly hecking if allocation failed
        return false;
    }

    return true;
}

if (playerCount > 0) {  // Clearly checking for players
    ...
}
```

❌ Don't

```c
if (playerCount) {  // Unclear: Could mean "any non-zero value"
    // ...
}

bool smIsRunning(void) {
    return tracker != NULL;  // Unnecessarily verbose
}
```

<br>

#### Flow

* Functions should handle all “failure” or “invalid” conditions first, returning
  early when appropriate. This keeps code readable, avoids deep nesting, and
  makes the function’s success path clear.
* Return statements should be visually attached to the logic that produces
  them, never separated by blank lines.

✅ Example

```c
bool smStart(void) {
    // Failure conditions first
    if (tracker) {
        lgInternalLog(LOG_WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START, CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(StateMachineTracker));
    if (!tracker) {
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START, CONSEQ_ABORTED);
        return false;
    }

    // Success path - minimal nesting, clear flow
    tracker->fps = DEFAULT_FPS;

    lgInternalLog(LOG_INFO, MODULE, CAUSE_MODULE_STARTED, FN_START, CONSEQ_SUCCESSFUL);
    return true;
}
```

<br>

#### Related Logic

* Group related operations without blank lines and separate logical sections
  with one blank line.
* Each group should represent a single "conceptual step" in the function.
* This visual organization makes the function's algorithm clear at a glance.

✅ Example

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
  memory leaks. This improves maintainability and reduces duplicated free() or
  close() calls.
* ⚠️ Never use `goto` for control flow or looping.

✅ Do

```c
bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn) {
    StateMap *entry = smInternalGetEntry(name);
    State *state = malloc(sizeof(State));
    ...
    
    char *nameCopy = malloc(strlen(name) + 1);
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
void smPrintNumbers(void) {
    int i = 0;

start:
    printf("%d\n", i);
    i++;
    if (i < 5)
        goto start;
}
```

<br>

#### Shared Code and Messages

* Use `CommonInternal.h` for shared utility functions.
* Use `CommonInternalMessages.h` for shared log messages and error strings.
* This ensures consistency across modules and reduces duplication.
* Always check these files before creating new shared resources.

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
        lgInternalLog(LOG_ERROR, MODULE, CAUSE_NOT_RUNNING, FN_STATE_EXISTS, CONSEQ_ABORTED);
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

#### Braces — General

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

#### Braces — Dangling blocks

* Use braces after loops and conditionals, even if they're only one line long.
  This avoids accidental one-line bugs.
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

if (false) { return; }

if (false) 
    return;
```

<br>

#### Declaring Pointers

* Write the pointer operator (`*`) next to the variable, not the data type. This
  makes multiple pointer declarations clearer.

* ✅ Do

```c
Player *p;
```

❌ Don't

```c
Player* p;
```

#### Dereference Operator

* Leave no space between the dereference operator (`*`) and the variable.

* ✅ Do

```c
int *ptr = malloc(sizeof(int));
*ptr = 10;
```

❌ Don't

```c
int *ptr = malloc(sizeof(int));
* ptr = 10;
```

#### Arrow Operator

* Use the arrow operator (`->`) to dereference struct pointers.
* Don’t use the dereference-dot pattern (`(*p).member`) or include spaces around
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

#### Commas and Other Operators

* Leave one space after each comma and around operators, except for the
  pointer, dereference, and arrow operators.

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
bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn) {
    ...
}
```

❌ Don't

```c
bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn) {
    ...
}
```

<br>

### — _Miscellaneous_

#### nullptr

* Use `nullptr` (C23 standard) instead of `NULL` to avoid implicit conversions
  and ensure type safety.

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

#### Comments

* Comments should explain **why**, not **what**.
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
// The line below
// increments i by 1
i++;

player.health = 100;  /* player health set to 100 */
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