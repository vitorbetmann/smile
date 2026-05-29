# 🤝 Contributing to Smile

## Thank you for your interest in contributing to Smile

This document will guide you through the contributor setup, repository
structure, and coding-guideline entry points.

---

## Table of Contents

- [Developer Setup](#-developer-setup)
    - [Differences Between DEV and USER Modes](#differences-between-dev-and-user-modes)
    - [Building Smile in Developer Mode](#building-smile-in-developer-mode)
- [Smile's Structure](#-smiles-structure)
    - [Project Root Directory](#project-root-directory)
    - [Public and Internal](#public-and-internal)
    - [Directory Breakdown](#directory-breakdown)
- [Coding Guidelines](#-coding-guidelines)
- [Documentation Guidelines](#-documentation-guidelines)
- [Testing Guidelines](#-testing-guidelines)
- Pull Requesting (🚧 Under Development)
- Issues and Suggestions (🚧 Under Development)

---

## ⌨️ Developer Setup

### Differences Between DEV and USER Modes

By default, Smile builds in User Mode, optimized for performance and minimal
build size. It excludes test executables and focuses on delivering the final
library.

Developer Mode is intended for contributors. It enables Smile's test
executables and internal test hooks, and provides additional output during
configuration to help with debugging and development.

### Building Smile in Developer Mode

#### Prerequisites

Before building Smile, make sure you have the following installed:

- **CMake** 3.30 or higher
- A CMake-supported build tool such as **Ninja** or **Make**
- A C compiler such as **Clang** or **GCC**

#### Cloning and Building

If you have not yet cloned Smile, run the commands below. These steps assume a
single-config generator:

```zsh
git clone https://github.com/vitorbetmann/smile.git
cd smile
```

Now you can compile it in Developer Mode:

```zsh
cmake -S . -B build -DSMILE_DEV=ON
cmake --build build
```

You should see output similar to the following:

```zsh
-- Smile — Build type: Debug  (override: -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel>)
-- Smile — Warning logs: ON  (override: -DSMILE_WARN=ON|OFF)
-- Smile — Info logs: ON  (override: -DSMILE_INFO=ON|OFF)
-- Smile — Build Tests: ON  (override: -DSMILE_TESTS=ON|OFF)
```

This confirms Smile is built in developer mode.

**Note:**  
By default, Smile compiles with runtime **warning** and **info** logs enabled.
If you want to disable them, build Smile with the following flags:

```zsh
cmake -S . -B build -DSMILE_DEV=ON -DSMILE_WARN=OFF -DSMILE_INFO=OFF
```

This disables Smile warning and info logging at build time. Errors cannot be
disabled.

### Running Tests

With a developer build in place, run the full test suite with:

```zsh
ctest --test-dir build -R Test --output-on-failure
```

The `-R Test` regex matches all test targets (`TestAPILog`, `TestAPIParticleSystem`,
`TestAPISceneManager`, `TestToolGenScene`). To run a single target, use its full name

---

## 🏛 Smile's Structure

### Project Root Directory

At the project root, you'll find a structure similar to this:

```zsh
Smile
├── CMakeLists.txt
├── .gitignore
├── LICENSE
├── README.md
├── build/
├── docs/
├── external/
├── include/
├── src/
└── tests/
```

### Public and Internal

- Smile uses two organizational subdirectories that can appear under `src/`,
  `docs/`, or `tests/`:
    - `internal/` holds developer-facing modules and documentation that support
      Smile's implementation but are not part of the public API.
    - `tools/` holds standalone command-line tools such as `GenScene`.
- Within a directory that uses this convention, anything outside `internal/`
  and `tools/` is considered public.
- Directory naming follows a convention: `camelCase` names (`internal`,
  `tools`) are organizational buckets that group related modules or tools;
  `PascalCase` names (`SceneManager`, `Log`, `Common`, `GenScene`) are the
  actual modules or tools.
- Public modules and tools do not live under a dedicated `Public` directory —
  their absence from `internal/` or `tools/` makes them public by default.
- `tests/` currently contains only public API tests (`Log.c`, `ParticleSystem.c`,
  `SceneManager.c`, `tools/GenScene.c`). There is no `tests/internal/` — public-API tests are
  expected to exercise internal code transitively.

### Directory Breakdown

#### `external/`

The `external/` directory contains third-party libraries used internally to
implement Smile's modules. It is not part of Smile's public API.

#### `include/`

The `include/` directory contains public headers only.

#### `src/`

Public Smile modules commonly include these files:

1. `<ModuleName>.c` - Implements the module's public and internal functions.
2. `<ModuleName>Internal.h` - Declares internal helper functions needed by the
   module but not exposed as public API.
3. `<ModuleName>Messages.h` - Defines module-specific log and error messages.
4. `<ModuleName>TestHooks.h` - Exposes test-only hooks needed to validate
   internal behavior without making those details part of the public API.

> Note: the `Log` module is the current exception — it does not define a
> `LogTestHooks.h`, because its public API already exposes everything tests need.

#### The `Log` module

- The `Log` module serves both as Smile's internal logger and as a user-facing
  debugging log API.
- Because both roles share the same underlying implementation, `Log` remains in
  a single public module directory.

#### The `Common` module

- The `Common` module lives entirely under `src/internal/Common/` and is not part
  of Smile's public API.
- It provides utilities shared across multiple modules:
    - `cmResult` — shared result-code enum; the `-1..-99` range is exclusive to Common.
    - `CM_PATH_MAX` — maximum allowed path length (256 bytes, including null terminator).
    - `cmIsRunning` — guard that verifies a module is running before a public API call proceeds.
    - Filesystem helpers: `cmDirExists`, `cmValidatePath`, `cmCreateDir`, `cmFileExists`,
      `cmDeleteFile`, `cmDeleteDir`.
- `CommonMessages.h` defines shared `CSE_`/`CSQ_` string constants. Check it before
  adding new module-specific messages so shared strings are reused consistently.

#### The `Test` module

- The `Test` module lives entirely under `src/internal/Test/` and is not part
  of Smile's public API.
- It provides interception wrappers for system functions that tests use to validate
  internal behavior, including memory-allocation and file-system failure paths:
  `tsMalloc`, `tsCalloc`, `tsRealloc`, `tsFopen`, `tsMkdir`.
- Use `tsDisable(FN, n)` to make the nth call to a wrapped function fail, and
  `tsReset()` at the start of any test that uses `tsDisable()` to clear state.
- Additional helpers: `tsMkdtemp` (portable `mkdtemp()`), `tsPass` (prints a
  `[PASS]` line), and `TS_MOCK_DT` (mock delta-time, ≈ 60 fps).
- Modules that need test-controllable system calls should call these wrappers
  instead of the standard library functions directly.

#### `docs/`

The `docs/` directory contains documentation for both public and internal APIs,
as well as `CONTRIBUTING.md` (this file) and [CONVENTIONS.md](CONVENTIONS.md).

Public modules and tools have their own directories that typically include both API reference
material and a README with an overview and workflow example.

Internal APIs are documented directly under `docs/internal/` in files such as
`CommonAPI.md`, `LogInternalAPI.md`, `SceneManagerInternalAPI.md`, and
`TestAPI.md`. `docs/internal/` also stores GIFs, images, and videos under
`Assets/` for use in README files.

---

## 🧑‍💻 Coding Guidelines

Please refer to [CONVENTIONS.md](CONVENTIONS.md) for Smile's conventions on code
organization and style.

---

## 📝 Documentation Guidelines

### File Placement

Documentation mirrors the `src/` public/internal split:

| Content                      | Location                |
|------------------------------|-------------------------|
| Public module docs           | `docs/<Module>/`        |
| Internal module docs         | `docs/internal/`        |
| Tool docs                    | `docs/tools/`           |
| Shared assets (GIFs, images) | `docs/internal/Assets/` |

New public modules need a `docs/<Module>/` directory. New internal modules get a
single file directly under `docs/internal/`.

### Two-Tier Structure for Public Modules

Every public module ships two Markdown files.

#### `README.md` — Getting Started

Intended for first-time users. Sections in order:

1. One-to-two sentence description and module contract.
2. Thread-safety warning (`### 🚨 Warning! This module is not thread-safe!`) if
   applicable.
3. Table of Contents.
4. Visual example — a GIF or screenshot hosted in `docs/internal/Assets/` or an
   external URL.
5. Module Header — the include path and a `✅ Example`.
6. Lifecycle — numbered steps (1️⃣ 2️⃣ …) covering the module's lifecycle arc.
   The shape depends on the module's state model:
    - **Global-state modules** (e.g. `SceneManager`) follow `Start → Use → Stop`.
      `xStart()` initializes a single shared state; `xStop()` tears it down.
    - **Per-instance modules** (e.g. `ParticleSystem`) follow `Create → Use →
      Destroy`. `xCreate()` returns a caller-owned instance; `xDestroy()` frees it.
      Multiple instances can coexist.
7. Quick Reference Table — function signatures and one-line descriptions.
8. Workflow Example — complete, runnable code demonstrating a typical use case.

#### `<Module>API.md` — API Reference

Intended for look-up. Sections in order:

1. One-to-two sentence description.
2. Cross-reference links to the Getting Started README and the Internal API doc
   (if one exists).
3. Thread-safety warning if applicable.
4. Table of Contents.
5. Module Header — same as README.
6. Data Types — sub-divided into Function Pointers, Enums, and Structs; omit
   unused kinds.
7. Functions — organized by logical category matching the header's subsection
   comments.
8. Variables — if the module exports named constants.

Each entry (type or function) uses a single-cell Markdown table for its
signature, followed by a prose description, a Parameters list, a Returns
description, optional Notes, and a `✅ Example` code block.

Internal modules (`Common`, `Test`) ship only a `<Module>API.md` under
`docs/internal/` — no README.

### Markdown Style

- Use emoji section headers consistently. Common set:
    - 📋 Table of Contents
    - 😊 Module Header
    - 👀 Visual Example
    - 🔄 Lifecycle
    - 🔍 Quick Reference
    - 📦 Data Types
    - 🛠️ Functions
    - 🚨 Warnings
- Delimit sections with `---`.
- Put function or type signatures in a single-cell Markdown table:
  ```md
  | `returnType functionName(params)` |
  |------------------------------------|
  ```
- Prefix all code examples with `✅ Example`.
- Use `c` as the fenced-code language specifier.
- Write prose in **American English** (`behavior`, `color`, `license`).
- Keep descriptions concise. One sentence per entry; expand in a Notes list for
  non-obvious behavior, ownership, or side effects.

### In-Header Documentation (Doxygen)

Refer to [CONVENTIONS.md § Doxygen](CONVENTIONS.md#doxygen) for full formatting
rules. Key points:

- Every declaration in a public or internal header **must** have a Doxygen
  comment.
- Single-line `/** @brief … */` for typedefs, function pointer types, enum/struct
  type blocks, and variables.
- `/**< … */` inline trailing comment for enum values and struct fields.
- Multi-line block for function declarations:
  ```c
  /**
   * @brief One-sentence summary.
   *
   * @param name Description.
   *
   * @return …
   */
  ```
- `@return` wording by return type:
    - **`int`** — `0 on success, or a negative result code on failure`.
    - **`bool`** — `true if X, false otherwise`.
    - **Pointer** — describe what the pointer refers to, state ownership, and
      note when `nullptr` is returned. Use `"owned by <Module>"` when the module
      retains ownership (caller must not free), or `"caller-owned"` when the
      caller is responsible for freeing. Examples:
        - `Pointer to the current scene name (owned by SceneManager), or nullptr
          if no scene is active or SceneManager is not running.`
        - `Pointer to the new ParticleSystem (caller-owned), or nullptr on
          failure.`

---

## 🧪 Testing Guidelines

### File Placement

| Content              | Location               |
|----------------------|------------------------|
| Public module tests  | `tests/<Module>.c`     |
| Tool tests           | `tests/tools/<Tool>.c` |

There is no `tests/internal/` — public API tests are expected to exercise internal code transitively.

### Test Targets

| CTest Target            | Source                   |
|-------------------------|--------------------------|
| `TestAPILog`            | `tests/Log.c`            |
| `TestAPIParticleSystem` | `tests/ParticleSystem.c` |
| `TestAPISceneManager`   | `tests/SceneManager.c`   |
| `TestToolGenScene`      | `tests/tools/GenScene.c` |

### Naming Convention

- **Regular tests:** `Test_<functionName>_<Behavior>` — e.g., `Test_psCreate_ReturnsNullWhenMaxParticlesIsZero`
- **Stress tests:** `TestStress_<functionName>_<Behavior>`
- Call `tsPass(__func__)` at the end of every passing test function.

### Test Structure

Each test file is a standalone executable with a hand-rolled `main()` that calls every test function in order, grouped by logical category using `puts()` banners. Tests rely on `assert()` from `<assert.h>` — there is no external test framework.

Every test file must include a compile-time guard against `NDEBUG`:

```c
#ifdef NDEBUG
#error "<TargetName> must be compiled without NDEBUG (asserts required)."
#endif
```

### Setup and Teardown

Use static `setup()` and `teardown()` helpers to bracket each test:

- **Global-state modules** (e.g., `SceneManager`): `setup()` calls `xStart()`; `teardown()` calls `xStop()`.
- **Per-instance modules** (e.g., `ParticleSystem`): `setup()` calls `xCreate()`; `teardown()` calls `xDestroy()`.

### The Test Module (`tsXxx` Helpers)

The internal `Test` module (`src/internal/Test/`) provides allocation-interception wrappers and failure-simulation helpers. Include it via `"Test.h"`. Full reference in [TestAPI.md](internal/TestAPI.md).

| Helper | Description |
|--------|-------------|
| `tsPass(const char *fnName)` | Prints `[PASS] fnName` on success. |
| `tsDisable(tsSysFn fn, unsigned int at)` | Makes the `at`-th call to `fn` return a failure; auto-resets after firing. Values: `MALLOC`, `CALLOC`, `REALLOC`, `FOPEN`, `MKDIR`. |
| `tsReset()` | Clears all pending failures. Call defensively at the start of any test that uses `tsDisable()`. |
| `tsMalloc` / `tsCalloc` / `tsRealloc` / `tsFopen` / `tsMkdir` | Wrappers that production modules must call instead of the libc functions so `tsDisable()` can intercept them. |
| `tsMkdtemp(char *tmpl)` | Portable `mkdtemp()` for temporary test directories. **Not** intercepted by `tsDisable(MKDIR, n)`. |
| `TS_MOCK_DT` | `0.016f` (≈ 60 fps). Use as the delta-time argument when calling time-stepped functions in tests. |

✅ Example — forcing an allocation failure:

```c
void Test_psCreate_ReturnsNullWhenCallocFails(void)
{
    tsReset();
    tsDisable(CALLOC, 1);
    ps = psCreate(MAX_PARTICLES, ORIGIN_X, ORIGIN_Y);
    assert(!ps);
    tsPass(__func__);
}
```

### TestHooks

Modules that ship a `*TestHooks.h` (currently only `SceneManager`) expose `extern` variables that tests can set to intercept callbacks and mock system behavior. Always include them under `#ifdef SMILE_DEV`, and reset every hook pointer in `teardown()` to prevent cross-test pollution.

✅ Example — intercepting a scene enter callback:

```c
#ifdef SMILE_DEV
#include "SceneManagerTestHooks.h"
#endif

static void onEnter(MockData *data) { data->enterCount++; }

static void resetHooks(void)
{
    smTestEnter             = nullptr;
    smTestEnterWithArgs     = nullptr;
    smTestExit              = nullptr;
    smMockData              = nullptr;
    smMockArgs              = nullptr;
    smMockCurrTime          = (struct timespec){0};
    smMockClockGettimeFails = false;
}

static void teardown(void)
{
    resetHooks();
    assert(smStop() == RES_OK);
}
```

### Adding Tests for a New Module

1. Create `tests/<Module>.c` (public module) or `tests/tools/<Tool>.c` (tool).
2. Register the target in `CMakeLists.txt` using `add_smile_test()` or `add_smile_tool_test()`.
3. Add the `NDEBUG` compile-time guard.
4. Write test functions following the `Test_<functionName>_<Behavior>` naming convention.
5. Wire them up in `main()` with `puts()` section banners and explicit function calls.

---
