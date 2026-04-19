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
- Documentation Guidelines (🚧 Under Development)
- Testing Guidelines (🚧 Under Development)
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
- `tests/` currently contains only public API tests (`Log.c`, `SceneManager.c`,
  `tools/GenScene.c`). There is no `tests/internal/` — public-API tests are
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

- The common module contains code and messages shared across multiple modules.

#### The `Test` module

- The `Test` module lives entirely under `src/internal/Test/` and is not part
  of Smile's public API.
- It provides allocation-interception wrappers that tests use to validate
  internal behavior such as memory-allocation failure paths.
- Modules that need test-controllable allocations should call the `Test`
  wrappers instead of the standard allocators directly.

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
