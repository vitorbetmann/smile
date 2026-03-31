# The Structure Of Smile 🏛️

## Table of Contents

- [Smile's Project Root Directory](#-smiles-project-root-directory)
- [Public and Internal](#-public-and-internal)
    - [external](#-external)
    - [include](#-include)
    - [src](#-src)
    - [docs](#-docs)
    - [test](#-tests)
    - [tools](#-tools)
- [Up Next](#-up-next)

## 🌳 Smile's Project Root Directory

After compiling Smile, your directory should look like this:

```zsh
Smile
├── CMakeLists.txt
├── .gitignore
├── LICENSE
├── README.md
├── build
├── docs
├── external
├── include
├── src
├── tests
└── tools

```

Files and directories in the project root can be grouped into two categories:

- User-facing: These include files like `README.md` and `LICENSE`. They provide
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

Within directories such as `docs`, `src`, and `test`, `_Internal` hold
developer-focused documentation, internal functions, and tools.

As you may have noticed, `_Internal` directories begin with and underscore (
`_`). This is to easily sort it when directories are organized alphabetically,
as directories of `Public` modules don't go under a specific folder. This
pattern can also be found in `docs/_Internal` where `_Contributing` also starts
with an underscore (`_`) and `__Assets` begins with two.

Anything outside `_Internal` is considered `Public`.

## 👶 Naming

`Public` files, `Public` directories, and directories under an `_Internal`
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

Each module in Smile typically has three key files aside from its `Public`
header:

1. `<ModuleName>.c` – Implements the module’s public and internal functions.
2. `<ModuleName>Internal.h` – Contains internal helper functions that are
   necessary for the module to work but are not part of the public API.
3. `<ModuleName>Messages.h` – Contains all messages related to warnings, errors,
   or other notifications for that module.

✅ Example

```zsh
SceneManager
├── SceneManager.c          # Implements `create_state`, `set_state`, etc.
│
├── SceneManagerInternal.h  # Functions like `get_state` or `find_state` which
│                           # manipulate internal structures (e.g., the state
│                           # table) not exposed to users.
│
├── SceneManagerMessages.h  # Contains messages like `"Scene already exists"`
                            # used in logging.
```

This separation ensures:

- Public headers define the stable API for users.
- Internal headers provide the tools developers need to implement the module.
- Messages are centralized and reusable.

#### EXCEPTION: The `Log` module!

- The `Log` module contains both a `Public` and an `Internal` API, however,
  since both utilize the same base code, and to prevent the creation of multiple
  files and directories, its directory is stored as `Public`.

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
and messages that can be shared through multiple modules. For example, since
most modules have a `Start()` function, the message `"Module Started"` is
defined in `CommonInternalMessages` instead of being defined in every single
module with a `Start()` function.

---

### — docs

Contains documentation for both `Public` and `Internal` API, as well as
guidelines for contributing (under `_Contributing`). Internal also stores
example gifs, images, videos under `__Assets` to be used in README files.

As opposed to `Internal` APIs, documentation of `Public` systems contain not
only the API documentation, but also a README for that module, which provides an
overview of the module and a workflow example.

See [3_Documentation_Guidelines] (🚧 Under Development) for more details on
documenting `Public` and `Internal` modules.

✅ Example

```zsh
├── docs
│├── Log  # Documentation of Public modules
│├── SceneManager
│└── _Internal
│    ├── Log  # Documentation of Internal modules
│    ├── Test
│    ├── _Contributing
│    └── __Assets
```

#### EXCEPTION: The `Log` module!

- The `Log` module contains both a `Public` and an `Internal` API.

✅ Example

```zsh
├── docs
│ ├── Log  # Log's Public API
│ ├── SceneManager
│ └── _Internal
│     ├── Log  # Log's Internal API
```

---

### — tests

Contains source and header files for testing a module's API and tools. Test
files for APIs end in `APITest` and for tools end in `ToolTest`.

File and directory nomenclature follow standard `Public` and `Internal`
conventions.

✅ Example

```zsh
├── tests
│ ├── SceneManager  # API and tools tests for Public modules
│ │ ├── SceneManagerAPITest.c
│ │ ├── SceneManagerToolTest.c
│ │ └── SceneManagerToolTest.h
│ └── _Internal
│     ├── Test  # API and tools tests for Internal modules
│     │   ├── TestInternalAPITest.c
│     │   └── TestInternalAPITest.h
```

---

### — tools

Contains the implementation of both `Public` and `Internal` tools that can be
called straight from the command line, reducing repetitive tasks for both users
and developers. For example, SceneManager could have a tool to automate the
creation of State files (.c and .h) by generating boilerplate code for common
functions

The directory structure mirrors [src](#-src). Files and directories follow the
`Public` and `Internal` naming conventions, except they include the `Tool`
suffix after `<ModuleName>`.

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
for [Code Organization](/docs/_Internal/_Contributing/2_Coding_Guidelines/Code_Organization.md).
