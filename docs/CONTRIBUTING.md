# 🤝 Contributing to Smile 

## Thank you for your interest in contributing to Smile!

This document will guide you through everything you need to know to work
effectively within the project.

<!-- summary of what this doc contains -->

---

## Table of Contents

- [Developer Setup](#-developer-setup)
  - [Differences Between DEV and USER Modes](#-differences-between-dev-and-user-modes)
  - [Building Smile In Developer Mode](#-building-smile-in-developer-mode)
- [Smile's Structure](#-smiles-structure)
  - [Project Root Directory](#-project-root-directory)
  - [Public and Internal](#-public-and-internal)
  - [Directory Breakdown](#-directory-breakdown)
- Coding Guidelines (🚧 Under Development)
- Documentation Guidelines (🚧 Under Development)
- Testing Guidelines (🚧 Under Development)
- Pull Requesting (🚧 Under Development)
- Issues and Suggestions (🚧 Under Development)

---

# ⌨️ Developer Setup

## ⚖️ Differences Between DEV And USER Modes

By default, Smile builds in User Mode, optimized for performance and minimal
build size. It excludes development tools and test files, focusing on delivering
the final library or application.

Developer Mode, on the other hand, is intended for contributors. It enables
extra build steps, includes internal tests, and provides additional output
during configuration to help with debugging and development.


## 🚀 Building Smile in Developer Mode

### Prerequisites

Before building Smile, make sure you have the following installed:

- **CMake** 3.30 or higher
- A CMake-supported build tool such as **Ninja**, **Make**
- A C compiler such as **Clang** or **GCC**

### Cloning and Building

If you have not yet cloned Smile, run the commands below (steps assume a 
single-config generator):

```zsh
git clone https://github.com/vitorbetmann/smile.git
cd smile
```

Now you can compile it in Developer Mode:

```zsh
cmake -S . -B build -DSMILE_DEV=ON
cmake --build build
```

You should see the following output:

```zsh
-- Smile — Build type: Debug  (override: -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel>)
-- Smile — Warning logs: OFF  (override: -DSMILE_WARN=ON|OFF)
-- Smile — Info logs: OFF  (override: -DSMILE_INFO=ON|OFF)
-- Smile — Build Tests: ON  (override: -DSMILE_TESTS=ON|OFF)
```

This confirms Smile is built in developer mode.

**Note:**  
By default, Smile compiles with runtime **warning** and **info** logs enabled.
Below is an example of how they would appear in your terminal:

![Example of Smile Logs](https://raw.githubusercontent.com/vitorbetmann/smile/refs/heads/main/docs/_Internal/Assets/_SmileREADME/LogExample.png)

If you want to disable them, build Smile command with the following flags:

 ```zsh
 cmake -S . -B build -DSMILE_DEV=ON -DSMILE_WARN=OFF -DSMILE_INFO=OFF
 ```

This will disable all Smile **warning** and **info** logging output at build
time. Errors cannot be disabled.

---

# 🏛️ Smile's Structure

## 🌳 Project Root Directory

At the project root, you'll find a structure similar to this:

```zsh
Smile
├── CMakeLists.txt
├── .gitignore
├── LICENCE
├── README.md
├── build/
├── docs/
├── external/
├── include/
├── src/
└── tests/
```

## 🔑 Public and Internal

- Smile separates between user- and developer-facing components, referred to as 
`Public` (intended for users) and `Internal`(intended for Smile developers).
- In the `docs` and `src` directories, `_Internal` holds developer-focused
documentation, functions, and modules.
- In the `tests` directory, `_Internal` holds, tests of internal modules.
- Directories and files of `Public` modules and tools don't go under a specific
folder.
- `_Internal` directories begin with an underscore (`_`) to easily sort it 
alphabetically. 
- Within directories that use this convention, anything outside `_Internal` is
considered `Public`.

## 🧩 Directory Breakdown

### — external/

The `external/` directory contains third-party libraries. They're used 
internally to implement Smile's modules. Not meant for users.

### — include/

Only contains header files for public modules.

### — src/

`Public` Smile modules are expected to include four key files:

1. `<ModuleName>.c` – Implements the module's public and internal functions.
2. `<ModuleName>Internal.h` – Contains internal helper functions that are
   necessary for the module to work but are not part of the public API.
3. `<ModuleName>Messages.h` – Contains all messages related to information, 
warnings, errors, and fatal errors for that module.
4. `<ModuleName>TestHooks.h` – Exposes test-only hooks needed to validate
   internal behaviour without making those details part of the public API.

#### The `Log` module

- The `Log` module serves both as Smile's internal logger for info, warnings and
errors, and as a user-facing debugging log API. Since both use cases share the
same underlying implementation, `Log` is kept in a single `Public` directory.

#### The `Common` module

- Contains code and messages that can be shared across multiple modules.
- The directory is called `_Common` to easily sort it alphabetically.

### — docs

Contains documentation for both `Public` and `Internal` API, as well as 
CONTRIBUTING.md (this file) and [CONVENTIONS.md](CONVENTIONS.md). `_Internal` 
also stores GIFs, images, videos under `Assets` to be used in README files.

As opposed to `Internal` APIs, documentation for `Public` systems contains not
only the API documentation, but also a README for that module, which provides an
overview of the module and a workflow example.

