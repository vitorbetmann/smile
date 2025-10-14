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

- [Smile Structure](#-smile-structure)
    - [Public Headers](#-_public-headers_)
    - [Internal Headers](#-_internal-headers_)
    - [Message Files](#-_message-files_)
    - [Source Files](#-_source-files_)

---

<br>

## 😊 Smile Structure

Talk about the structure of smile's directories

- Consistency: Each module should have its own <ModuleName>Messages.h
- Reusability: CommonMessages.h for shared messages.
- Maintainability: Internal functions go in ModuleNameInternal.h, helpers inside
  .c files only.
- naming files

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

| Version | Author        | Description      | Date         |
|---------|---------------|------------------|--------------|
| 1.0.0   | Vitor Betmann | Created document | Oct 14, 2025 |