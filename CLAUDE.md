# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Smile is a dependency-free C23 static library of modules (Log, SceneManager, SaveLoad 🚧, ParticleSystem 🚧) that reduce boilerplate for 2D game development, plus command-line tools (GenScene). It builds as `libsmile.a` / `smile.lib` and exposes only the headers under `include/`.

## Build

All work is driven by CMake (≥3.30). There are two build personas:

```zsh
# User build (default): Release, no tests, no internal checks.
cmake -S . -B build
cmake --build build

# Developer build: Debug, tests ON, SMILE_DEV defined.
cmake -S . -B build -DSMILE_DEV=ON
cmake --build build
```

Flag reference (all `ON` by default except `SMILE_DEV`):

- `-DSMILE_DEV=ON` — developer mode. Flips `SMILE_TESTS` default to `ON`, sets `CMAKE_BUILD_TYPE=Debug` on single-config generators, and compiles `smile` with `-DSMILE_DEV`.
- `-DSMILE_TESTS=ON|OFF` — build test executables independently of `SMILE_DEV`.
- `-DSMILE_WARN=OFF` / `-DSMILE_INFO=OFF` — strip runtime warning/info logs at compile time. Error logs can never be disabled.

Pre-existing build directories in the repo: `build/`, `build-strict/`, `build_ci_check/`, `cmake-build-debug/`. Prefer configuring into one of these (don't invent new ones) unless the user asks.

## Test

Tests are CTest targets. They only exist when `SMILE_TESTS=ON` (implied by `SMILE_DEV=ON`).

```zsh
# Run the whole suite from the build directory.
ctest --test-dir build --output-on-failure

# Run a single test by name.
ctest --test-dir build -R TestAPISceneManager --output-on-failure

# Or run the executable directly for faster iteration.
./build/TestAPISceneManager
```

Current test targets (see `CMakeLists.txt` for the source of truth):

- `TestAPILog` — `tests/Log.c`
- `TestAPISceneManager` — `tests/SceneManager.c`
- `TestToolGenScene` — `tests/tools/GenScene.c`, compiled with `GS_TESTING` so it can link against `GenScene.c` without its `main`.

There is no `tests/internal/` — public-API tests are expected to exercise internal code transitively. Tool tests use `add_smile_tool_test` in `CMakeLists.txt`, which takes a `testing_define` that the tool's source guards its `main` behind.

## Formatting

`.clang-format` is the source of truth (C23, 4-space indent, 120 col, `Right` pointer alignment, braces on their own line via `BreakBeforeBraces: Custom`). If `docs/CONVENTIONS.md` conflicts with the formatter, the formatter wins.

## Architecture

### Public vs internal layout

The repo uses a convention that repeats under `src/`, `docs/`, and `tests/`:

- Anything directly under these roots is **public** (e.g., `src/Log/`, `src/SceneManager/`).
- `internal/` holds modules that support the implementation but are not public API (`src/internal/Common/`, `src/internal/Test/`).
- `tools/` holds standalone CLI executables (`src/tools/GenScene/`).
- `camelCase` directory = organizational bucket (`internal`, `tools`); `PascalCase` directory = an actual module/tool (`SceneManager`, `Common`, `GenScene`).
- There is no `Public/` directory — public is the default.

`include/` contains only public headers (`Log.h`, `SceneManager.h`). Everything else lives beside its implementation in `src/`.

### Anatomy of a public module

A typical public module (`SceneManager` is the canonical example) consists of:

1. `include/<Module>.h` — public API only. No variable declarations.
2. `src/<Module>/<Module>.c` — implements both public and internal functions, plus `static` file-private `Private` helpers.
3. `src/<Module>/<Module>Internal.h` — cross-TU-internal declarations for helpers needed by the module's own code or by tests, not by users.
4. `src/<Module>/<Module>Messages.h` — `CSE_` (causes) and `CSQ_` (consequences) message macros for logs/errors.
5. `src/<Module>/<Module>TestHooks.h` — test-only hooks that expose internal state to tests without leaking into the public API.

`Log` is the one exception: no `LogTestHooks.h`, because its public API already covers everything tests need.

### The `Common` and `Test` internal modules

Both live entirely under `src/internal/` and, by convention, their types and functions drop the `Internal` segment from their names since the parent directory already signals it.

- **Common** (`src/internal/Common/`, prefix `cm`) — cross-module utilities: the shared `cmResult` result-code enum (`RES_OK`, `RES_*` negatives, Common-exclusive range is `-1..-99`), `cmIsRunning` guard, filesystem helpers (`cmDirExists`, `cmValidatePath`, `cmCreateDir`, `cmFileExists`, `cmDeleteFile`, `cmDeleteDir`), and `CM_PATH_MAX`. `CommonMessages.h` holds shared `CSE_`/`CSQ_` message macros — check it before adding new module-specific messages.
- **Test** (`src/internal/Test/`, prefix `ts`) — allocation-interception wrappers (`tsMalloc`, `tsDisable`, …) that let tests force `MALLOC`/`CALLOC`/`REALLOC`/`FOPEN`/`MKDIR` failures at specific call counts. Modules that need test-controllable allocations should call these wrappers instead of the libc functions directly.

### Start → Use → Stop lifecycle

Public modules follow a uniform `Start → Use → Stop` shape (e.g., `smStart()` → `smCreateScene`/`smUpdate`/`smDraw` → `smStop()`). Modules own their memory internally; users interact through the module prefix only. Public APIs guard entry points with `cmIsRunning` before doing work.

### Naming at a glance

Two-letter lowercase module prefix + PascalCase verb. Prefix table:

| Module         | Prefix |
|----------------|--------|
| Log            | `lg`   |
| ParticleSystem | `ps`   |
| SaveLoad       | `sl`   |
| SceneManager   | `sm`   |
| Common         | `cm`   |
| Test           | `ts`   |

- `smStart` — public function.
- `smInternalGetScene` — declared in `SceneManagerInternal.h`.
- `smPrivateIsNameValid` — file-`static` helper inside the `.c`.
- `cmDirExists` / `tsMalloc` — internal modules drop the `Internal` segment.
- Types follow the same pattern: `smEnterFn` (public), `smInternalScene` (per-module internal), `cmResult` (under `internal/`).

Full style rules (C23 usage, include ordering, shared-message conventions, section-header layout, etc.) live in `docs/CONVENTIONS.md`. Read it before adding a new module or touching cross-cutting code.

## Tools

`GenScene` (`src/tools/GenScene/`) is a CLI that emits template `.h`/`.c` files for a new SceneManager scene into `include/` and `src/`. It is built as its own executable and installed to `bin/` via `cmake --install`. When modifying it, remember its test target (`TestToolGenScene`) compiles the same source with `GS_TESTING` defined — guard anything test-unfriendly (notably `main`) behind that macro.

## Docs

`docs/` mirrors the public/internal split:

- `docs/<Module>/` — public API docs + README with an overview and example.
- `docs/internal/` — internal API references (`CommonAPI.md`, `LogInternalAPI.md`, `SceneManagerInternalAPI.md`, `TestAPI.md`) and shared `Assets/` (GIFs, images, screenshots).
- `docs/tools/` — tool documentation.
- `docs/CONTRIBUTING.md` and `docs/CONVENTIONS.md` — contributor entry points.

Smile uses American English in prose (the repo ships a `LICENSE`, not a `LICENCE`) — prefer `behavior`, `license`, `color`, etc. in documentation.