# Contributing to SMILE Engine 😊

Thank you for your interest in contributing to SMILE!  
This project is designed to be beginner-friendly and modular—feel free to explore, fix bugs, add features, or simply learn.

---

## 📦 About the Project

SMILE stands for **State Machine Is Lowkey Elegant**.

It’s a modular collection of tools designed to simplify 2D game development in C, aimed at providing a lightweight and extensible foundation for developers at all skill levels.

To learn more about it, please see this [README](../README.md).

---

## 🚀 How to Get Started

### 1. Clone the repo

```sh
git clone https://github.com/vitorbetmann/smile_engine.git
cd smile_engine
```

### 2. Build with CMake (Recommended)

SMILE uses a simple, portable CMake build system.

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
```

> Requires CMake 3.25+ and a C compiler (Clang, GCC...).

---

## 🧪 Running Tests

When `BUILD_TESTS=ON`, SMILE compiles unit tests for individual modules.

After building, run the test executables manually, for example:

```sh
./build/TestStateMachine
```

Note: Test executable names may vary by module; check your build output for exact names.

Test sources live under `tests/`.

---

## 🗂 Project Structure

```
/smile_engine/
├── src/                # Engine modules, source files, internal headers
│   ├── StateMachine/
│   │   ├── StateMachine.c
│   │   └── StateMachineInternal.h
│   └── ParticleSystem/
├── include/            # Public headers
│   ├── StateMachine.h
│   ├── ParticleSystem.h
├── tests/              # Unit tests with test sources and headers
│   ├── StateMachine/
│   │   ├── StateMachineTestAccess.h
│   │   └── test_state_machine.c
├── docs/               # Documentation files
│   ├── StateMachine/
│   │   ├── SM_API.md
│   │   └── SM_GettingStarted.md
```

---

## 💡 Ways to Contribute

- Fix a bug (see [issues](https://github.com/vitorbetmann/smile_engine/issues))
- Add edge-case tests
- Improve documentation
- Propose or prototype a new module (e.g., SaveLoadSystem, RigidBodyPhysics...)
- Create bindings for other languages (Python, Lua, C++...)
- Reduce dependencies (e.g. abstract raylib for portability)
- Add or help maintain a Code of Conduct or Style Guide to foster a welcoming community.

### 🤖 Using AI Tools

- AI can be a helpful assistant—just remember to use it to support your learning, not replace it.
- Whether you're learning to code, write documentation, plan architecture, or design tests, try doing it yourself first.
- Use AI to review, suggest, or guide you, especially when you're stuck or want a second opinion.
- This isn't mandatory – but if you're here to grow, you'll only be cheating yourself.

---

## 🔖 Issue Tags to Look For

- `good first issue` → Beginner-friendly
- `type: bug` → Reproducible issues
- `type: proposal` → New features or design ideas
- `module: State Machine`, `module: Particle System`, etc.

---

## 🧷 Coding Style Guide

The following conventions apply to all engine modules, tests, and documentation. Stick to them when writing or editing code to keep things clean and consistent.

### Language and Syntax

- C23 standard only.
- Use `void` for functions that take no arguments (but omit `void` in function calls).

```c
// ✅ OK
void init_game(void);

// ❌ Don't do this
void init_game();
```

- Always use braces `{}` even for single-line control statements.

```c
// ✅ OK
if (is_running) {
    update();
}

// ❌ Don't do this
if (is_running)
    update();
```

### Naming

- Prefix all function names with their corresponding module acronym to make their origin clear and avoid name collisions:

  - For example, functions from the State Machine module start with `SM_`, like `SM_Init()` or `SM_ChangeState()`.
  - Functions from the Particle System module use `PS_`, like `PS_Emit()` or `PS_ResetParticles()`.

- Constants use PascalCase as well; normal variables use camelCase.
- Prefix functions with the module acronym (e.g., `SM_` for State Machine, `PS_` for Particle System).
- Non-public functions (e.g., internal helpers or test-only utilities) follow the same PascalCase naming style, but are prefixed clearly to indicate their context:

  - Use `Module_Internal_` for private/internal-use functions within a module.  
    Example: `SM_Internal_GetCurrState()`
  - Use `Module_Test_` for functions exposed for unit testing purposes.  
    Example: `SM_Test_GetStateCount()`

### Formatting

- Indent with **2 spaces**.
- Use `Type *ptr` (space after type) for pointer declarations.
- Align comments and keep them short and clear.
- Organize files using clearly marked sections and follow this order:

  1. `#include <...>`
  2. `#define` macros
  3. Data types (e.g., `struct`, `enum`)
  4. Variables
  5. Function prototypes
  6. `main()` (if present)
  7. Function definitions

  > In test files, place function definitions before main().

  Use comment headers to clearly separate each section:

  ```c
  // --------------------------------------------------
  // Section Name (e.g., Includes, Prototypes)
  // --------------------------------------------------
  ```

For example

```c
// --------------------------------------------------
// Variables
// --------------------------------------------------
static float myFloat = 0.016f;

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

/**
 * @brief Does something.
 *
 * @return true if it did something, otherwise false.
 * @author Vitor Betmann
 */
bool ABC_DoSomething(void);
```

### Documentation

- Use Doxygen-style comments for all functions, including internal helpers and test utilities.
- Add your name as `@author` if you create or substantially modify a file.

See [StateMachineInternal.h](../src/StateMachine/StateMachineInternal.h) for a well-documented example.

### 🧪 Test Function Naming

Test function names start with `Test_`, followed by the exact function name including its module prefix, and then a short description of the expected behavior.

For example, since the function is named `SM_Init`, a test for it might be named:

```c
Test_SM_Init_ReturnsFalseIfCalledTwice()
```

This keeps names clear and informative.

---

## 📨 Submitting a Pull Request

### ✅ Before You Submit Checklist

- [ ] Compiled successfully
- [ ] Passed all tests
- [ ] Followed coding style
- [ ] Documented all functions — including public APIs, internal helpers, and test-only utilities like `SM_Test_GetTracker()` (Doxygen-style)
- Take some credit: Add your name as `@author` if you create or substantially modify a file. 😉

### ✈️ How to submit

1. Fork the repo
2. Create a new branch  
   `git checkout -b fix-state-crash`, for example
3. Keep branches focused and sync with the main branch before submitting a PR.
4. Commit your changes
5. Push to your fork
6. Open a PR with a clear summary

### ‼️ ATTENTION! Pull requests without tests or with failing tests will not be accepted.

---

## 🐛 Submitting GitHub Issues

Found a bug or have an idea? Let us know!

Before opening a new issue:

1. Search the [issues tab](https://github.com/vitorbetmann/smile_engine/issues) to avoid duplicates.
2. For bugs, include:
   - What you were doing
   - What went wrong
   - OS and compiler version
3. For features, use the label `type: proposal`

If you're unsure how to label your issue, just open it — we'll take care of it.

---

## 🙋 Need Help?

If you have questions or want to discuss anything related to SMILE, feel free to reach out directly to Betmann by opening an issue labeled `type: question` on GitHub.

---

Thanks again for helping build SMILE! 😊
