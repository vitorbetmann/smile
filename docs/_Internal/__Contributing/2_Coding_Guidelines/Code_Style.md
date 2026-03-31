# Coding Guidelines — Code Style 🧑‍💻

This document defines Smile’s C coding standard conventions to ensure
consistency, readability, maintainability, and safety across all modules.

> Note:
> Smile follows the C23 standard for all C source code and headers.  
> 
> All examples and conventions in this document assume C23-compatible compilers
> Features like `nullptr` and `bool` are used accordingly.
>
> Smile uses `.clang-format` as the source of truth for code formatting. If this
> document conflicts with formatting produced by `.clang-format`, follow
> `.clang-format`. Contributors who use IDE or editor auto-formatting should
> configure their tools to use `clang-format` with the repository's
> `.clang-format` file.

## Table of Contents

- [Golden Rule](#-golden-rule)
- [Variables and Constants](#-variables-and-constants)
- [Functions](#-functions)
- [Formatting and Layout](#-formatting-and-layout)
- [Miscellaneous](#-miscellaneous)
- [Up Next](#-up-next)

## 🥇 Golden Rule

### — ⚠️ Never use globals in Smile unless it's for testing.

---

## 🌦️ Variables and Constants

### — Variable Naming: General

* Use `camelCase` for all non-constant variable names.
* Avoid single-character names except for loop iterators or short mathematical expressions.
* Use descriptive names and avoid non-standard abbreviations unless common (e.g., `ptr`, `buf`, `len`).
* Use plural for collections unless it's a string.
* See [Type Naming](#-type-naming) for rules on naming structs, enums, and typedefs.
* See [Constant Values](#-constant-values) for rules on naming #define and constant variables.

✅ Do

```c
int sceneCount;    -- Non-constant variables use camelCase

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

char *smileModules[4];             -- Plural for non-string collections
char *greeting = "Hello Smile";    -- Singular for strings

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

for (int i = 0; i < max; i++)    -- Single-character name for loop iterator
{
    ...
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

float c = sqrt(pow(a, 2) + pow(b, 2));    -- Single-character name for mathematical formulas

```

❌ Don't

```c
int my_int = 1;    -- snake_case
int MyInt = 1;     -- PascalCase
int myint = 1;     -- lowercase

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int n = 10;    -- single-letter non-loop variable

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

char *smileModule[4];               -- Misleading! Doesn't indicate a collection
char *greetings = "Hello Smile";    -- Misleading! Indicates a collection
```

---

### — Variable Naming: Booleans

* Boolean variables should read naturally in conditionals and use consistent prefixes that indicate their boolean
  nature. Common names include (but aren't limited to):
    * `is`/`are` for states (`isVisible`, `areLightsOn`)
    * `has` for ownership or presence (`hasChildren`, `hasTexture`)
    * `should` for conditional behaviour (`shouldRender`, `shouldUpdate`)
    * `can` for capabilities (`canJump`, `canCastSpells`)
    * Present tense for existence checks (`fileExists`, `playerLives`)
* See [Type Conversion](#-type-conversion) for preferred boolean and pointer comparison styles.

✅ Do

```c
bool isRunning = true;    -- Prefixes clearly state they're booleans
bool canShoot = false;

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

if (isRunning && canShoot)    -- Reads naturally: "If is running and can shoot"
{
    ...
}
```

❌ Don't

```c
bool particles = false;    -- Misleading! Doesn't indicate it's a boolean


if (particles)    -- Doesn't read naturally for a boolean
{
    ...
}
```

---

### — File-Scoped Non-Constant Variables

* Declare all file-scoped variables as `static` to restrict access.

✅ Do

```c
static InternalTracker *tracker;
```

❌ Don't

```c
InternalTracker *tracker;    -- Externally linked by default
```

---

### — Variable Declaration and Grouping

* Declare each variable on its own line unless they represent the same logical unit.
* Don't group unrelated variables, even if they share a type.
* When declaring pointers:
    * Write the `*` next to the variable name, not the type.
    * Declare each pointer separately for clarity.
* ⚠️️ Never mix pointer and non-pointer variables in a single declaration.

✅ Do

```c
int width, height, depth;    -- Different logical units in separate lines
int price, change;

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

char *src;    -- Pointers declared in different lines
char *dst;
```

❌ Don't

```c
int width, height, depth, price, change;    -- Unrelated groups

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

char *src, *dst;    -- Unnecessarily verbose

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

char *name, grade;    -- Misleading! Only one is a pointer
```

---

### — Constant Values

* Use `const` for local constants inside functions only.
* For file- or module-scoped constants:
    * Use `#define` for primitive values and strings.
    * Use `enum` for related groups of integer constants.
    * Use `static const` for array/struct constants private to a file.
* All non-pointer constants must use SCREAMING_SNAKE_CASE.
* `const *` variables may use camelCase if the data is meant to be read-only (i.e., the pointer can change, but the
  content cannot).
* ⚠️ Never use `#define` to create function-like macros.

✅ Do

```c
#define DEFAULT_FPS 60    -- #define for primitives or strings

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef enum    -- Enums for related groups
{
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void myFunc(void)
{
    const int MAX_BUFFER_SIZE = 1024;    -- const inside a function
    char buffer[MAX_BUFFER_SIZE];
    ...
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

static const float IDENTITY_MATRIX[16] = {    -- static const for file-scoped array
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1
};

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

const InternalState *currScene;    -- Pointer can change, content cannot.

const InternalState *const CURR_STATE;    -- Neither pointer nor content can change.

```

❌ Don't

```c
#define LEVEL_USER 0       -- Related integer constants should use enum     
#define LEVEL_INFO 1
#define LEVEL_WARNING 2    

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

const int MAX_BUFFER_SIZE = 1024;    -- Wrong scope.

void myFunc(void)
{
    ...
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define SQUARE(x) ((x) * (x))    -- Function-like macro     

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

const InternalState *CURR_STATE;    -- Misleading! Indicates pointer cannot change.

```

---

### — Magic Numbers

* Avoid magic numbers. All repeated or meaningful values should be defined as constants.
* Exceptions (values that are self-explanatory or universally understood):
    * Loop iterators.
    * Return values from standard or external library functions.
    * Obvious mathematical formulas or operations.
    * `return 0` at the end of `main()`.
    * Obvious initializations or allocations.

✅ Do

```c
for (int i = 0; i < max; i++)    -- Loop iterators
{
    ...
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Return values from functions belonging to non-Smile libraries

bool isSameName = strcmp(newName,oldName) == 0

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

float c = sqrt(pow(a, 2) + pow(b, 2));    -- Mathematical formulas

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int next = current + 1;    -- Obvious oparations
bool isEven = num % 2 == 0;

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int main(void)
{
    ...
    return 0;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————


-- Obvious initializations or allocations

Type myStruct = { 0 };

InternalTracker *tracker = calloc(1, sizeof(InternalTracker));

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

#define DEFAULT_FPS 60    -- Meaningful or repeated values should be constants
```

❌ Don't

```c
float circumference =  2 * 3.14159f * radius;     -- Meaningful primitives should be #define'd

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

for (int i = 0; i < 10; i++)    -- Unclear! 10 is an arbitrary number
{
    ...
}

```

## 🛠️ Functions

### — Declaration and Usage

* Always include `void` as a parameter if the function takes no arguments in declarations and definitions. Omit when
  calling.

✅ Do

```c
bool smIsRunning(void);    -- Declaration

smIsRunning();    -- On call
```

❌ Don't

```c
bool smIsRunning();    -- Declaration not explicit

smIsRunning(void);    -- Call unnecessarily verbose
```

---

### — Naming: General

* All Smile functions begin with a unique module-identifying two-letter lowercase module prefix, followed by a
  PascalCase name. Below are two tables that relate modules and their prefixes.
* Function names should be verbs or verb phrases that describe the action. Common functions names include: `Start`,
  `IsRunning`, `Stop`, `Get`, `Set`, `Create`, `Delete`, and `Exists`.
* Using a consistent prefix prevents naming collisions, while shared names help users quickly infer a function’s purpose
  even in unfamiliar modules.

| Public Module  | Prefix |
|----------------|--------|
| Log            | lg     |
| ParticleSystem | ps     |
| SaveLoad       | sl     |
| SceneManager   | sm     |

| Internal Module | Prefix |
|-----------------|--------|
| _Common         | cm     |
| Test            | ts     |

✅ Example

```c
bool smStart(void);
bool smIsRunning(void);

bool smCreateScene(const char *name, smEnterFn enter, smUpdateFn update, smDrawFn draw, smExitFn exit);
bool smSceneExists(const char *name);
bool smSetScene(const char *name, void *args);
int smGetSceneCount(void);
bool smDeleteScene(const char *name);

bool smUpdate(float dt);
bool smDraw(void);

bool smStop(void);
```

---

### — Naming: Access Levels

* For different access levels, include the following after the prefix:
    * Public: Only module prefix.
    * Internal: Add `Internal` after prefix.
    * Private: Add `Private` after prefix.
    * Test: Add `Test` after prefix. See [4_Testing_Guidelines] (🚧 Under Development) for details.

✅ Example

```c
-- Found in SceneManager.h
bool smSceneExists(const char *name);

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Found in SceneManager.internal.h  
const State *smInternalGetScene(const char *name);

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Found in SceneManager.c
bool smPrivateIsNameValid(const char *name, const char *fnName);

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Found in SceneManagerAPITest.h
typedef void (*smTestExitFn)(MockData *data);

```

---

### — Parameters: General

* Function parameters should use `camelCase`.
* Choose descriptive names that make the argument's purpose clear.
* When appropriate, function argument order should make the signature read like natural language.

✅ Example

```c
-- "Log (level) from (origin) with (message) using (args)"
void lgPrivateLogV(InternalLevel level, const char *origin, const char *msg, va_list args);

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- "Delete state: (name)"
bool smDeleteScene(const char *name);
```

---

### — Parameters: Const

* Passed-by-reference should `const Type *ptr` when reading only.
* Copied-by-value parameters shouldn't be `const`.

✅ Do

```c
bool smSceneExists(const char *name);    -- Implies read-only parameter

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool smUpdate(float dt);    -- const is unecessary because argument is copied by value
```

❌ Don't

```c
bool smSceneExists(char *name);    -- Misleading! Implies the function modifies the string

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool smUpdate(const float dt);    -- Unnecessarily verbose
```

---

### — Type Conversion

* Use implicit conversion for nullptr checks in both return statements and conditional expressions.
* Use explicit comparisons for numeric and other non-pointer types to make conditions unambiguous.
* This approach maintains clarity while keeping code concise for the most common cases, and helps contributors quickly
  identify pointers without adding verbosity.

✅ Do

```c
-- Implicit conversion for pointer checks in return statements and conditionals

bool smIsRunning(void)
{
    return tracker; 
}

if (tracker)
{ 
    return false;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

if (playerCount > 0)    -- Explicit comparisons for non-pointer variables 
{
    ...
}
```

❌ Don't

```c
if (playerCount)    -- Misleading! Implies pointer
{
    ...
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool smIsRunning(void)
{
    return tracker != nullptr;    -- Unnecessarily verbose
}
```

---

### — Flow and Structure

* Skip no lines from the function signature to its first statement.
* Handle all failure or invalid conditions first and return early. This avoids deep nesting and keeps the main logic
  path clear.
* Group related operations together and separate conceptual steps with a single blank line.
* Use wrapper test functions for memory allocation. This facilitates testing.
* Use `Private` functions for statements that are repeated throughout multiple functions.
* Skip no lines before return statements.

✅ Example

```c
bool smStart(void)
{
    if (tracker)        -- Failure conditions first
    {
        return false;
    }

    -- Code blocks related to a single task go together (in this example, allocating memory and handling errors)
    tracker = tsInternalCalloc(1, sizeof(SceneManagerTracker));    -- Use wrapper test functions for memory allocation
    if (!tracker)
    {
        return false;
    }

    -- Success path
    tracker->fps = DEFAULT_FPS;
    return true;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Use Private functions for statements that are repeated throughout multiple functions

bool smSceneExists(const char *name)
{
    if (!smPrivateIsRunning(FN_SCENE_EXISTS))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_SCENE_EXISTS))
    {
        return false;
    }

    return smInternalGetEntry(name);
}
```

---

### — Return Types and Error Handling

* Return `bool` for functions that:
    * Would normally return void but require success/failure indication for testing purposes (e.g., `smStart`,
      `slStop`).
    * Semantically represent state checks.
* For numeric return types, use sentinel values like `-1` or `-1.0f` to indicate failure.
* For pointer return types, return `nullptr` when data is unavailable or operations fail.
* Return a `const` type if a certain data that belongs to Smile (not the user).
* Log all failures through the Log module with appropriate severity levels (
  See [InternalLog](/docs/_Internal/LogInternalAPI.md) for details).
* ⚠️ Always `return 0` from `main()`.

✅ Example

```c
-- Doesn't return any meaningful value for the user but returns bool for testing purposes

bool smStart(void)
{
    if (tracker)
    {
        lgInternalLog(WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(InternalTracker));
    if (!tracker)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return false;
    }

    tracker->fps = DEFAULT_FPS;

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return true;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Return bool for state checks

bool smIsRunning(void)
{
    return tracker;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Return -1, -1.0f or -1.0 on error for numeric return types

int smGetSceneCount(void)
{
    if (!smPrivateIsRunning(FN_GET_SCENE_COUNT))
    {
        return -1;
    }

    return tracker->sceneCount;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Return nullptr, if appropriate, when data is unavailable or operations fail.
-- This func also returns a const char pointer on success, meaning it belongs to
   Smile and the caller shouldn't modify or free it.
const char *smGetCurrentSceneName(void)
{
    if (!smPrivateIsRunning(FN_GET_CURRENT_SCENE_NAME))
    {
        return nullptr;
    }

    return tracker->currScene ? tracker->currScene->name : nullptr;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

int main(void)
{
    ...
    return 0;
}
```

---

### — goto

* Use `goto` only for cleanup paths to simplify error handling and prevent memory leaks. This improves maintainability
  and reduces duplicated free() or close() calls.
* ⚠️ Never use goto for non-error control flow (e.g., loops or jumps).

✅ Do

```c
-- Only use goto for cleanup

bool smCreateScene(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
                   smDrawFn drawFn, smExitFn exitFn)
{
    StateMap *entry = smInternalGetEntry(name);
    State *state = malloc(sizeof(State));
    ...
    
    char *nameCopy = malloc(strlen(name) + 1);
    if (!nameCopy)
    {
        ...
        goto nameCopyError;
    }
    
    ...

    StateMap *mapEntry = tsInternalMalloc(sizeof(StateMap));
    if (!mapEntry)
    {
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
start:
    printf("%d\n", i);
    i++;
    if (i < 5)
    {
        goto start;    -- Don't use goto to create a loop
    }
```

---

### — Shared Code and Messages

* Use `CommonInternal.h` for shared utility functions and `CommonInternalMessages.h` for shared log messages and error
  strings. Always check these files before creating new shared resources.
* Message definitions should have prefixes added as specifies bellow:

| Section Element | Prefix  |
|-----------------|---------|
| Functions Names | FN_     |
| Causes          | CAUSE_  |
| Consequences    | CONSEQ_ |

* The macro name should have English words separated by an underscore (`_`).
* The macro value should be the same as the macro name after the prefix but with spaces instead of underscores and
  following standard capitalization rules.
    * Function names are exceptions and should be written as the function is declared while omitting the module prefix.
* Do not add a period at the end of the macro value.

✅ Example

```c
-- In CommonInternalMessages.h
#define CONSEQ_ABORTED  "Aborted"

-- In SceneManagerMessages.h
#define CAUSE_SCENE_NOT_FOUND "Scene not found"
#define FN_SET_SCENE "SetScene"  -- Altogether, as this is the function's name
                                 -- Also, omit the module's prefix, so smSetScene
                                    becomes SetScene only

-- In SceneManager.c
bool smSetScene(const char *name, void *args)
{
    ...
    
    const InternalState *nextState = smInternalGetScene(name);
    if (!nextState)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND,
                             name,FN_SET_SCENE, CONSEQ_ABORTED);
        return false;
    }
    
    ...
}
```

## 📖 Formatting and Layout

Smile's `.clang-format` file is the source of truth for code formatting. When a
rule in this document and the formatter disagree, follow `.clang-format`.
Unless noted otherwise, Smile uses LLVM style as a baseline and overrides the
rules below.

### — Includes

* Order include groups in this sequence:
    * System headers: `<...>`
    * Project headers: `"..."`
    * Any remaining unmatched includes last
* Keep each include category grouped together.

✅ Do

```c
#include <stdbool.h>
#include <stddef.h>

#include "SceneManager.h"
#include "SceneManagerInternal.h"
```

❌ Don't

```c
#include "SceneManagerInternal.h"
#include <stdbool.h>
#include "SceneManager.h"
```

---

### — Braces

* Use brace-wrapping consistent with `.clang-format`:
    * Put the opening brace on the next line for functions, control statements, `struct`s, `enum`s, `union`s, classes,
      namespaces, and lambdas.
    * Put `else`, `catch`, and `while` on their own line before the opening brace.
    * Put the opening brace on the next line after a `case` label when the case body is wrapped in braces.
* Always use braces for function and control-statement bodies.

✅ Do

```c
bool smStart(void)
{
    if (tracker)
    {
        return false;
    }

    return true;
}

typedef enum
{
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;

switch (level)
{
    case INFO:
    {
        return;
    }
    default:
    {
        return;
    }
}
```

❌ Don't

```c
bool smStart(void) {
    return true;
}

typedef enum {
    USER,
    INFO,
} InternalLevel;
```

---

### — Pointers

* Write the pointer operator (`*`) next to the variable name, not the type.
* Leave no space between the dereference operator (`*`) and the variable.
* Use the arrow operator (`->`) for struct pointers without surrounding spaces.

✅ Do

```c
InternalTracker *tracker = nullptr;
*tracker = value;

Player *player = tracker->player;
```

❌ Don't

```c
InternalTracker* tracker = nullptr;
* tracker = value;

int score = player -> score;
```

---

### — Indentation and Empty Lines

* Use 4-column indentation.
* Indent `case` bodies one level inside the case label.
* Keep at most 2 consecutive empty lines.

✅ Do

```c
switch (state)
{
    case STATE_RUNNING:
    {
        update();
        break;
    }

    default:
    {
        break;
    }
}
```

❌ Don't

```c
switch (state)
{
case STATE_RUNNING:
{
    update();
    break;
}



default:
    break;
}
```

---

### — Spacing

* Do not put spaces inside parentheses, empty parentheses, angle brackets, or C-style cast parentheses.
* Do not put spaces before a function call's empty parameter list.
* Keep trailing comments unaligned; write them where they naturally fall.

✅ Do

```c
bool smIsRunning(void)
{
    return (count > 0) && (limit < max);
}

value = (float)count;
smStop();
```

❌ Don't

```c
bool smIsRunning( void )
{
    return ( count > 0 ) && ( limit < max );
}

value = ( float )count;
smStop( );
```

---

### — Line Length

* Keep lines within 120 columns (but aim for 80).
* When a declaration or call exceeds the limit, let the formatter wrap it across multiple lines.
* When a math expression wraps across multiple lines, start the continuation line with the operator.
* Do not manually align consecutive declarations, assignments, or trailing comments; let the formatter handle wrapped
  operand alignment.

✅ Do

```c
bool smCreateScene(const char *name, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn);

double tempDt = currentTime.tv_sec - tracker->lastTime.tv_sec
                + (currentTime.tv_nsec - tracker->lastTime.tv_nsec) / 1e9;
```

❌ Don't

```c
bool smCreateScene(const char *name, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn);

double tempDt = currentTime.tv_sec - tracker->lastTime.tv_sec+ (currentTime.tv_nsec - tracker->lastTime.
    tv_nsec) / 1e9;

int   width  = 1920;
float height = 1080.0f;
```

## 🃏 Miscellaneous

### — Typedef Preference

* Use `typedef` for defining `enums` and `structs`.

✅ Do

```c
typedef struct
{
    int x;
    int y;
} Point;

typedef struct Node
{
    int data;
    struct Node *next;
} Node;
```

❌ Don't

```c
struct Node
{
    int data;
    struct Node *next;
};
```

---

### — Type Naming

* Use `PascalCase` for all `struct`, `enum`, and `typedef` names.
* All public types must include the module prefix as part of their name.
* Prefix internal-only types with `Internal`.
* ⚠️ Never typedef primitive types.

✅ Do

```c
typedef struct
{    -- Internal type
    ...
} InternalState;

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef void (*smEnterFn)(void *args);    -- Public type
```

❌ Don't

```c
typedef int Number;    -- Hides primitive type

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

typedef void (*Enter)(void*);   -- No module prefix for a public type
```

---

### — nullptr

* Use `nullptr` (C23 standard) instead of `NULL` to avoid implicit conversions and ensure type safety.

✅ Do

```c
void *myFunction(void)
{
    ...
    return nullptr;
}
```

❌ Don't

```c
void *myFunction(void)
{
    ...
    return NULL;
}
```

---

### — Comments

* Comments should explain why, not what.
* Use `//` for short inline comments, and `/* ... */` for documentation blocks or temporarily disabling code.
* If a variable or function call’s purpose isn’t obvious, add a concise // comment. If a section needs heavy commenting
  to be understood, refactor it into a well-named function and document that instead (see [3_Documentation_Guidelines] (
  🚧 Under Development) for details).

✅ Do

```c
-- Document typedefs, functions, structs, enums... 
/**
 * @brief Function pointer type for state exit callbacks.
 *
 * @author Vitor Betmann
 */
typedef void (*smExitFn)(void);

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Comment on statements that are hard to understand at a glance
fprintf(stderr, "%s\n", SMILE_WHITE); // Reset Log color
```

❌ Don't

```c
-- Obvious comment
player.health = 100;  // player health set to 100

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Don't use single-line style for multi-line comments or vice-versas
// @brief Function pointer type for state exit callbacks.
// 
// @author Vitor Betmann
typedef void (*smExitFn)(void);
```

---

### — TODOS

* Place TODOs after a `@note` tag in the file header comment of the file they affect.
* If the TODO spans multiple lines, align all subsequent lines with the first word of the first line.
* Each TODO should match its corresponding GitHub issue title.
* Do not place TODOs in function bodies or scattered inline comments.
* For details on overall file header structure, see [3_Documentation_Guidelines] (🚧 Under Development).
* For details on naming and creating GitHub issues, see [6_Issued_And_Suggestions] (🚧 Under Development).

✅ Do

```c
/**
 * @file
 * @brief Implementation of the SceneManager module.
 * ...
 *
 * @note TODO #16 [Feature] for [SceneManager] - Create a function to limit the
 *       game's FPS to a max value
 * @note TODO #27 [Feature] for [SceneManager] - Create Internal Trim Function
 *       and Integrate into SceneManager Name Validation
 *
 * ...
 */
```

❌ Don't

```c
bool smSetFPS(int fps)
{
    // todo implement this    -- Don't include TODO's anywhere other than at the top of the file 
    return false;
}

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- These TODO's don't follow Smile's convention

// todo: fix this
// fix whitespace issue later
// TODO check names

————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

-- Dont' create really long TODO's

/*
 * @note TODO #42 this function is broken because of the complex memory allocation
 *       logic that needs to be refactored to avoid leaks when the initialization
 *       fails, see the following stack trace...
 */
```

---

### — Preprocessor Rules

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
#endif
```

## ➡️ Up Next

[3_Documentation_Guidelines] (🚧 Under Development)
