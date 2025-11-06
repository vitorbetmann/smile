# Coding Guidelines — Code Organization

This document defines Smile’s C coding standard conventions to ensure
consistency, readability, maintainability, and safety across all modules.

> Note:
> Smile follows the C23 standard for all C source code and headers.  
> All examples and conventions in this document assume C23-compatible compilers
> Features like `nullptr` and `bool` are used accordingly.

## Table of Contents

- [General Guidelines](#-code-organization)
- [Public Headers](#-_public-headers_)
- [Internal Headers](#-_internal-headers_)
- [Message Files](#-_message-files_)
- [Source Files](#-_source-files_)

## 🗂 General Guidelines

### Section Headers

* All code-related Smile files must be organized into sections, each marked by a
  standardized comment block, referred to as a section header.
* Leave 1 blank line between a section header and the first line of code in that
  section.
* Leave 2 blank lines between the end of one section and the start of the next
  section.

✅ Example

```c
// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <stdlib.h>


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————

#define GRAVITY 10
```

After the last line of a given section, skip 2 lines until the header of the
next one. Sections that might not be relevant to a certain file can be omitted.

Below is a structural Example

```c
// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <> — External libraries (e.g., stdlib.h, string.h) 

#include "" — Related to the current module (e.g., StateMachine.h, StateMachineInternal.h)

#include "" — Files that support the current module (e.g., LogInternal, TestInternal.h)


// —————————————————————————————————————————————————————————————————————————————
// Defines
// —————————————————————————————————————————————————————————————————————————————
 
#defines 


// —————————————————————————————————————————————————————————————————————————————
// Data Types
// —————————————————————————————————————————————————————————————————————————————

Data Types (e.g., enums, structs)


// —————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————

Variables


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

Prototypes

// —————————————————————————————————————————————————————————————————————————————
// Functions — Public
// —————————————————————————————————————————————————————————————————————————————

Public Functions definitions


// —————————————————————————————————————————————————————————————————————————————
// Functions — Internal
// —————————————————————————————————————————————————————————————————————————————

Internal Functions definitions


// —————————————————————————————————————————————————————————————————————————————
// Functions — Private
// —————————————————————————————————————————————————————————————————————————————

Private Functions definitions


// —————————————————————————————————————————————————————————————————————————————
// Main
// —————————————————————————————————————————————————————————————————————————————

main() Function
```

Below is an example using the StateMachine module:

```c
// —————————————————————————————————————————————————————————————————————————————
// Includes
// —————————————————————————————————————————————————————————————————————————————

#include <string.h>
#include <../external/uthash.h>

#include "../include/StateMachine.h"
#include "StateMachineInternal.h"
#include "StateMachineMessages.h"

#include "../src/Log/LogInternal.h"
#include "../src/_Internal/Common/CommonInternalMessages.h"
#include "../src/_Internal/Test/TestInternal.h"

// —————————————————————————————————————————————————————————————————————————————
// Variables
// —————————————————————————————————————————————————————————————————————————————

static StateMachineTracker *tracker;


// —————————————————————————————————————————————————————————————————————————————
// Prototypes
// —————————————————————————————————————————————————————————————————————————————

static bool smPrivateHasStarted(const char *fnName);

static bool smPrivateIsNameValid(const char *name, const char *fnName);


// —————————————————————————————————————————————————————————————————————————————
// Functions - Public
// —————————————————————————————————————————————————————————————————————————————

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

## Up Next

[Code_Style](Code_Style.md)

## ✏️ Last Modified

| Last modified | Author (Discord)             | Description       |
|---------------|------------------------------|-------------------|
| Nov 06, 2025  | Vitor Betmann (vitorbetmann) | Created document; |