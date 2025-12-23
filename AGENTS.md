# Repository Guidelines

## Project Structure & Module Organization
- `src/` and `include/` hold application/library sources and public headers.
- `tests/` contains automated tests; `examples/` and `pocs/` are for samples and experiments.
- `cmake/` and `scripts/` provide build helpers; `3rdparty/` vendors external deps.
- `docs/` captures conventions and design notes.

## Build, Test, and Development Commands
- Configure a build directory (CMake project, C++17):
  `cmake -S . -B build`
- Build all targets:
  `cmake --build build`
- Run tests (CTest):
  `ctest --test-dir build`
These commands are the expected workflow for this repo; adjust the build directory name as needed.

## Coding Style & Naming Conventions
- Follow the Google C++-based conventions in `docs/convention/`.
- Indentation: 2 spaces, no tabs; keep lines under 100 chars.
- Naming: `UpperCamelCase` for types, `lowerCamelCase` for functions/vars, `kConstants` for constants, trailing `_` for private members; filenames are `lower_snake_case`.
- Include order: matching header first, then standard, third-party, and project headers.
- Use `#pragma once` or include guards; avoid `using namespace` in headers.
- Run static analysis before submitting: `clang-tidy` (and `cpplint` if used).

## Testing Guidelines
- Tests live under `tests/` and use GoogleTest-style macros: `TEST(ComponentTest, DescribesBehavior)` and `TEST_F` for fixtures.
- Keep Arrange/Act/Assert comments near each section and write a regression test for every bug fix.
- Run the suite via `ctest --test-dir build` after building.

## Commit & Pull Request Guidelines
- Commit messages follow: `Verb Scope: Short description` (e.g., `Fix Session: Handle reconnect loop`).
- Reference related issues where applicable.
- PRs should include a concise summary, tests run (or reason for omission), and screenshots for UI-facing changes.

## Documentation & Examples
- Update `docs/` when you add new patterns or workflows; keep examples aligned with current APIs.