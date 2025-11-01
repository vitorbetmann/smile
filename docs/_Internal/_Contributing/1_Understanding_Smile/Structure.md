# The Structure Of Smile 🏛️

---

<br>

## Table of Contents

- [Smile's Project Root Directory](#-smiles-project-root-directory)
- [Public and Internal](#-public-and-internal)
    - [external](#-_external_)
    - [include](#-_include_)
    - [src](#-_src_)
    - [docs](#-_docs_)
    - [test](#-_test_)
- [Up Next](#up-next)

---

<br>

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
└── test
```

Files in the project root can be grouped into two categories:

- User-facing: These include files like `README.md` and `LICENSE`. They provide
  information to users, explaining what Smile is, how to use it, and what to
  expect from the project.
- Developer-facing: These include files such as `CMakeLists.txt` and
  `.gitignore`. They primarily support development and build processes, helping
  maintainers and contributors work efficiently.

The `build/` directory contains Smile's compiled files.

<br>

---

## 🔑 Public and Internal

Smile maintains a clear separation between user-facing and developer-facing
components, referred to as `Public` and `Internal`.

`Public` encompasses all resources intended for non-developer users, including
guides, API documentation, and header files.

Within directories such as `docs`, `src`, and `test`, `_Internal` folders hold
developer-focused documentation, internal functions, helpers, and tools.
Anything outside `_Internal` is considered `Public`.

Further explanation regarding Smile's `Public` and `Private` categorization are
explained bellow.

<br>

### — _external_

While the `external/` directory contains third-party libraries, it is not meant
for users, as they're used internally by Smile to implement modules.

✅ Example

```zsh
├── external
│ ├── raylib
│ ├── uthash.h
```

<br>

### — _include_

Only contains header files for public modules.

✅ Example

```zsh
├── include
│ ├── Log.h
│ ├── ParticleSystem.h
│ ├── SaveLoad.h
│ ├── StateMachine.h
```

<br>

### — _src_

Each module in Smile typically has three key files:

1. `ModuleName.c` – Implements the module’s public and internal functions.
2. `ModuleNameInternal.h` – Contains internal helper functions that are
   necessary for the module to work but are not part of the public API.
3. `ModuleNameMessages.h` – Contains all messages related to warnings, errors,
   or other notifications for that module.

**Example: StateMachine**

- `StateMachine.c` → Implements `create_state`, `set_state`, etc.
- `StateMachineInternal.h` → Functions like `get_state` or `find_state` which
  manipulate internal structures (e.g., the state table) not exposed to users.
- `StateMachineMessages.h` → Contains messages like `"State already exists"`
  used in logging.

This separation ensures:

- Public headers define the stable API for users.
- Internal headers provide the tools developers need to implement the module.
- Messages are centralized and reusable.
-

**Exception**
The `Log` module ...

<br>

### — _docs_

**Exception**
The `Log` module ...

<br>

### — _test_

<br>

---

<br>

## Naming Conventions

### — _Folders_

<br>

### — _Files_

<br>

---

## Up Next

[2.1_Coding_Guidelines - Code Organization](../2_Coding_Guidelines/Code_Organization.md)

---

<br>

| Last modified | Author        | Description       |
|---------------|---------------|-------------------|
| Oct 21, 2025  | Vitor Betmann | Created document. |