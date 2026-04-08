# Coding Guidelines — Code Style 🧑‍💻

This document defines Smile’s C coding standard conventions to ensure
consistency, readability, maintainability, and safety across all modules.

> Note:
> Smile follows the C23 standard for all C source code and headers.
>
> All examples and conventions in this document assume C23-compatible compilers.
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

### — ⚠️ Never use externally linked global variables in Smile unless it's for testing.

---

## 🌦️ Variables and Constants

### — Variable Naming: General

* Use `camelCase` for all non-constant variable names.
* Avoid single-character names except for loop iterators or short mathematical expressions.
* Use descriptive names and avoid non-standard abbreviations unless common (e.g., `ptr`, `buf`, `len`).
* Use plural for collections unless it's a string.
* See [Type Naming](#-type-naming) for rules on naming structs, enums, and typedefs.
* See [Constant Values](#-constant-values) for rules on naming constants.

✅ Do

```c
int sceneCount; // Non-constant variables use camelCase

char *smileModules[4]; // Plural for non-string collections
char *greeting = "Hello Smile"; // Singular for strings

for (int i = 0; i < max; i++) // Single-character name for loop iterator
{
    ...
}

float c = sqrt(pow(a, 2) + pow(b, 2)); // Single-character name for mathematical formulas

```

❌ Don't

```c
int my_int = 1; // snake_case
int MyInt = 1; // PascalCase
int myint = 1; // lowercase

int n = 10; // Single-letter non-loop variable

char *smileModule[4]; // Misleading: doesn't indicate a collection
char *greetings = "Hello Smile"; // Misleading: indicates a collection
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
bool isRunning = true; // Prefixes clearly state they're booleans
bool canShoot = false;

if (isRunning && canShoot) // Reads naturally in conditionals
{
    ...
}
```

❌ Don't

```c
bool particles = false; // Name does not clearly indicate a boolean value
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
InternalTracker *tracker; // Externally linked by default
```

---

### — Variable Declaration and Grouping

* Declare each variable on its own line by default.
* You may declare multiple variables in a single declaration only when they represent the same logical unit and none
  of them are pointers.
* Don't group unrelated variables, even if they share a type.
* When declaring pointers:
    * Write the `*` next to the variable name, not the type.
    * Declare each pointer separately for clarity.
* ⚠️️ Never mix pointer and non-pointer variables in a single declaration.
* Separate declarations reduce ambiguity in diffs, initialization, and pointer readability.

✅ Do

```c
int width, height, depth; // Same logical unit, exception allowed

char *src; // Pointers declared on different lines
char *dst;
```

❌ Don't

```c
int sceneCount, windowWidth; // Unrelated variables

char *src, *dst; // Pointers should be declared separately

char *name, salary; // Misleading: only one is a pointer
```

---

### — Variable Qualifiers: Pointers

* Use pointer qualifiers to clearly communicate whether the pointer, the pointed-to data, or both are immutable.
* Use `camelCase` for ordinary read-only pointer variables.
* Use `SCREAMING_SNAKE_CASE` only when the variable itself is a named constant.

✅ Do

```c
const InternalState *currScene; // Pointer can change, content cannot.

InternalState *const currentScene = &scene; // Pointer cannot change, content can.

const InternalState *const DEFAULT_SCENE = &scene; // Neither pointer nor content can change.
```

❌ Don't

```c
const InternalState *CURR_SCENE; // Misleading: suggests the variable itself is a named constant
```

---

### — Constant Values

* Prefer named constants over unexplained repeated literals.
* Use local `const` values inside functions sparingly. Avoid filling function bodies with many one-off `const`
  declarations unless they clearly improve readability.
* For simple module-local compile-time constants that help keep a large implementation file tighter, use `#define` in
  `[ModuleName]Internal.h`.
* Use `enum` for related groups of integer constants.
* Use `static const` for typed immutable data private to a single source file, such as arrays, structs, and lookup
  tables.
* Named constants should use `SCREAMING_SNAKE_CASE`.
* ⚠️ Never use `#define` to create function-like macros.

✅ Do

```c
#define DEFAULT_FPS 60 // Simple module-local compile-time constant in an internal header

typedef enum // Enums for related groups
{
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;

void myFunc(void)
{
    const int MAX_BUFFER_SIZE = 1024; // Use local const sparingly when it improves readability
    char buffer[MAX_BUFFER_SIZE];
    ...
}

static const float IDENTITY_MATRIX[16] = { // static const for file-scoped array
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1
};

```

❌ Don't

```c
#define LEVEL_USER 0 // Related integer constants should use enum
#define LEVEL_INFO 1
#define LEVEL_WARNING 2

void myFunc(void)
{
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int FPS = 60; // Too many local one-off constants add noise
    ...
}

#define SQUARE(x) ((x) * (x)) // Function-like macro

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
for (int i = 0; i < max; i++) // Loop iterators
{
    ...
}

bool isSameName = strcmp(newName, oldName) == 0; // Return values from non-Smile libraries

float c = sqrt(pow(a, 2) + pow(b, 2)); // Mathematical formulas

int next = current + 1; // Obvious operations
bool isEven = num % 2 == 0;

int main(void)
{
    ...
    return 0;
}

Type myStruct = {0}; // Obvious initialization

InternalTracker *tracker = calloc(1, sizeof(InternalTracker)); // Obvious allocation

#define DEFAULT_FPS 60 // Meaningful or repeated values should be constants
```

❌ Don't

```c
float circumference = 2 * 3.14159f * radius; // Meaningful primitives should be named constants

for (int i = 0; i < 10; i++) // Unclear: 10 is an arbitrary number
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
bool smIsRunning(void); // Declaration

smIsRunning(); // Call
```

❌ Don't

```c
bool smIsRunning(); // Declaration not explicit

smIsRunning(void); // Call unnecessarily verbose
```

---

### — Naming: General

* All Smile functions begin with a unique module-identifying two-letter lowercase module prefix, followed by a
  PascalCase name. Below are two tables that relate modules and their prefixes.
* Function names should be verbs or verb phrases that describe the action. Common function names include: `Start`,
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

* Smile distinguishes between module-private internals and shared internal utilities:
    * Public: Only module prefix. Declare these in public headers under `include/`.
    * Module-Private Internal: Add `Internal` after the prefix. Declare these in `[ModuleName]Internal.h` and use them
      only within that module's implementation and tightly related private tests.
    * Shared Internal: Shared helpers under `_Internal` (for example, `_Common`) may also use `Internal` after the
      prefix when they are reused across modules or internal tools.
    * Private: Add `Private` after the prefix and declare the function `static`.
    * Test: Add `Test` after the prefix. See 4_Testing_Guidelines (🚧 Under Development) for details.

✅ Example

```c
// Found in SceneManager.h
bool smSceneExists(const char *name);

// Found in SceneManagerInternal.h
const smInternalScene *smInternalGetScene(const char *name);

// Found in CommonInternal.h
bool cmInternalIsRunning(bool (*isRunning)(void), const char *module, const char *fnName);

// Found in SceneManager.c
static int smPrivateIsNameValid(const char *name, const char *fnName);

// Found in SceneManagerAPITest.h
typedef void (*smTestExitFn)(MockData *data);

```

---

### — Parameters: General

* Function parameters should use `camelCase`.
* Choose descriptive names that make the argument's purpose clear.
* When appropriate, function argument order should make the signature read like natural language.

✅ Example

```c
// "Log (level) from (origin) with (message) using (args)"
void lgPrivateLogV(InternalLevel level, const char *origin, const char *msg, va_list args);

// "Delete state: (name)"
bool smDeleteScene(const char *name);
```

---

### — Parameters: Const

* Passed-by-reference parameters should be `const Type *ptr` when reading only.
* Copied-by-value parameters shouldn't be `const`.

✅ Do

```c
bool smSceneExists(const char *name); // Implies read-only parameter

bool smUpdate(float dt); // const is unnecessary because the argument is copied by value
```

❌ Don't

```c
bool smSceneExists(char *name); // Misleading: implies the function modifies the string

bool smUpdate(const float dt); // Unnecessarily verbose
```

---

### — Type Conversion

* Prefer implicit conversion for pointer and `bool` checks in return statements and simple conditional expressions.
* Use explicit comparisons for numeric non-boolean types to make conditions unambiguous.
* This approach maintains clarity while keeping code concise for the most common cases, and helps contributors quickly
  identify pointers without adding verbosity. Use explicit pointer comparisons when they materially improve clarity.

✅ Do

```c
// Implicit conversion for pointer and bool checks in return statements and conditionals

bool smIsRunning(void)
{
    return tracker;
}

if (isRunning)
{
    return false;
}

if (tracker)
{
    return false;
}

if (playerCount > 0) // Explicit comparisons for numeric non-boolean variables
{
    ...
}
```

❌ Don't

```c
if (playerCount) // Numeric non-boolean variables should use explicit comparisons
{
    ...
}

bool smIsRunning(void)
{
    return tracker != nullptr; // Usually unnecessary for simple pointer checks
}

if (isRunning == true) // Unnecessarily verbose for bool values
{
    return false;
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
    if (tracker) // Failure conditions first
    {
        return false;
    }

    tracker = tsInternalCalloc(1, sizeof(SceneManagerTracker)); // Use wrapper test functions for memory allocation
    if (!tracker)
    {
        return false;
    }

    tracker->fps = DEFAULT_FPS;
    return true;
}

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

* Return `bool` for predicate-style functions that semantically represent state checks, capability checks, or existence
  checks.
* Return `int` result codes for operations that can fail for multiple reasons and where the caller may need to know why
  the operation failed.
* Use shared or module-specific negative result codes for failure cases.
* For pointer return types, return `nullptr` when data is unavailable or operations fail.
* Return a `const` type when the returned data belongs to Smile rather than the user.
* Log all failures through the Log module with appropriate severity levels. See [InternalLog](../../LogInternalAPI.md)
  for details.
* Return `0` from `main()` on success.
* Return a non-zero exit status when the program fails.

✅ Example

```c
int smStart(void)
{
    if (tracker)
    {
        lgInternalLog(WARNING, MODULE, CAUSE_ALREADY_RUNNING, FN_START,
                      CONSEQ_ABORTED);
        return CM_RESULT_ALREADY_RUNNING;
    }

    tracker = tsInternalCalloc(1, sizeof(InternalTracker));
    if (!tracker)
    {
        lgInternalLog(ERROR, MODULE, CAUSE_MEM_ALLOC_FAILED, FN_START,
                      CONSEQ_ABORTED);
        return CM_RESULT_MEM_ALLOC_FAILED;
    }

    tracker->fps = DEFAULT_FPS;

    lgInternalLog(INFO, MODULE, CAUSE_MODULE_STARTED, FN_START,
                  CONSEQ_SUCCESSFUL);
    return CM_RESULT_OK;
}

bool smIsRunning(void)
{
    return tracker;
}

int smGetSceneCount(void)
{
    if (!smPrivateIsRunning(FN_GET_SCENE_COUNT))
    {
        return CM_RESULT_NOT_RUNNING;
    }

    return tracker->sceneCount;
}

// Return nullptr when data is unavailable or operations fail.
// This function also returns a const char pointer on success, meaning it
// belongs to Smile and the caller shouldn't modify or free it.
const char *smGetCurrentSceneName(void)
{
    if (!smPrivateIsRunning(FN_GET_CURRENT_SCENE_NAME))
    {
        return nullptr;
    }

    return tracker->currScene ? tracker->currScene->name : nullptr;
}

int main(void)
{
    ...
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) // Expected at least one user-provided argument
    {
        return 1;
    }

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
// Only use goto for cleanup

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
        goto start; // Don't use goto to create a loop
    }
```

---

### — Shared Code and Messages

* Keep module-private declarations in `[ModuleName]Internal.h`.
* Use `CommonInternal.h` for shared utility functions and `CommonInternalMessages.h` for shared log messages and error
  strings used across multiple modules or internal tools. Always check these files before creating new shared
  resources.
* Message definitions should have prefixes added as specified below:

| Section Element | Prefix  |
|-----------------|---------|
| Function Names  | FN_     |
| Causes          | CAUSE_  |
| Consequences    | CONSEQ_ |

* The macro name should have English words separated by an underscore (`_`).
* The macro value should be the same as the macro name after the prefix but with spaces instead of underscores and
  following standard capitalization rules.
    * Function names are exceptions and should be written as the function is declared while omitting the module prefix.
* Do not add a period at the end of the macro value.

✅ Example

```c
// In CommonInternalMessages.h
#define CONSEQ_ABORTED  "Aborted"

// In SceneManagerMessages.h
#define CAUSE_SCENE_NOT_FOUND "Scene not found"
#define FN_SET_SCENE "SetScene" // Omit the module prefix from the function name

// In SceneManager.c
bool smSetScene(const char *name, void *args)
{
    ...
    
    const InternalState *nextState = smInternalGetScene(name);
    if (!nextState)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_SCENE_NOT_FOUND,
                             name, FN_SET_SCENE, CONSEQ_ABORTED);
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
    * Put the opening brace on the next line for functions, control statements, `struct`s, `enum`s, and `union`s.
    * Put `else` and `while` on their own line before the opening brace.
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
* Prefix internal-only types with the module prefix followed by `Internal`.
* ⚠️ Never typedef primitive types.

✅ Do

```c
typedef struct
{ // Internal type
    ...
} smInternalState;

typedef void (*smEnterFn)(void *args); // Public type
```

❌ Don't

```c
typedef int Number; // Hides primitive type

typedef void (*Enter)(void *); // No module prefix for a public type
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

* Comments should be inline (if short) or placed before what they comment.
* Use `//` for short inline comments, and `/* ... */` for multi-line comments.
* In multi-line block comments, align the leading `*` on continuation lines and put closing `*/` on its own line.
* If a variable or function call’s purpose isn’t obvious, add a concise // comment. If a section needs heavy commenting
  to be understood, refactor it into a well-named function and document that instead (see
  3_Documentation_Guidelines (🚧 Under Development) for details).
* For file headers, API documentation blocks, and documentation tags, follow
  3_Documentation_Guidelines (🚧 Under Development).

✅ Do

```c
// Comment on statements that are hard to understand at a glance
fprintf(stderr, "%s\n", SMILE_WHITE); // Reset Log colour (short inline) 

/* Explain a multi-line workaround
 * or temporary limitation when needed.
 */
if (clock_gettime(CLOCK_MONOTONIC, &currTime) == -1)
{
    ...
}
```

❌ Don't

```c
tracker->fps = DEFAULT_FPS; // Assign default FPS to tracker (obvious comment)

/* Misaligned continuation lines
* make block comments harder to scan.
*/
```

---

### — Preprocessor Rules

* Use `#ifdef` for single condition checks.
* Use the `defined(...)` operator for multiple condition checks.
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

3_Documentation_Guidelines (🚧 Under Development)
