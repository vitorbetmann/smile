# Repository Guidelines

## Project Structure & Source of Truth

Smile is a modular C23 static library built with CMake.

- `include/`: public API headers such as `Log.h` and `SceneManager.h`.
- `src/`: module implementations and internal support code under `Log/`, `SceneManager/`, and `_Internal/`.
- `tests/`: public API tests (`tests/SceneManagerAPITest.c`, `tests/LogAPITest.c`).
- `docs/`: user-facing module docs plus contributor guidance under `docs/_Internal/Contributing/`.
- `external/`: vendored single-header dependencies such as `uthash.h`.
- `tools/`: developer utilities and templates, including `GenScene.c` and `tools/SceneTemplates/`.
- `build*` and `cmake-build-*`: generated artifacts, not source of truth.

Read these before making structural or style changes:

- `docs/_Internal/Contributing/0_Start_Here/Developer_Setup.md`
- `docs/_Internal/Contributing/1_Understanding_Smile/Structure.md`
- `docs/_Internal/Contributing/2_Coding_Guidelines/Code_Organization.md`
- `docs/_Internal/Contributing/2_Coding_Guidelines/Code_Style.md`

When these docs conflict with ad hoc local habits, follow the docs and `.clang-format`.

## Build, Test, and Development Commands

- Default configure: `cmake -S . -B build`
- Developer configure: `cmake -S . -B build -G Ninja -DSMILE_DEV=ON`
- Build: `cmake --build build`
- Build with multi-config generator: `cmake --build build --config Debug`
- Run all tests: `ctest --test-dir build --output-on-failure`
- Run a single test binary: `./build/SceneManagerAPITest` or `./build/LogAPITest`

Useful configure flags:

- `-DSMILE_DEV=ON`: enables contributor-focused defaults
- `-DSMILE_TESTS=ON`: builds tests explicitly
- `-DSMILE_WARN=OFF`: disables runtime warning logs
- `-DSMILE_INFO=OFF`: disables runtime info logs

## Coding Rules

- Language standard is C23. Use C23 features already present in the codebase, such as `nullptr` and `bool`.
- Use `.clang-format` as the formatting source of truth. Keep line length at 120.
- Follow Smile’s file sectioning rules from `Code_Organization.md`, including named section headers and section order.
- Use `camelCase` for non-constant variables and functions.
- Use `SCREAMING_SNAKE_CASE` for named constants and macro-style compile-time values.
- Do not add externally linked global variables unless the change is explicitly test-only.
- Keep header dependencies explicit. Every file must include what it uses.
- Preserve the project’s Public vs Internal split:
  - public APIs live in `include/` and public module docs
  - internal helpers stay under `src/.../*Internal*` or `src/_Internal/`
  - message strings belong in `*Messages.h`

## Testing Expectations

- Add or update tests for public API behavior changes.
- Keep tests deterministic and fast.
- Prefer API-level coverage in `tests/` unless the change is strictly internal.
- If developer mode or tests are relevant to the change, verify with `ctest --test-dir build --output-on-failure`.

## Documentation Expectations

- Update `README.md` or module docs when public API or usage changes.
- Keep docs aligned with the module layout described in `Structure.md`.
- Prefer referencing existing contributor docs instead of duplicating guidance in new files.

## Change Discipline

- Keep edits focused. Avoid opportunistic refactors unless they are required for correctness.
- Do not treat generated build output as editable source.
- If the worktree already contains unrelated user changes, leave them untouched.
