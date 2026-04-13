# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What is Smile

**Smile** (Simple Modularity Is Lowkey Elegant) is a dependency-free, modular C23 static library for 2D game
development. It reduces boilerplate for common game-dev tasks. Every module follows a `Start → Use → Stop` lifecycle and
uses a two-letter lowercase prefix for all identifiers (e.g., `sm` for SceneManager, `lg` for Log).

## Build Commands

```bash
# Developer build (enables tests, debug mode, internal checks)
cmake -S . -B build -DSMILE_DEV=ON
cmake --build build

# Run all tests
ctest --test-dir build --output-on-failure

# Run a single test
ctest --test-dir build -R LogAPITest --output-on-failure
# Or run the binary directly:
./build/LogAPITest

# Format a file
clang-format -i <file>
```

**Build flags:**

- `-DSMILE_DEV=ON` — Debug build with tests enabled
- `-DSMILE_TESTS=ON|OFF` — Build test executables independently
- `-DSMILE_WARN=ON|OFF` — Runtime warning logs (default ON)
- `-DSMILE_INFO=ON|OFF` — Runtime info logs (default ON)

## Architecture

### Module Layout

Each module has a strict public/private split:

```
include/<Module>.h              ← Public API (only thing consumers see)
src/<Module>/<Module>.c         ← Implementation
src/<Module>/<Module>Internal.h ← Module-private structs/declarations
src/<Module>/<Module>Messages.h ← Log/error message macros
src/_Internal/Common/          ← Shared utilities across all modules (cm prefix)
src/_Internal/Test/             ← Test allocation interception utilities (ts prefix)
external/uthash.h               ← Bundled hash table (no other external deps)
tests/<Module>APITest.c         ← Public API tests (also include internal headers)
```

### Current Modules

| Module       | Prefix | Purpose                                                              |
|--------------|--------|----------------------------------------------------------------------|
| Log          | `lg`   | Printf-style terminal logging with severity levels and color         |
| SceneManager | `sm`   | Game scene/state management with transitions and per-frame callbacks |
| _Common      | `cm`   | Shared result codes, error messages, utilities                       |
| Test         | `ts`   | Allocation interception for test validation                          |

**Planned:** ParticleSystem (`ps`), SaveLoad (`sl`)

### SceneManager Data Flow

`smStart()` → register scenes via `smCreateScene(name, enter, update, draw, exit)` → `smSetScene(name, args)` switches
scenes → main loop calls `smUpdate(dt)` / `smDraw()` → `smStop()` tears down. Uses `uthash` for O(1) scene lookup by
name. Internally tracks a monotonic clock targeting 60 FPS.

### Dependency Graph

```
SceneManager → Log → (none)
SceneManager → _Common
Log → _Common
Tests → public API + Internal headers + TestInternal
```

## Conventions (from `docs/CONVENTIONS.md`)

**Naming:**

- Functions: `{prefix}{PascalCase}` — public: `smStart`, internal: `smInternalGetScene`, file-private static:
  `smPrivateIsNameValid`
- Variables/members: `camelCase`; constants/macros: `SCREAMING_SNAKE_CASE`
- Types: `smEnterFn` (public), `smInternalScene` (private)
- Boolean names: prefix with `is`, `has`, `can`, `should`, `are`

**Language:**

- C23 — use `nullptr` instead of `NULL`
- No compiler extensions (`CMAKE_C_EXTENSIONS OFF`)
- No externally linked globals; file-scope state must be `static`
- Pointer declarations: `Type *name` (one per line)

**Control flow:**

- Early returns for error/invalid conditions
- `goto` only for cleanup paths in error handling
- `bool` return for predicate APIs; `int` result codes when callers need to distinguish failures

**Headers:**

- Include guards prefixed `SMILE_`, with 2 blank lines before closing `#endif`
- Includes: external angle-bracket headers, then current-module headers, then supporting Smile headers — alphabetical
  within groups, 1 blank line between groups
- Source files must include everything they directly use (no transitive reliance)
- Use only the file name in includes, never a path

**File layout:** Use the file-generation tool (`tools/GenScene.c`) when creating new source files — it generates the
standard section-header boilerplate. Sections are separated by standardized comment blocks with 2 blank lines before
each header and 1 blank line after.

**Messages:** Check `CommonInternalMessages.h` before adding new message macros. Message macros use prefixes `FN_`,
`CAUSE_`, `CONSEQ_`.

## Testing Notes

- Tests use `assert.h` — must compile without `NDEBUG` (dev mode handles this)
- Test files include both public `include/` headers and private `*Internal.h` headers
- CI runs ASan + UBSan on Linux; logging is disabled in CI (`-DSMILE_WARN=OFF -DSMILE_INFO=OFF`)