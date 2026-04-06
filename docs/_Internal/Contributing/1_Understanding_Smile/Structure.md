# The Structure Of Smile 🏛️

## Table of Contents

- [Smile's Project Root Directory](#-smiles-project-root-directory)
- [Public and Internal](#-public-and-internal)
    - [external](#-external)
    - [include](#-include)
    - [src](#-src)
    - [docs](#-docs)
    - [tests](#-tests)
    - [tools](#-tools)
- [Up Next](#-up-next)

## 🌳 Smile's Project Root Directory

At the project root, you'll find a structure similar to this:

```zsh
Smile
├── CMakeLists.txt
├── .gitignore
├── LICENCE
├── README.md
├── build
├── docs
├── external
├── include
├── src
└── tests

```

Files and directories in the project root can be grouped into two categories:

- User-facing: These include files like `README.md` and `LICENCE`. They provide
  information to users, explaining what Smile is, how to use it, and what to
  expect from the project.
- Developer-facing: These include files such as `CMakeLists.txt` and
  `.gitignore`. They primarily support development and build processes, helping
  maintainers and contributors work efficiently.

The `build/` directory contains Smile's compiled files.

## 🔑 Public and Internal

Smile maintains a clear separation between user-facing and developer-facing
components, referred to as `Public` and `Internal`.

`Public` encompasses all resources intended for non-developer users, including
guides, API documentation, and header files.

This distinction is applied as a naming convention within module-oriented
directories such as `docs` and `src`. In those directories, `_Internal` holds
developer-focused documentation, internal functions, and support modules.

As you may have noticed, `_Internal` directories begin with an underscore (
`_`). This is to easily sort it when directories are organized alphabetically,
as directories of `Public` modules don't go under a specific folder.

Within directories that use this convention, anything outside `_Internal` is
considered `Public`.

Not every developer-facing directory uses the `_Internal` prefix. For example,
`tests/` is only intended for developers, but it is organized separately
because test code is optional and is not part of Smile's public API surface.

## 👶 Naming

`Public` files and directories, and directories under an `_Internal`
directory, are named after the module they represent. Message files follow the
pattern `<ModuleName>Messages.h`.

However, non-message files inside an `_Internal` directory are suffixed with
`Internal`, and message files are suffixed with `InternalMessages`.

Each module may also contain a `<ModuleName>Internal.h` file but this will be
elaborated upon when discussing the [src](#-src) directory.

✅ Example

```zsh
├── src
│ ├── Log  # Directory's name is the same as the module's name
│ │   │
│ │   ├── Log.c  # Source file has same name as the module
│ │   │
│ │   └── LogInternal.h  # Each module contains an internal header (see src under
│ │                    # the Directory Breakdown  section below).
│ ├── SceneManager 
│ │   ├── SceneManager.c
│ │   ├── SceneManagerInternal.h 
│ │   └── SceneManagerMessages.h  # Message file for a  Public module 
│ │
│ └── _Internal    # _Internal begins with an underscore for sorting purposes
│     │
│     ├── _Common  # _Common begins with an underscore for sorting purposes
│     │   │ 
│     │   └── CommonInternalMessages.h  # Message files inside an _Internal 
│     │                                 # directory are named:
│     │                                 # <ModuleName>InternalMessages.h
│     │
│     └── Test     # Even under _Internal, the directory's name is just the
│         │        # module's name
│         │           
│         ├── TestInternal.c  # Source and header files inside an _Internal 
│         └── TestInternal.h  # directory get the Internal suffix
```

## Directory Breakdown

### — external

While the `external/` directory contains third-party libraries, it is not meant
for users, as they're used internally by Smile to implement modules.

✅ Example

```zsh
├── external
│ ├── uthash.h  # Used for internal implementation only
```

---

### — include

Only contains header files for public modules.

✅ Example

```zsh
├── include
│ ├── Log.h
│ ├── ParticleSystem.h
│ ├── SaveLoad.h
│ ├── SceneManager.h
```

---

### — src

Aside from the module's `Public` header, new `Public` modules in Smile are expected to include four key files:

1. `<ModuleName>.c` – Implements the module's public and internal functions.
2. `<ModuleName>Internal.h` – Contains internal helper functions that are
   necessary for the module to work but are not part of the public API.
3. `<ModuleName>Messages.h` – Contains all messages related to warnings, errors,
   or other notifications for that module.
4. `<ModuleName>TestHooks.h` – Exposes test-only hooks needed to validate
   internal behaviour without making those details part of the public API.

✅ Example

```zsh
SceneManager
├── SceneManager.c          # Implements `smCreateScene`, `smSetScene`, etc.
│
├── SceneManagerInternal.h  # Functions like `smInternalGetScene` or
│                           # `smInternalGetEntry` which manipulate internal
│                           # structures (e.g., the scene map) not exposed to
│                           # users.
│
├── SceneManagerMessages.h  # Contains messages like `"Scene already exists"`
│                           # used in logging.
│
└── SceneManagerTestHooks.h # Exposes test-only hooks for API-level tests.
```

This separation ensures:

- Public headers define the stable API for users.
- Internal headers provide the tools developers need to implement the module.
- Messages are centralized and reusable.
- Test hooks support deterministic tests without expanding the public API.

#### EXCEPTION: The `Log` module!

- The `Log` module intentionally does not follow the standard layout of other modules.
  It serves both as Smile's internal logger for warnings and errors,
  and as a user-facing debugging log API.
- Since both use cases share the same underlying implementation, `Log` is kept
  in a single `Public` directory instead of being split into separate `Public`
  and `Internal` module layouts.

✅ Example

```zsh
├── src
│ ├── Log  # The Log directory can be found alongside other Public directories 
│ ├── SceneManager
│ └── _Internal
│     ├── _Common
│     └── Test
```

Under `src/_Internal` you'll also find the `_Common` module. This contains code
and messages that can be shared across multiple modules. For example, since
most modules have a `Start()` function, the message `"Module Started"` is
defined in `CommonInternalMessages` instead of being defined in every single
module with a `Start()` function.

---

### — docs

Contains documentation for both `Public` and `Internal` API, as well as
guidelines for contributing (under `Contributing`). Internal also stores
example GIFs, images, videos under `Assets` to be used in README files.

As opposed to `Internal` APIs, documentation for `Public` systems contains not
only the API documentation, but also a README for that module, which provides an
overview of the module and a workflow example.

See 3_Documentation_Guidelines (🚧 Under Development) for more details on
documenting `Public` and `Internal` modules.

✅ Example

```zsh
├── docs
│├── Log
││   ├── LogAPI.md          # Public API documentation
││   └── README.md          # Public module overview and workflow example
│├── SceneManager
││   ├── README.md
││   └── SceneManagerAPI.md
│└── _Internal
│    ├── CommonInternalAPI.md
│    ├── LogInternalAPI.md
│    ├── SceneManagerInternalAPI.md
│    ├── TestInternalAPI.md
│    ├── Contributing/
│    └── Assets
```

#### EXCEPTION: The `Log` module!

- The `Log` module contains both a `Public` and an `Internal` API.

✅ Example

```zsh
├── docs
│ ├── Log
│ │   ├── LogAPI.md         # Log's Public API
│ │   └── README.md
│ └── _Internal
│     └── LogInternalAPI.md # Log's Internal API
```

---

### — tests

Contains source files for testing a module's public API. Test files for APIs
end in `APITest`.

Tests are currently organized only for `Public` modules, with one directory per
module under `tests/`.

✅ Example

```zsh
├── tests
│ ├── Log
│ │   └── LogAPITest.c
│ └── SceneManager
│     └── SceneManagerAPITest.c
```

---

### — tools

`tools/` is planned, but is not currently part of the repository.

When added, it will contain the implementation of both `Public` and `Internal`
tools that can be called straight from the command line, reducing repetitive
tasks for both users and developers. For example, SceneManager could have a
tool to automate the creation of Scene files (.c and .h) by generating
boilerplate code for common functions.

The planned directory structure mirrors [src](#-src). Files and directories
will follow the `Public` and `Internal` naming conventions, except they include
the `Tool` suffix after `<ModuleName>`.

```zsh
├── tools
│├── SceneManager  # Public Tools
││├── SceneManagerTool.c
││├── SceneManagerToolInternal.h
││└── SceneManagerToolMessages.h
│└── _Internal
│    ├── Test  # Internal Tools
│    │   ├── TestInternalTool.c
│    │   └── TestInternalTool.h
```

## ➡️ Up Next

Learn the standards
for [Code Organization](../2_Coding_Guidelines/Code_Organization.md).
