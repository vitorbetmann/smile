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

Below is a structural Example

```c
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Includes
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include <> — External libraries (e.g., stdlib.h, string.h) 

#include "" — Related to the current module (e.g., StateMachine.h, StateMachineInternal.h)

#include "" — Files that support the current module (e.g., LogInternal, TestInternal.h)


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————
 
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

### — _Headers_

1. All headers must have inclusion guards

// ✅ Do
#ifndef SMILE_MODULE_NAME_H
#define SMILE_MODULE_NAME_H
// ... header content ...
#endif // SMILE_MODULE_NAME_H

============================================================

4. Add include order convention

// System headers first
#include <stdio.h>
#include <stdlib.h>

// Then project headers  
#include "smStateMachine.h"
#include "lgLog.h"

// Then internal headers
#include "smStateMachineInternal.h"

============================================================

➤ “Header Dependencies”

	•	Every header should include everything it needs (no implicit dependencies).
	•	Never rely on transitive includes.

#### Public Headers

* naming

Public API Functions: [modulePrefix][Verb][Object]

**Example**

```c
```

<br>

For documenting Public Headers, see [Documenting Public Headers]() in
the [Documentation Guidelines]() doc.

<br>

#### Internal Headers

* Logging Related

Internal Functions: [modulePrefix]Internal[Verb][Object]

**Example**

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

**Example**

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

static void helperFunc(void) { // File scope function

**Example**

```c
```

<br>

* private funcs

Private Functions: [modulePrefix]Private[Description]

**Example**

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

* Use `camelCase` for all non-constant variable names.
* Avoid single-character names except for loop iterators or short
  mathematical expressions.
* Use descriptive names and avoid non-standard abbreviations unless common (
  e.g., `ptr`, `buf`, `len`).
* Use plural for collections unless it's a string.
* See [Type Naming](#type-naming) for rules on naming structs, enums, and
  typedefs.
* See [Constant Values](#constant-values) for rules on naming #define and
  constant variables.

✅ Do

```c
int stateCount;    --> Non-constant variables use camelCase

————————————————————————————————————————————————————————————————————————————————

char *smileModules[4];             --> Plural for non-string collections
char *greeting = "Hello Smile";    --> Singular for strings

————————————————————————————————————————————————————————————————————————————————

for (int i = 0; i < max; i++) {    --> Single-character name for loop iterator
    ...
}

————————————————————————————————————————————————————————————————————————————————

float c = sqrt(pow(a, 2) + pow(b, 2));    --> Single-character name for mathematical formulas

```

❌ Don't

```c
int my_int = 1;    --> snake_case
int MyInt = 1;     --> PascalCase
int myint = 1;     --> lowercase

————————————————————————————————————————————————————————————————————————————————

int n = 10;    --> single-letter non-loop variable

————————————————————————————————————————————————————————————————————————————————

char *smileModule[4];               --> Misleading! Doesn't indicate a collection
char *greetings = "Hello Smile";    --> Misleading! Indicates a collection
```

<br>

#### Variable Naming — Booleans

* Boolean variables should read naturally in conditionals and use consistent
  prefixes that indicate their boolean nature. Common names include (but
  aren't limited to):
    * `is`/`are` for states (`isVisible`, `areLightsOn`)
    * `has` for ownership or presence (`hasChildren`, `hasTexture`)
    * `should` for conditional behavior (`shouldRender`, `shouldUpdate`)
    * `can` for capabilities (`canJump`, `canCastSpells`)
    * Present tense for existence checks (`fileExists`, `playerLives`)
* See [Type Conversion](#type-conversion) for preferred boolean and pointer
  comparison styles.

✅ Do

```c
bool isRunning = true;    --> Prefixes clearly state they're booleans
bool canShoot = false;

————————————————————————————————————————————————————————————————————————————————

if (isRunning && canShoot) {    --> Reads naturally: "If is running and can shoot"
    ...
}
```

❌ Don't

```c
--> Misleading! Doesn't indicate it's a boolean
bool particles = false;

if (particles) {
    ...
}
```

<br>

#### File-Scoped Non-Constant Variables

* Declare all file-scoped variables as `static` to restrict access.

✅ Do

```c
static InternalTracker *tracker;
```

❌ Don't

```c
InternalTracker *tracker;    --> Externally linked by default
```

<br>

#### Variable Declaration and Grouping

* Declare each variable on its own line unless they represent the same logical
  unit.
* Don't group unrelated variables, even if they share a type.
* When declaring pointers:
    * Write the `*` next to the variable name, not the type.
    * Declare each pointer separately for clarity.
* ⚠️️ Never mix pointer and non-pointer variables in a single declaration.

✅ Do

```c
int width, height, depth;    --> Different logical units in separate lines
int price, change;

————————————————————————————————————————————————————————————————————————————————

char *src;    --> Pointers declared in different lines
char *dst;
```

❌ Don't

```c
int width, height, depth, price, change;    --> Unrelated groups

————————————————————————————————————————————————————————————————————————————————

char *src, *dst;    --> Unnecessarily verbose

————————————————————————————————————————————————————————————————————————————————

char *name, grade;    --> Misleading! Only one is a pointer
```

<br>

#### Constant Values

* Use `const` for local constants inside functions only.
* For file- or module-scoped constants:
    * Use `#define` for primitive values and strings.
    * Use `enum` for related groups of integer constants.
    * Use `static const` for array/struct constants private to a file.
* All constants must use SCREAMING_SNAKE_CASE.
* ⚠️ Never use `#define` to create function-like macros.

✅ Do

```c
#define DEFAULT_FPS 60    --> #define for primitives or strings

————————————————————————————————————————————————————————————————————————————————

typedef enum {    --> Enums for related groups
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL
} InternalLevel;

————————————————————————————————————————————————————————————————————————————————

void myFunc(void) {
    const int MAX_BUFFER_SIZE = 1024;    --> const inside a function
    char buffer[MAX_BUFFER_SIZE];
    ...
}

————————————————————————————————————————————————————————————————————————————————

static const float IDENTITY_MATRIX[16] = {    --> static const for file-scoped array
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1
};
```

❌ Don't

```c
#define LEVEL_USER 0       --> Related integer constants should use enum     
#define LEVEL_INFO 1
#define LEVEL_WARNING 2    

————————————————————————————————————————————————————————————————————————————————

const int MAX_BUFFER_SIZE = 1024;    --> Wrong scope.

void myFunc(void) {
    ...
}

————————————————————————————————————————————————————————————————————————————————

#define SQUARE(x) ((x) * (x))    --> Function-like macro     
```

<br>

#### Magic Numbers

* Avoid magic numbers. All repeated or meaningful values should be defined as
  constants.
* Exceptions:
    * Loop iterators.
    * Obvious mathematical formulas or operations.
    * Obvious initializations or allocations.

✅ Do

```c
for (int i = 0; i < max; i++) {    --> Loop iterators
    ...
}

————————————————————————————————————————————————————————————————————————————————

float c = sqrt(pow(a, 2) + pow(b, 2));    --> Mathematical formulas

————————————————————————————————————————————————————————————————————————————————

next = current + 1;    --> Obvious oparations

————————————————————————————————————————————————————————————————————————————————

--> Obvious initializations or allocations

Type myStruct = { 0 };

InternalTracker *tracker = calloc(1, sizeof(InternalTracker));

————————————————————————————————————————————————————————————————————————————————

#define DEFAULT_FPS 60    --> Meaningful or repeated values should be constants
```

❌ Don't

```c
float circumference =  2 * 3.14159f * radius;     --> Meaningful primitives should be #define'd

————————————————————————————————————————————————————————————————————————————————

for (int i = 0; i < 10; i++) { --> Unclear! 10 is an arbitrary number
    ...
}

```

<br>

### — _Functions_

#### Naming — General

* All Smile functions begin with a unique module-identifying two-letter
  lowercase module prefix, followed by a PascalCase name. Below are two tables
  that relate modules and their prefixes.
* Function names should be verbs or verb phrases that describe the action.
  Common functions names include: `Start`, `IsRunning`, `Stop`, `Get`, `Set`,
  `Create`, `Delete`, and `Exists`.
* Using a consistent prefix prevents naming collisions, while shared names help
  users quickly infer a function’s purpose even in unfamiliar modules.

| Public Module  | Prefix |
|----------------|--------|
| Log            | lg     |
| ParticleSystem | ps     |
| SaveLoad       | sl     |
| StateMachine   | sm     |

| Internal Module | Prefix |
|-----------------|--------|
| Common          | cm     |
| Test            | ts     |

✅ Example

```c
bool smStart(void);
bool smIsRunning(void);

bool smCreateState(const char *name, smEnterFn enter, smUpdateFn update, smDrawFn draw, smExitFn exit);
bool smStateExists(const char *name);
bool smSetState(const char *name, const void *args);
int smGetStateCount(void);
bool smDeleteState(const char *name);

bool smUpdate(float dt);
bool smDraw(void);

bool smStop(void);
```

<br>

#### Naming — Access Levels

* For different access levels, include the following after the prefix:
    * Public: Only module prefix.
    * Internal: Add `Internal` after prefix.
    * Private: Add `Private` after prefix.

✅ Example

```c
--> Found in StateMachine.h
bool smStateExists(const char *name);

————————————————————————————————————————————————————————————————————————————————

--> Found in StateMachine.internal.h  
const State *smInternalGetState(const char *name);

————————————————————————————————————————————————————————————————————————————————

--> Found in StateMachine.c
bool smPrivateIsNameValid(const char *name, const char *fnName);
```

<br>

#### Parameters — General

* Function parameters should use `camelCase`.
* Choose descriptive names that make the argument's purpose clear.
* When appropriate, function argument order should make the signature read like
  natural language.

✅ Example

```c
--> "Log (level) from (origin) with (message) using (args)"
void lgPrivateLogV(InternalLevel level, const char *origin, const char *msg, va_list args);

————————————————————————————————————————————————————————————————————————————————

--> "Delete state: (name)"
bool smDeleteState(const char *name);
```

<br>

#### Parameters — Const

* Passed-by-reference should `const Type *ptr` when reading only.
* Copied-by-value parameters shouldn't be `const`.

✅ Do

```c
bool smStateExists(const char *name);    --> Implies read-only parameter

————————————————————————————————————————————————————————————————————————————————

bool smUpdate(float dt);    --> const is unecessary because argument is copied by value
```

❌ Don't

```c
bool smStateExists(char *name);    --> Misleading! Implies the function modifies the string

————————————————————————————————————————————————————————————————————————————————

bool smUpdate(const float dt);    --> Unnecessarily verbose
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
--> Implicit conversion for pointer checks in return statements and conditionals

bool smIsRunning(void) {
    return tracker; 
}

if (tracker) { 
    return false;
}

————————————————————————————————————————————————————————————————————————————————

if (playerCount > 0) {    --> Explicit comparisons for non-pointer variables 
    ...
}
```

❌ Don't

```c
if (playerCount) {    --> Misleading! Implies pointer
    ...
}

————————————————————————————————————————————————————————————————————————————————

bool smIsRunning(void) {
    return tracker != nullptr;    --> Unnecessarily verbose
}
```

<br>

#### Flow and Structure

* Skip no lines from the function signature to its first statement.
* Handle all failure or invalid conditions first and return early. This avoids
  deep nesting and keeps the main logic path clear.
* Group related operations together and separate conceptual steps with a single
  blank line.
* Use wrapper test functions for memory allocation. This facilitates testing.
* Use `Private` functions for statements that are repeated throughout multiple
  functions.
* Skip no lines before return statements.

✅ Example

```c
bool smStart(void) {
    if (tracker) {        --> Failure conditions first
        return false;
    }

    /* Code blocks related to a single task go together
     * In this example, allocating memory and handling errors
     */
    tracker = tsInternalCalloc(1, sizeof(StateMachineTracker));    --> Use wrapper test functions for memory allocation
    if (!tracker) {
        return false;
    }

    --> Success path
    tracker->fps = DEFAULT_FPS;
    return true;
}

————————————————————————————————————————————————————————————————————————————————

--> Use Private functions for statements that are repeated throughout multiple functions

bool smStateExists(const char *name) {
    if (!smPrivateIsRunning(FN_STATE_EXISTS)) {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_STATE_EXISTS)) {
        return false;
    }

    return smInternalGetEntry(name);
}
```

<br>

#### Return Types and Error Handling

* Return `bool` for functions that:
    * Would normally return void but require success/failure indication for
      testing purposes (e.g., `smStart`, `slStop`).
    * Semantically represent state checks.
* For numeric return types, use sentinel values like `-1` or `-1.0f` to indicate
  failure.
* For pointer return types, return `nullptr` when data is unavailable or
  operations fail.
* Log all failures through the Log module with appropriate severity levels (
  See [InternalLog](../Log/LogInternal_API.md) for details).

✅ Example

```c
--> Doesn't return any meaningful value for the user but returns bool for testing purposes

bool smStart(void) {
    if (tracker) {
        lgInternalLog(WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(InternalTracker));
    if (!tracker) {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker->fps = DEFAULT_FPS;

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return true;
}

————————————————————————————————————————————————————————————————————————————————

--> Return bool for state checks

bool smIsRunning(void) {
    return tracker;
}

————————————————————————————————————————————————————————————————————————————————

--> Return -1, -1.0f or -1.0 on error for numeric return types

int smGetStateCount(void) {
    if (!smPrivateIsRunning(FN_GET_STATE_COUNT)) {
        return -1;
    }

    return tracker->stateCount;
}

————————————————————————————————————————————————————————————————————————————————

--> Return nullptr, if appropriate, when data is unavailable or operations fail.

const char *smGetCurrentStateName(void) {
    if (!smPrivateIsRunning(FN_GET_CURRENT_STATE_NAME)) {
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

```

<br>

#### goto

* Use `goto` only for cleanup paths to simplify error handling and prevent
  memory leaks. This improves maintainability and reduces duplicated free() or
  close() calls.
* ⚠️ Never use goto for non-error control flow (e.g., loops or jumps).

✅ Do

```c
--> Only use goto for cleanup

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
--> Never use goto for non-error control flow

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
--> In CommonInternalMessages.h
#define CONSEQ_ABORTED  "Aborted"

--> In StateMachineMessages.h
#define CAUSE_STATE_NOT_FOUND "State not found"
#define FN_SET_STATE "SetState"

--> In StateMachine.c
bool smSetState(const char *name, const void *args) {
    ...
    
    const InternalState *nextState = smInternalGetState(name);
    if (!nextState) {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_STATE_NOT_FOUND,
                             name,FN_SET_STATE, CONSEQ_ABORTED);
        return false;
    }
    
    ...
}
```

<br>

### — _Formatting and Layout_

#### Braces

* Always use braces, even for single-line conditionals or loops.
* Place the opening brace on the same line as the statement, and the closing
  brace one line after the final statement.
* Include a `struct` or `enum`'s name on the same line as the closing brace.

✅ Do

```c
int main(void) {
    ...
    return 0;
}

if (false) {
    return;
}

typedef enum {
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;
```

❌ Don't

```c
--> Wrong brace placement

int main(void)
{
    ...
    return 0;
    
}

if (false)
{ return ; }

typedef enum {
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL } InternalLevel;
    
————————————————————————————————————————————————————————————————————————————————

--> Inline statements
 
if (false) { return; }

if (false) return;

————————————————————————————————————————————————————————————————————————————————

--> Dangling statement

if (false) 
    return; 
```

<br>

#### Pointers

* Write the pointer operator (`*`) next to the variable name, not the type.
* Leave no space between the dereference operator (`*`) and the variable.
* Use the arrow operator (`-->`) to dereference struct pointers.
* ⚠️ Don’t use the dereference-dot pattern (`(*p).member`) or include spaces
  around
  the arrow.

* ✅ Do

```c
int *ptr = malloc(sizeof(int));
*ptr = 10;

————————————————————————————————————————————————————————————————————————————————

Player *player = &player;
int score = player-->score;
```

❌ Don't

```c
int *ptr = malloc(sizeof(int));
* ptr = 10;    --> It should have no space between the dereference operator and the variable

————————————————————————————————————————————————————————————————————————————————

Player* p = &player;             --> Pointer operator should go next to the variable name
int playerScore = (*player).score;    --> Don't use the dereference-dot pattern
int playerId = player -> id;          --> No spaces around the arrow operator
```

<br>

#### Commas and Other Operators

* Leave one space after each comma and around operators, except for the
  pointer, dereference, and arrow operators.
* A collection should be multi-line if it would be hard to comprehend. The
  lines should be indented.
* `Enums` should always be multi-line.
* For multi-line collections, always include a trailing comma.

✅ Do

```c
--> Collections that would be hard to comprehend as a single line should be multi-line
static const float IDENTITY_MATRIX[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1,    --> Include trailing comma for multi-line collections
};

————————————————————————————————————————————————————————————————————————————————

typedef enum {    --> Enums are always multi-line
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;

————————————————————————————————————————————————————————————————————————————————

int myArray[] = { 0, 1, 2, 3, 4 };  --> Simple, short collections can be a single line long

————————————————————————————————————————————————————————————————————————————————

int sum = num1 + num2;    --> Include spaces around operators
```

❌ Don't

```c
int myArray[] = {0,1,2,3,4};    --> No spaces after commas decreases readability

————————————————————————————————————————————————————————————————————————————————

--> Short, easy to grasp collection should be a single line long

int myArray[] = {
    0,
    1,
    2,
}

————————————————————————————————————————————————————————————————————————————————

int sum=num1+num2;    --> No spaces around operators decreases readability
```

<br>

#### Indentation

* Indentation occurs in increments of 4 spaces.
* ⚠️ Never use tabs.

✅ Example

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
* For long statements (function calls, `if`, `while`, `for`, etc.), break after
  a comma, operator, or logical boundary and align subsequent lines with the
  first argument.

✅ Do

```c
bool smCreateState(const char *name, smEnterFn enter, smUpdateFn update,        --> Line limit
                   smDrawFn draw, smExitFn exit);
```

❌ Don't

```c
bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn);
```

<br>

### — _Miscellaneous_

#### Typedef Preference

* Use `typedef` for defining `enums` and `structs`.

✅ Do

```c
typedef struct {
    int x;
    int y;
} Point;

typedef struct Node {
    int data;
    struct Node* next;
} Node;
```

❌ Don't

```c
struct Node {
    int data;
    struct Node* next;
};
```

#### Type Naming

* Use `PascalCase` for all `struct`, `enum`, and `typedef` names.
* All public types must include the module prefix as part of their name.
* Prefix internal-only types with `Internal`.
* ⚠️ Never typedef primitive types.

✅ Do

```c
typedef struct {    --> Internal type
    ...
} InternalState;

————————————————————————————————————————————————————————————————————————————————

typedef void (*smEnterFn)(const void *args);    --> Public type
```

❌ Don't

```c
typedef int Number;    --> Hides primitive type

————————————————————————————————————————————————————————————————————————————————

typedef void (*Enter)(void*);   --> No module prefix for a public type
```

<br>

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

#### Comments — General

* Comments should explain why, not what.
* Use `//` for short inline comments, and `/* ... */` for documentation blocks
  or temporarily disabling code.
* If a variable or function call’s purpose isn’t obvious, add a concise //
  comment. If a section needs heavy commenting to be understood, refactor it
  into a well-named function and document that instead (see
  [3_Documentation_Guidelines](3_Documentation_Guidelines.md) for details).

✅ Do

```c
--> Document typedefs, functions, structs, enums... 
/**
 * @brief Function pointer type for state exit callbacks.
 *
 * @author Vitor Betmann
 */
typedef void (*smExitFn)(void);

————————————————————————————————————————————————————————————————————————————————

--> Comment on statements that are hard to understand at a glance
fprintf(stderr, "%s\n", SMILE_WHITE); // Reset Log color
```

❌ Don't

```c
--> Obvious comments
player.health = 100;  // player health set to 100

————————————————————————————————————————————————————————————————————————————————

--> Use single-line style for multi-line comments or vice-versas
// @brief Function pointer type for state exit callbacks.
// 
// @author Vitor Betmann
typedef void (*smExitFn)(void);
```

<br>

#### Comments — TODOS

* Each TODO comment in code should be short and concise, referencing a
  corresponding GitHub issue using the format
  `// TODO #<issue_number> <summary>`.
* Place TODOs at the top of the file they affect.

✅ Do

```c
// TODO #16 trim state name on registration    --> This is at the top of the file
// TODO #42 refactor allocation logic
```

❌ Don't

```c
bool smSetFPS(int fps) {
    // todo implement this    --> Don't include TODO's anywhere other than at the top of the file 
    return false;
}

————————————————————————————————————————————————————————————————————————————————

// todo: fix this    --> These TODO's don't follow Smile's convention
// fix whitespace issue later
// TODO check names

————————————————————————————————————————————————————————————————————————————————

--> Really long TODO's

/*
 * TODO #42 this function is broken because of the complex memory allocation
 * logic that needs to be refactored to avoid leaks when the initialization
 * fails, see the following stack trace...
 */
```

<br>

#### Preprocessor Rules

* Use `#ifdef` for single condition checks.
* Use the `defined()` function for multiple condition checks.
* Always include a matching #endif with a trailing comment naming the condition.
* Don’t indent preprocessor directives. They should start at column 0.
* Keep a single space between the directive and the condition.

✅ Example

```c
#ifdef _WIN32
    ...
#elif defined(__APPLE__) || defined(__linux__)
    ...
#endif // #ifdef _WIN32
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