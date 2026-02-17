# Repository Guidelines

## Project Structure & Module Organization

Smile is a modular C23 static library organized by feature.

- `include/`: public API headers (`Log.h`, `SceneMachine.h`, `SaveLoad.h`).
- `src/`: module implementations (`src/Log/`, `src/SceneMachine/`,
  `src/SaveLoad/`) plus internal utilities in `src/_Internal/`.
- `tests/`: API test programs by module (for example
  `tests/SceneMachine/SceneMachineAPITest.c`).
- `docs/`: public docs and internal contributor material, especially coding
  guides in `docs/_Internal/__Contributing/2_Coding_Guidelines/`.
- `external/`: third-party headers (currently `uthash.h`).

Keep changes focused and avoid moving public headers unless the task explicitly
requires it.

## Build, Test, and Development Commands

Run from repository root.

```bash
cmake -S . -B build
cmake --build build
```

Configures and builds the `smile` static library with default settings.

```bash
cmake -S . -B build -DSMILE_DEVELOPER=ON
cmake --build build
```

Enables developer mode (Debug defaults and module test targets).

```bash
cmake -S . -B build -DSMILE_LOG_WARNING=OFF -DSMILE_LOG_INFO=OFF
```

Builds with warning/info logs disabled at compile time.

Run test executables directly from `build/` (example:
`./build/SceneMachineAPITest`).

## Coding Style & Naming Conventions

- Standard: `C23`; compiler extensions are disabled (`CMAKE_C_EXTENSIONS OFF`).
- Naming: `camelCase` for non-constant variables/functions,
  `SCREAMING_SNAKE_CASE` for constants/macros.
- Scope: avoid global state in production code; prefer `static` for file-local
  symbols.
- Style source of truth:
  `docs/_Internal/__Contributing/2_Coding_Guidelines/Code_Style.md` and
  `Code_Organization.md`.

## Testing Guidelines

- Tests are plain C executables linked against `smile` (target pattern:
  `<Module>APITest`).
- Add tests under `tests/<Module>/` for any changed behavior.
- Follow existing naming like `Test_smStart_IsIdempotentPostStart`.
- Before submitting, rebuild and run impacted test binaries; report exactly
  which tests ran.

## Commit & Pull Request Guidelines

- Prefer short, imperative commit messages (examples in history:
  `Update README.md`, `Restore SaveLoad files on implement-save-load`).
- Keep each commit single-purpose and reviewable.
- PRs should include: change summary, affected modules/files, validation
  commands + results, and linked issue/task when applicable.
