# Coding Guidelines — Code Style 🧑‍💻

This document defines Smile’s C coding standard conventions to ensure
consistency, readability, maintainability, and safety across all modules.

> Note:
> Smile follows the C23 standard for all C source code and headers.  
> All examples and conventions in this document assume C23-compatible compilers
> Features like `nullptr` and `bool` are used accordingly.

## Table of Contents

- [Golden Rule](#-golden-rule)
- [Variables and Constants](#-variables-and-constants)
- [Functions](#-functions)
- [Formatting and Layout](#-formatting-and-layout)
- [Miscellaneous](#-miscellaneous)
- [Up Next](#-up-next)

## 🥇 Golden Rule

### ⚠️ Never use globals in Smile unless it's for testing.

## 🌦️ Variables and Constants

### Variable Naming — General

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
int stateCount;    -- Non-constant variables use camelCase

————————————————————————————————————————————————————————————————————————————————

char *smileModules[4];             -- Plural for non-string collections
char *greeting = "Hello Smile";    -- Singular for strings

————————————————————————————————————————————————————————————————————————————————

for (int i = 0; i < max; i++)    -- Single-character name for loop iterator
{
    ...
}

————————————————————————————————————————————————————————————————————————————————

float c = sqrt(pow(a, 2) + pow(b, 2));    -- Single-character name for mathematical formulas

```

❌ Don't

```c
int my_int = 1;    -- snake_case
int MyInt = 1;     -- PascalCase
int myint = 1;     -- lowercase

————————————————————————————————————————————————————————————————————————————————

int n = 10;    -- single-letter non-loop variable

————————————————————————————————————————————————————————————————————————————————

char *smileModule[4];               -- Misleading! Doesn't indicate a collection
char *greetings = "Hello Smile";    -- Misleading! Indicates a collection
```

### Variable Naming — Booleans

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
bool isRunning = true;    -- Prefixes clearly state they're booleans
bool canShoot = false;

————————————————————————————————————————————————————————————————————————————————

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

### File-Scoped Non-Constant Variables

* Declare all file-scoped variables as `static` to restrict access.

✅ Do

```c
static InternalTracker *tracker;
```

❌ Don't

```c
InternalTracker *tracker;    -- Externally linked by default
```

### Variable Declaration and Grouping

* Declare each variable on its own line unless they represent the same logical
  unit.
* Don't group unrelated variables, even if they share a type.
* When declaring pointers:
    * Write the `*` next to the variable name, not the type.
    * Declare each pointer separately for clarity.
* ⚠️️ Never mix pointer and non-pointer variables in a single declaration.

✅ Do

```c
int width, height, depth;    -- Different logical units in separate lines
int price, change;

————————————————————————————————————————————————————————————————————————————————

char *src;    -- Pointers declared in different lines
char *dst;
```

❌ Don't

```c
int width, height, depth, price, change;    -- Unrelated groups

————————————————————————————————————————————————————————————————————————————————

char *src, *dst;    -- Unnecessarily verbose

————————————————————————————————————————————————————————————————————————————————

char *name, grade;    -- Misleading! Only one is a pointer
```

### Constant Values

* Use `const` for local constants inside functions only.
* For file- or module-scoped constants:
    * Use `#define` for primitive values and strings.
    * Use `enum` for related groups of integer constants.
    * Use `static const` for array/struct constants private to a file.
* All non-pointer constants must use SCREAMING_SNAKE_CASE.
* `const *` variables may use camelCase if the data is meant to be read-only (
  i.e., the pointer can change, but the content cannot).
* ⚠️ Never use `#define` to create function-like macros.

✅ Do

```c
#define DEFAULT_FPS 60    -- #define for primitives or strings

————————————————————————————————————————————————————————————————————————————————

typedef enum {    -- Enums for related groups
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;

————————————————————————————————————————————————————————————————————————————————

void myFunc(void)
{
    const int MAX_BUFFER_SIZE = 1024;    -- const inside a function
    char buffer[MAX_BUFFER_SIZE];
    ...
}

————————————————————————————————————————————————————————————————————————————————

static const float IDENTITY_MATRIX[16] = {    -- static const for file-scoped array
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1
};

————————————————————————————————————————————————————————————————————————————————

const InternalState *currState;    -- Pointer can change, content cannot.

const InternalState *const CURR_STATE;    -- Neither pointer nor content can change.

```

❌ Don't

```c
#define LEVEL_USER 0       -- Related integer constants should use enum     
#define LEVEL_INFO 1
#define LEVEL_WARNING 2    

————————————————————————————————————————————————————————————————————————————————

const int MAX_BUFFER_SIZE = 1024;    -- Wrong scope.

void myFunc(void)
{
    ...
}

————————————————————————————————————————————————————————————————————————————————

#define SQUARE(x) ((x) * (x))    -- Function-like macro     

————————————————————————————————————————————————————————————————————————————————

const InternalState *CURR_STATE;    -- Misleading! Indicates pointer cannot change.

```

### Magic Numbers

* Avoid magic numbers. All repeated or meaningful values should be defined as
  constants.
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

————————————————————————————————————————————————————————————————————————————————

-- Return values from functions belonging to non-Smile libraries

bool isSameName = strcmp(newName,oldName) == 0

————————————————————————————————————————————————————————————————————————————————

float c = sqrt(pow(a, 2) + pow(b, 2));    -- Mathematical formulas

————————————————————————————————————————————————————————————————————————————————

int next = current + 1;    -- Obvious oparations
bool isEven = num % 2 == 0;

————————————————————————————————————————————————————————————————————————————————

int main(void)
{
    ...
    return 0;
}

————————————————————————————————————————————————————————————————————————————————


-- Obvious initializations or allocations

Type myStruct = { 0 };

InternalTracker *tracker = calloc(1, sizeof(InternalTracker));

————————————————————————————————————————————————————————————————————————————————

#define DEFAULT_FPS 60    -- Meaningful or repeated values should be constants
```

❌ Don't

```c
float circumference =  2 * 3.14159f * radius;     -- Meaningful primitives should be #define'd

————————————————————————————————————————————————————————————————————————————————

for (int i = 0; i < 10; i++)    -- Unclear! 10 is an arbitrary number
{
    ...
}

```

## 🛠️ Functions

### Declaration and Usage

* Always include `void` as a parameter if the function takes no arguments in
  declarations and definitions. Omit when calling.

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

### Naming — General

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
bool smSetState(const char *name, void *args);
int smGetStateCount(void);
bool smDeleteState(const char *name);

bool smUpdate(float dt);
bool smDraw(void);

bool smStop(void);
```

### Naming — Access Levels

* For different access levels, include the following after the prefix:
    * Public: Only module prefix.
    * Internal: Add `Internal` after prefix.
    * Private: Add `Private` after prefix.

✅ Example

```c
-- Found in StateMachine.h
bool smStateExists(const char *name);

————————————————————————————————————————————————————————————————————————————————

-- Found in StateMachine.internal.h  
const State *smInternalGetState(const char *name);

————————————————————————————————————————————————————————————————————————————————

-- Found in StateMachine.c
bool smPrivateIsNameValid(const char *name, const char *fnName);
```

### Parameters — General

* Function parameters should use `camelCase`.
* Choose descriptive names that make the argument's purpose clear.
* When appropriate, function argument order should make the signature read like
  natural language.

✅ Example

```c
-- "Log (level) from (origin) with (message) using (args)"
void lgPrivateLogV(InternalLevel level, const char *origin, const char *msg, va_list args);

————————————————————————————————————————————————————————————————————————————————

-- "Delete state: (name)"
bool smDeleteState(const char *name);
```

### Parameters — Const

* Passed-by-reference should `const Type *ptr` when reading only.
* Copied-by-value parameters shouldn't be `const`.

✅ Do

```c
bool smStateExists(const char *name);    -- Implies read-only parameter

————————————————————————————————————————————————————————————————————————————————

bool smUpdate(float dt);    -- const is unecessary because argument is copied by value
```

❌ Don't

```c
bool smStateExists(char *name);    -- Misleading! Implies the function modifies the string

————————————————————————————————————————————————————————————————————————————————

bool smUpdate(const float dt);    -- Unnecessarily verbose
```

### Type Conversion

* Use implicit conversion for nullptr checks in both return statements and
  conditional expressions.
* Use explicit comparisons for numeric and other non-pointer types to make
  conditions unambiguous.
* This approach maintains clarity while keeping code concise for the most common
  cases, and helps contributors quickly identify pointers without adding
  verbosity.

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

————————————————————————————————————————————————————————————————————————————————

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

————————————————————————————————————————————————————————————————————————————————

bool smIsRunning(void)
{
    return tracker != nullptr;    -- Unnecessarily verbose
}
```

### Flow and Structure

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
bool smStart(void)
{
    if (tracker)        -- Failure conditions first
    {
        return false;
    }

    -- Code blocks related to a single task go together (in this example, allocating memory and handling errors)
    tracker = tsInternalCalloc(1, sizeof(StateMachineTracker));    -- Use wrapper test functions for memory allocation
    if (!tracker)
    {
        return false;
    }

    -- Success path
    tracker->fps = DEFAULT_FPS;
    return true;
}

————————————————————————————————————————————————————————————————————————————————

-- Use Private functions for statements that are repeated throughout multiple functions

bool smStateExists(const char *name)
{
    if (!smPrivateIsRunning(FN_STATE_EXISTS))
    {
        return false;
    }

    if (!smPrivateIsNameValid(name, FN_STATE_EXISTS))
    {
        return false;
    }

    return smInternalGetEntry(name);
}
```

### Return Types and Error Handling

* Return `bool` for functions that:
    * Would normally return void but require success/failure indication for
      testing purposes (e.g., `smStart`, `slStop`).
    * Semantically represent state checks.
* For numeric return types, use sentinel values like `-1` or `-1.0f` to indicate
  failure.
* For pointer return types, return `nullptr` when data is unavailable or
  operations fail.
* Return a `const` type if a certain data that belongs to Smile (not the user).
* Log all failures through the Log module with appropriate severity levels (
  See [InternalLog](../../Log/LogInternalAPI.md) for details).
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

————————————————————————————————————————————————————————————————————————————————

-- Return bool for state checks

bool smIsRunning(void)
{
    return tracker;
}

————————————————————————————————————————————————————————————————————————————————

-- Return -1, -1.0f or -1.0 on error for numeric return types

int smGetStateCount(void)
{
    if (!smPrivateIsRunning(FN_GET_STATE_COUNT))
    {
        return -1;
    }

    return tracker->stateCount;
}

————————————————————————————————————————————————————————————————————————————————

-- Return nullptr, if appropriate, when data is unavailable or operations fail.
-- This func also returns a const char pointer on success, meaning it belongs to
   Smile and the caller shouldn't modify or free it.
const char *smGetCurrentStateName(void)
{
    if (!smPrivateIsRunning(FN_GET_CURRENT_STATE_NAME))
    {
        return nullptr;
    }

    return tracker->currState ? tracker->currState->name : nullptr;
}

————————————————————————————————————————————————————————————————————————————————

int main(void)
{
    ...
    return 0;
}
```

### goto

* Use `goto` only for cleanup paths to simplify error handling and prevent
  memory leaks. This improves maintainability and reduces duplicated free() or
  close() calls.
* ⚠️ Never use goto for non-error control flow (e.g., loops or jumps).

✅ Do

```c
-- Only use goto for cleanup

bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn,
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

### Shared Code and Messages

* Use `CommonInternal.h` for shared utility functions.
* Use `CommonInternalMessages.h` for shared log messages and error strings.
* This ensures consistency across modules and reduces duplication.
* Always check these files before creating new shared resources.

✅ Example

```c
-- In CommonInternalMessages.h
#define CONSEQ_ABORTED  "Aborted"

-- In StateMachineMessages.h
#define CAUSE_STATE_NOT_FOUND "State not found"
#define FN_SET_STATE "SetState"

-- In StateMachine.c
bool smSetState(const char *name, void *args)
{
    ...
    
    const InternalState *nextState = smInternalGetState(name);
    if (!nextState)
    {
        lgInternalLogWithArg(WARNING, MODULE, CAUSE_STATE_NOT_FOUND,
                             name,FN_SET_STATE, CONSEQ_ABORTED);
        return false;
    }
    
    ...
}
```

## 📖 Formatting and Layout

### Braces

* Always use braces, even for single-line conditionals or loops.
* For functions or statements, place the opening brace one line below it, and
  the closing
  brace one line after the final statement aligned with the start of the
  declaration.
* For a `struct`, an `enum`, or after an assignment operator (`=`) place the
  opening brace on the same line it's declared, and the closing
  brace one line after the final item aligned with the start of the declaration.
* Include a `struct` or `enum`'s name on the same line as the closing brace.

✅ Do

```c
int main(void)
{
    ...
    return 0;
}

if (false)
{
    return;
}

switch (value)
{
    case 1:
        ...
}

typedef enum {
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;

static const float IDENTITY_MATRIX[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1,
};
```

❌ Don't

```c
-- Wrong brace placement

int main(void) {
    ...
    return 0;
    
}

if (false)
{ return ; }

typedef enum
{
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL 
} InternalLevel;

static const float IDENTITY_MATRIX[16] =
{
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1,
};
    
————————————————————————————————————————————————————————————————————————————————

-- Inline statements
 
if (false) { return; }

if (false) return;

————————————————————————————————————————————————————————————————————————————————

-- Dangling statement

if (false) 
    return; 
```

### Pointers

* Write the pointer operator (`*`) next to the variable name, not the type.
* Leave no space between the dereference operator (`*`) and the variable.
* Use the arrow operator (`--`) to dereference struct pointers.
* ⚠️ Don’t use the dereference-dot pattern (`(*p).member`) or include spaces
  around
  the arrow.

* ✅ Do

```c
int *ptr = malloc(sizeof(int));
*ptr = 10;

————————————————————————————————————————————————————————————————————————————————

Player *player = &player;
int score = player->score;
```

❌ Don't

```c
int *ptr = malloc(sizeof(int));
* ptr = 10;    -- It should have no space between the dereference operator and the variable

————————————————————————————————————————————————————————————————————————————————

Player* p = &player;             -- Pointer operator should go next to the variable name
int playerScore = (*player).score;    -- Don't use the dereference-dot pattern
int playerId = player -> id;          -- No spaces around the arrow operator
```

### Commas and Other Operators

* Leave one space after each comma and around operators, except for the
  pointer, dereference, and arrow operators.
* A collection should be multi-line if it would be hard to comprehend. The
  lines should be indented.
* A `struct` or `enum` should always be multi-line.
* Always include a trailing comma for any collection.

✅ Do

```c
-- Collections that would be hard to comprehend as a single line should be multi-line
static const float IDENTITY_MATRIX[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0, 
    0, 0, 1, 0,
    0, 0, 0, 1,    -- Include trailing comma for multi-line collections
};

————————————————————————————————————————————————————————————————————————————————

typedef enum {    -- Enums are always multi-line
    USER,
    INFO,
    WARNING,
    ERROR,
    FATAL,
} InternalLevel;

————————————————————————————————————————————————————————————————————————————————

int myArray[] = { 0, 1, 2, 3, 4, };  -- Simple, short collections can be a single line long

————————————————————————————————————————————————————————————————————————————————

int sum = num1 + num2;    -- Include spaces around operators
```

❌ Don't

```c
int myArray[] = {0,1,2,3,4,};    -- No spaces after commas decreases readability

————————————————————————————————————————————————————————————————————————————————

-- Short, easy to grasp collection should be a single line long

int myArray[] = {
    0,
    1,
    2,
}

————————————————————————————————————————————————————————————————————————————————

int sum=num1+num2;    -- No spaces around operators decreases readability
```

### Indentation

* Indentation occurs in increments of 4 spaces.
* ⚠️ Never use tabs.

✅ Example

```c
while (true)
{
    for (int i = 0; i < max; i++)
    {
        ...
    }
}
```

### Line Length

* A line of code shouldn't extend over 80 characters.
* For long statements (function calls, `if`, `while`, `for`, etc.), break after
  a comma, operator, or logical boundary and align subsequent lines with the
  first argument.

✅ Do

```c
bool smCreateState(const char *name, smEnterFn enter, smUpdateFn update,
                   smDrawFn draw, smExitFn exit);
```

❌ Don't

```c
bool smCreateState(const char *name, smEnterFn enterFn, smUpdateFn updateFn, smDrawFn drawFn, smExitFn exitFn);
```

## 🃏 Miscellaneous

### Typedef Preference

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

### Type Naming

* Use `PascalCase` for all `struct`, `enum`, and `typedef` names.
* All public types must include the module prefix as part of their name.
* Prefix internal-only types with `Internal`.
* ⚠️ Never typedef primitive types.

✅ Do

```c
typedef struct {    -- Internal type
    ...
} InternalState;

————————————————————————————————————————————————————————————————————————————————

typedef void (*smEnterFn)(void *args);    -- Public type
```

❌ Don't

```c
typedef int Number;    -- Hides primitive type

————————————————————————————————————————————————————————————————————————————————

typedef void (*Enter)(void*);   -- No module prefix for a public type
```

### nullptr

* Use `nullptr` (C23 standard) instead of `NULL` to avoid implicit conversions
  and ensure type safety.

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

### Comments

* Comments should explain why, not what.
* Use `//` for short inline comments, and `/* ... */` for documentation blocks
  or temporarily disabling code.
* If a variable or function call’s purpose isn’t obvious, add a concise //
  comment. If a section needs heavy commenting to be understood, refactor it
  into a well-named function and document that instead (see
  [3_Documentation_Guidelines](../3_Documentation_Guidelines/1_General_Conventions.md)
  for details).

✅ Do

```c
-- Document typedefs, functions, structs, enums... 
/**
 * @brief Function pointer type for state exit callbacks.
 *
 * @author Vitor Betmann
 */
typedef void (*smExitFn)(void);

————————————————————————————————————————————————————————————————————————————————

-- Comment on statements that are hard to understand at a glance
fprintf(stderr, "%s\n", SMILE_WHITE); // Reset Log color
```

❌ Don't

```c
-- Obvious comment
player.health = 100;  // player health set to 100

————————————————————————————————————————————————————————————————————————————————

-- Don't use single-line style for multi-line comments or vice-versas
// @brief Function pointer type for state exit callbacks.
// 
// @author Vitor Betmann
typedef void (*smExitFn)(void);
```

### TODOS

* Place TODOs after a `@note` tag in the file header comment of the file they
  affect.
* If the TODO spans multiple lines, align all subsequent lines with the first
  word of the first line.
* Each TODO should match its corresponding GitHub issue title.
* Do not place TODOs in function bodies or scattered inline comments.
* For details on overall file header structure,
  see [Documentation Guidelines](../3_Documentation_Guidelines/1_General_Conventions.md).
* For details on naming and creating GitHub issues,
  see [Issued and Suggestions](../6_Issues_And_Suggestions/6_Issues_And_Suggestions.md).

✅ Do

```c
/**
 * @file
 * @brief Implementation of the StateMachine module.
 * ...
 *
 * @note TODO #16 [Feature] for [StateMachine] - Create a function to limit the
 *       game's FPS to a max value
 * @note TODO #27 [Feature] for [StateMachine] - Create Internal Trim Function
 *       and Integrate into StateMachine Name Validation
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

————————————————————————————————————————————————————————————————————————————————

-- These TODO's don't follow Smile's convention

// todo: fix this
// fix whitespace issue later
// TODO check names

————————————————————————————————————————————————————————————————————————————————

-- Dont' create really long TODO's

/*
 * @note TODO #42 this function is broken because of the complex memory allocation
 *       logic that needs to be refactored to avoid leaks when the initialization
 *       fails, see the following stack trace...
 */
```

### Preprocessor Rules

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

| Last modified | Author        | Description       |
|---------------|---------------|-------------------|
| Oct 18, 2025  | Vitor Betmann | Created document. |