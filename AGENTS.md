# Repository Guidelines

## Project Structure & Module Organization
Smile is a modular C23 static library built with CMake.
- `include/`: public API headers (for example, `Log.h`, `SceneManager.h`).
- `src/`: module implementations and internal code (`Log/`, `SceneManager/`, `_Internal/`).
- `tests/`: API-level tests (currently `tests/SceneManager/SceneManagerAPITest.c`).
- `docs/`: module docs and contributor references under `docs/_Internal/__Contributing/`.
- `external/`: third-party single-header dependencies (for example, `uthash.h`).
- `build*` and `cmake-build-*`: local build artifacts; do not treat as source of truth.

## Build, Test, and Development Commands
- Configure default (Release, tests off): `cmake -S . -B build`
- Configure developer mode (Debug defaults, tests on): `cmake -S . -B build -DSMILE_DEV=ON`
- Build (single-config generators): `cmake --build build`
- Build (multi-config generators): `cmake --build build --config Debug`
- Run all tests (recommended): `ctest --test-dir build --output-on-failure`
- Run API test executable (example): `./build/SceneManagerAPITest`
- Toggle runtime logs/tests at configure time:
  - `-DSMILE_WARN=OFF`
  - `-DSMILE_INFO=OFF`
  - `-DSMILE_TESTS=ON`

## Coding Style & Naming Conventions
- Language standard: C23 (`CMAKE_C_STANDARD 23`).
- Follow project conventions in `docs/_Internal/__Contributing/2_Coding_Guidelines/` (prefer linking a single primary guideline file when adding new docs).
- Use `camelCase` for non-constant variables/functions, `SCREAMING_SNAKE_CASE` for constants/macros.
- Use `SMILE_...` include guards in headers.
- Keep internal linkage explicit with `static` for file-scoped symbols.
- Organize files with section headers and consistent include ordering (system, module, support).

## Testing Guidelines
- Add tests under `tests/<Module>/` and name executables clearly (pattern used: `<Module>APITest`).
- Enable tests with `SMILE_TESTS=ON` (or `SMILE_DEV=ON`) before building.
- Validate new public API behavior and edge cases; keep tests deterministic and fast.

## Commit & Pull Request Guidelines
- Prefer short, imperative commit subjects (`Update ...`, `Rename ...`, `Refactor ...`), optionally module-scoped.
- Keep commits focused (code + related docs/tests together).
- PRs should include:
  - concise summary of behavior changes,
  - linked issue/context,
  - test evidence (commands run and results),
  - doc updates when API or workflow changes.
