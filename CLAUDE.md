# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What is Smile

Smile (`Simple Modularity Is Lowkey Elegant`) is a dependency-free C23 static library for 2D game development. It provides modules (`SceneManager`, `ParticleSystem`, `Log`, `SaveLoad`) and tools (`GenScene`). It builds as `libsmile.a` (Mac/Linux) or `smile.lib` (Windows).

## Build Commands

```zsh
# Developer mode (debug build + tests enabled)
cmake -S . -B build -DSMILE_DEV=ON
cmake --build build

# Run all tests
ctest --test-dir build -R Test --output-on-failure

# Run a single test target (e.g. TestAPISceneManager)
ctest --test-dir build -R TestAPISceneManager --output-on-failure

# Release build (user mode, no tests)
cmake -S . -B build
cmake --build build

# Install tools system-wide
sudo cmake --install build   # Mac/Linux
cmake --install build        # Windows (admin terminal)
```

Optional CMake flags:
- `-DSMILE_WARN=OFF` / `-DSMILE_INFO=OFF` — disable runtime warning/info logs at build time
- `-DSMILE_TESTS=ON` — enable tests without full `SMILE_DEV`

## Architecture

### Public vs Internal

- `include/` — public headers only (`Log.h`, `ParticleSystem.h`, `SceneManager.h`)
- `src/<Module>/` — public module: `<Module>.c`, `<Module>Internal.h`, `<Module>Messages.h`, `<Module>TestHooks.h`
- `src/internal/` — internal-only modules (`Common`, `Test`), never exposed publicly
- `tests/` — public API tests; no `tests/internal/` (internal code is exercised transitively)
- `external/` — third-party headers (`uthash.h`); not public API
- `docs/` — mirrors `src/` split: `docs/<Module>/` for public, `docs/internal/` for internal

### Module Lifecycle Patterns

- **Global-state modules** (`SceneManager`, `Log`): `xStart()` → use → `xStop()`
- **Per-instance modules** (`ParticleSystem`): `xCreate()` → use → `xDestroy()` (multiple instances can coexist)

### Internal Modules

**Common** (`src/internal/Common/`): Shared utilities across all modules.
- `cmResult` — shared result-code enum; Common's own codes are in `-1..-99`
- `CM_PATH_MAX` — max path length (256 bytes including null terminator)
- `cmIsRunning` — guard for verifying a module is active before API calls
- Filesystem helpers: `cmDirExists`, `cmValidatePath`, `cmCreateDir`, `cmFileExists`, `cmDeleteFile`, `cmDeleteDir`
- `CommonMessages.h` — check here before adding new log message strings (`CSE_` for causes, `CSQ_` for consequences)

**Test** (`src/internal/Test/`): Allocation and syscall interception for tests.
- `tsDisable(FN, n)` — makes the nth call to `FN` fail (`MALLOC`, `CALLOC`, `REALLOC`, `FOPEN`, `MKDIR`)
- `tsReset()` — clears all pending failures; call at the start of any test using `tsDisable()`
- `tsMalloc` / `tsCalloc` / `tsRealloc` / `tsFopen` / `tsMkdir` — wrappers production modules must call instead of libc so tests can intercept
- `tsMkdtemp(char *tmpl)` — portable `mkdtemp()` (not intercepted by `tsDisable`)
- `tsPass(__func__)` — prints `[PASS] fnName`; call at the end of every passing test
- `TS_MOCK_DT` — `0.016f` (~60 fps); use as delta-time in time-stepped tests

## Naming Conventions

**Module prefixes:**

| Module         | Prefix |
|----------------|--------|
| Log            | `lg`   |
| ParticleSystem | `ps`   |
| SaveLoad       | `sl`   |
| SceneManager   | `sm`   |
| Common         | `cm`   |
| Test           | `ts`   |

**Function naming:**
- Public: `smStart`, `psCreate`
- Module-private (in `*Internal.h`): `smInternalGetScene`
- File-private (`static`): `smPrivateIsNameValid`
- Modules entirely under `internal/` drop the `Internal` segment: `cmDirExists`, `tsMalloc`

**Types:** public types use module prefix (`smEnterFn`); per-module internal types add `Internal` (`smInternalScene`); types in `internal/` modules drop `Internal` (`cmResult`, `tsSysFn`).

## Coding Style

- C23 — use `nullptr` not `NULL`
- All headers use `#pragma once`
- 4-space indent, 100-column limit (see `.clang-format`)
- `Type *name` (pointer right), one pointer per declaration
- File-scoped variables must be `static`; no externally linked globals except for test hooks
- Includes: three groups separated by one blank line — angle-bracket headers, module headers, other Smile headers; alphabetical within each group
- Include only by filename, never by path
- `goto` only for cleanup paths in error handling
- Source files use named sections: `// SectionName ——————...` trailing to column 100

## Testing

Tests are standalone executables with a hand-rolled `main()` using `assert()` from `<assert.h>` — no external test framework.

Every test file requires:
```c
#ifdef NDEBUG
#error "<TargetName> must be compiled without NDEBUG (asserts required)."
#endif
```

**Naming:** `Test_<functionName>_<Behavior>` (e.g. `Test_psCreate_ReturnsNullWhenMaxParticlesIsZero`); stress tests prefix with `TestStress_`.

**Setup/teardown pattern:**
- Global-state modules: `setup()` calls `xStart()`, `teardown()` calls `xStop()`
- Per-instance modules: `setup()` calls `xCreate()`, `teardown()` calls `xDestroy()`

**TestHooks** (e.g. `SceneManagerTestHooks.h`): include under `#ifdef SMILE_DEV`; reset all hook pointers in `teardown()`.

**Adding a new test file:**
1. Create `tests/<Module>.c` or `tests/tools/<Tool>.c`
2. Register in `CMakeLists.txt` with `add_smile_test()` or `add_smile_tool_test()`

## Documentation

Every declaration in a public or internal header must have a Doxygen comment. Function pointers, typedefs, enum/struct type blocks, and variables use single-line `/** @brief … */`; struct fields and enum values use `/**< … */` trailing comments; function declarations use the multi-line block with `@brief`, `@param`, and `@return`.

Write `@return` as:
- `int` → `0 on success, or a negative result code on failure`
- `bool` → `true if X, false otherwise`
- Pointer → describe the value, state ownership (`"owned by <Module>"` vs `"caller-owned"`), and note when `nullptr` is returned

Docs mirror `src/` layout: `docs/<Module>/README.md` (getting started) + `docs/<Module>/<Module>API.md` (reference); internal modules get only `docs/internal/<Module>API.md`. Check `docs/CONVENTIONS.md` for the full Markdown style and section-header emoji set.
