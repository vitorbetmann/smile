# Smile Conventions

Use [`.clang-format`](../.clang-format) as the source of truth for formatting.
If this document conflicts with formatter output, follow `.clang-format`.

This document summarizes Smile's current coding and organization conventions,
with emphasis on project-specific rules that are not obvious from the
formatter alone.

## Language Standard

- Smile targets C23.
- Use C23 features when they improve clarity and safety.
- Use `nullptr` instead of `NULL`.

## Naming

### Variables and Members

- Use `camelCase` for non-constant variables and struct members.
- Boolean names should read naturally in conditionals, usually with prefixes
  such as `is`, `has`, `can`, `should`, or `are`.
- Avoid single-character names except for short-lived loop indices or obvious
  mathematical expressions.
- Use descriptive names and avoid non-standard abbreviations unless they are
  already common and obvious in context.
- Use plural names for collections unless the value is a string.

### Constants and Macros

- Use `SCREAMING_SNAKE_CASE` for named constants.
- Do not use function-like macros.
- Object-like macros are allowed for named constants and shared message
  definitions.
- Prefer named constants over repeated or meaningful literals.
- Prefer `enum` for related integer constants.
- Prefer `static const` for typed immutable data private to a source file.
- Local `const` values inside functions are allowed when they improve clarity,
  but avoid filling function bodies with one-off constants.

### Types

- Use the naming already established in Smile's codebase for types.
- Public type names use the module prefix, for example `smEnterFn`.
- Internal-only type names use the module prefix followed by `Internal`, for
  example `smInternalScene`.
- Result-code enums may use the module prefix plus a descriptive name, for
  example `smResult`.
- Do not typedef primitive types.

### Functions

- Smile function names begin with a two-letter lowercase module prefix followed
  by a `PascalCase` verb or verb phrase.
- Public functions use only the module prefix, for example `smStart`.
- Module-private internal functions use `Internal` after the prefix, for example
  `smInternalGetScene`.
- File-private helper functions use `Private` after the prefix and must be
  declared `static`, for example `smPrivateIsNameValid`.
- Test helpers use `Test` after the prefix.
- Function parameters use `camelCase`.

Current module prefixes:

| Module         | Prefix |
|----------------|--------|
| Log            | `lg`   |
| ParticleSystem | `ps`   |
| SaveLoad       | `sl`   |
| SceneManager   | `sm`   |
| _Common        | `cm`   |
| Test           | `ts`   |

## Variables and State

- Do not use externally linked global variables in Smile unless the use is
  test-specific.
- File-scoped variables must be declared `static`.
- Declare one variable per line by default.
- Multiple non-pointer variables may share a declaration only when they form
  one obvious logical unit.
- Declare pointers one per line.
- Write pointer declarations as `Type *name`.
- Do not mix pointer and non-pointer variables in the same declaration.

## Functions and Control Flow

- Declare no-argument functions with `void`.
- Passed-by-reference parameters should be `const Type *name` when the function
  only reads from them.
- Do not add `const` to copied-by-value parameters.
- Prefer implicit conversion for pointer and `bool` checks in simple
  conditionals and return statements.
- Use explicit comparisons for numeric non-boolean values.
- Prefer early returns for invalid or failure conditions.
- Group related steps with a single blank line when it improves readability.
- Use `Private` helpers for logic repeated across multiple functions.
- Use test allocation wrappers when code needs allocations that tests must
  intercept.
- Use `goto` only for cleanup paths in error handling.
- Do not use `goto` for ordinary control flow.

## Return Values and Errors

- Return `bool` for predicate-style APIs such as state checks, capability
  checks, and existence checks.
- Return `int` result codes when the caller may need to distinguish multiple
  failure reasons.
- Use shared or module-specific negative result codes for failures.
- Return `nullptr` from pointer-returning functions when data is unavailable or
  an operation fails.
- Return a `const` type when the returned data belongs to Smile and must not be
  modified by the caller.
- Log failures through the Log module with an appropriate severity.
- Return `0` from `main()` on success and a non-zero code on failure.
- Avoid magic numbers. Repeated or meaningful values should be named unless the
  value is already obvious in context, such as loop indices, standard-library
  return checks, simple arithmetic, zero-initialization, or `return 0` from
  `main()`.

## Internal Organization

- Public declarations belong in headers under `include/`.
- Module-private internal declarations belong in `[ModuleName]Internal.h`.
- Shared internal utilities belong under `internal`, typically in files such as
  `CommonInternal.h`.
- All headers must use include guards.
- Smile include guards begin with `SMILE_` and use underscores between words.
- Leave 2 blank lines before the closing `#endif` of an include guard and
  always comment what it closes.
- Source files must include everything they directly use. Do not rely on
  transitive includes or implied dependencies.
- Include statements should use only the file name, never an absolute or
  relative path.
- Order includes in three groups separated by 1 blank line:
  external headers in angle brackets, headers related to the current module,
  then supporting Smile headers.
- Keep includes alphabetical within each group.
- Public headers define user-facing APIs and public data types.
- Public headers should not declare variables.
- Internal headers define internal helpers and data types that support a
  module's implementation but are not public API.
- Source files implement public and internal headers and contain file-private
  `static` helpers.
- Check shared internal headers before introducing new cross-module helpers,
  messages, or error strings.

## File Layout

- Use Smile's file-generation tool to create new source files, public headers,
  internal headers, and message files, as the generated templates define
  Smile's standard boilerplate and file layout.
- Smile code files are organized into named sections marked by a standardized
  section-header comment block.
- Omit sections that do not apply to the file.
- Leave 1 blank line between a section header and the first line in that
  section.
- Leave 2 blank lines between the last line of one section and the next section
  header.
- Long groups of messages, data types, or functions may be split into
  subsections using comments in the form `// <Subsection Name>`.
- Leave 1 blank line between a subsection comment and functions or data types.
- In message files, leave no blank line between a subsection comment and the
  message definitions that follow it.

## Shared Messages

- Shared log and error message macros should use these prefixes:

| Element        | Prefix    |
|----------------|-----------|
| Function names | `FN_`     |
| Causes         | `CAUSE_`  |
| Consequences   | `CONSEQ_` |

- Macro names should use English words separated by underscores.
- Macro values should match the macro name after the prefix, using spaces and
  normal capitalization.
- Function-name message values should omit the module prefix and match the
  declared function name, for example `FN_SET_SCENE` -> `"SetScene"`.
- Do not end message macro values with a period.
- Message files define strings used for logging operation success, warnings,
  errors, and fatal conditions.
- Message files use this section order when sections are present:
  `Module Name`, `Functions Names`, `Causes`, `Consequences`.
- Message files should define the module name as `MODULE`.
- In `Causes`, subsections should follow the order `Infos`, `Warnings`,
  `Errors`, `Fatals`.
- In `Consequences`, subsections should follow the order `Success`, `Failure`, `Pause`.
- Check `CommonInternalMessages.h` before adding new module-specific messages so
  shared messages are reused consistently.

## Comments

- Prefer concise comments that explain intent, non-obvious constraints, or
  workarounds.
- Put comments before the code they describe unless a short trailing comment is
  clearer.
- If a block needs heavy commentary to be understandable, refactor it into a
  better-named helper instead.

## File and Directory Names

- Match the naming patterns already used in the repository.
- Public and module-specific source/header files typically use module-style
  names such as `SceneManager.h`, `SceneManagerInternal.h`, and
  `SceneManagerAPITest.c`.
- Shared internal directories and some support paths may use underscore-based
  names such as `internal` and `_Common`.
- Avoid spaces and special characters in new file and directory names.
