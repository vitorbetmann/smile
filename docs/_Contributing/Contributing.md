# Contributing to SMILE Engine 😊

Thank you for your interest in contributing to SMILE!  
This project is designed to be beginner-friendly and modular — feel free to explore, fix bugs, add features, or just learn.

---

## 📦 About the Project

SMILE is a lightweight C game engine built around clear, modular systems.  
Modules include a state machine and a particle system, with many more to come.

Each module is standalone and testable.

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
cmake -B build
cmake --build build
```

> Requires CMake 3.25+ and a working C compiler (Clang, GCC, or MSVC).

To enable test builds:

```sh
cmake -B build -DBUILD_TESTS=ON
```

---

## 🧪 Running Tests

When `BUILD_TESTS=ON`, SMILE compiles unit tests for individual modules.

After building, run the test executables manually:

```sh
./build/TestStateMachine
./build/TestParticleSystem
```

Test sources live under `tests/`, one per module.

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
│   ├── StateMachine/
│   │   └── StateMachine.h
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
- Propose or prototype a new module (e.g. SaveSystem, Logging)
- Reduce dependencies (e.g. abstract raylib for portability)

### 🤖 Using AI Tools

damn
AI can be a helpful assistant — just remember to use it to support your learning, not replace it.  
Whether you're learning to code, write documentation, plan architecture, or design tests, try doing it yourself first.  
Use AI to review, suggest, or guide you, especially when you're stuck or want a second opinion.  
This isn’t mandatory — but if you're here to grow, you'll only be cheating yourself.

---

## 🔖 Issue Tags to Look For

- `good first issue` → Beginner-friendly
- `type: bug` → Reproducible issues
- `type: proposal` → New features or design ideas
- `module: State Machine`, `module: Particle System`, etc.

---

## 🧷 Coding Style

The following conventions apply to both engine modules and tests.

- C23 only
- Function names use PascalCase: `SM_Init`, `SM_ChangeState`
- Function names prepend the module's acronym followed by an underscore.  
  For example: `SM_Init` (State Machine), `PS_Emit` (Particle System)
- Pointers use `Type *ptr`, not `Type* ptr`
- **Always use braces**, even for one-line control statements:

  ```c
  if (foo) {
      doSomething();
  }

  ```

- Comments should be concise and aligned
- Documentation uses **Doxygen-style comments** for consistency and to enable automated doc generation.
- See [StateMachine.h](../../include/StateMachine/StateMachine.h) for a good example of consistent documentation.

### 🧪 Test Naming Convention

Test function names start with `Test_`, followed by the exact function name including its module prefix, and then a short description of the expected behavior.

For example, since the function is named `SM_Init`, a test for it might be named:

```c
Test_SM_Init_ReturnsFalseIfCalledTwice
```

This keeps names clear and avoids redundant module prefixes.

---

## 📨 Submitting a Pull Request

### ✅ Before You Submit

- [ ] Compiled successfully
- [ ] Passed all tests
- [ ] Followed coding style
- [ ] Documented all functions — including public APIs, internal helpers, and test-only utilities like `SM_Test_GetTracker()` (Doxygen-style)

1. Fork the repo
2. Create a new branch
   `git checkout -b fix-state-crash`
3. Commit your changes
4. Push to your fork
5. Open a PR with a clear summary

---

## 🐛 Submitting Issues

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

If you have questions or want to discuss anything related to SMILE, feel free to reach out directly to Vitor Betmann by opening an issue labeled `type: question` on GitHub.

---

Thanks again for helping build SMILE! 😊
