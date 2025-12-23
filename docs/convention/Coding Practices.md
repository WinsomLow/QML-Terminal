# Coding Practices

This write-up captures the practical habits that keep our code safe, readable, and collaboration-ready while staying aligned with the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## Safety & Ownership

- Prefer RAII and smart pointers instead of manual `new`/`delete` to keep resource lifetime explicit.
- Encapsulate shared resources in dedicated classes and pass them by reference or smart pointer; avoid hidden global state.
- Always check return values, status objects, or `absl::StatusOr<T>` immediately; do not ignore errors silently.
- Avoid macros except for include guards, compile-time switches, or conditional logging; prefer typed constants or inline functions.
- Wrap local declarations in a `switch` `case` block with braces (`{}`) to prevent name collisions or leaks.

## Design & Maintainability

- Keep each function focused and short (preferably under 100 lines) and extract descriptively named helpers when logic grows.
- Avoid nested loops unless absolutely necessary; refactor with helper functions, standard algorithms, or early exits to manage complexity.
- Favor expressive names, single responsibilities, and early returns so readers can follow control flow without tracking state flags.
- Pass large objects by `const&` and prefer spans (`absl::Span`, `std::span`) for ranges to signal intent and avoid copies.
- Prefer `constexpr` for compile-time constants and `const` for values that should never mutate.

## Resource Management & Performance

- Reserve container capacity before bulk inserts to minimize reallocations.
- Use `std::move` when transferring ownership and reset moved-from values if you plan to reuse them.
- Favor algorithms (`std::sort`, `std::transform`, etc.) over hand-rolled loops when they improve clarity or correctness.
- Keep performance-critical loops simple; split work into helpers if a loop must nest to avoid exponential cost.

## Error Handling

- Use exceptions for error handling instead of ad-hoc error codes, and document the exception guarantees for public APIs.
- Catch concrete exception types at narrow scopes so you do not inadvertently swallow unexpected failures.
- Prefer `absl::Status` or `StatusOr` when exceptions are inappropriate (e.g., in signal handlers) and propagate errors explicitly.

## Header Hygiene

- Forward declare classes when the full definition is not required to reduce dependencies between headers.
- Keep include order consistent: related project headers first, then third-party headers, then standard headers.
- Prefer `#pragma once` (or include guards) and avoid leaking implementation details from headers into translation units.

## Templates & Generics

- Use `CamelCase` for template parameter names (e.g., `template <typename Callback>`).
- Document and limit template specializations to cases the general template cannot handle, keeping the specialization's intent clear.
- Avoid overly complex template metaprogramming; prefer readable, well-documented abstractions whenever possible.

## Testing & Quality

- Name tests with the `TEST(ComponentTest, DescribesBehavior)` pattern and keep Arrange/Act/Assert comments near each section.
- Reuse fixtures (`TEST_F`) for shared setup and keep each test focused on a single behavior.
- Write regression tests for every bug fix so the failure it guards against stays reproducible.
- Run static analysis (`clang-tidy`, `cpplint`, etc.) before submitting code and document any remaining warnings in your review comments.

## Collaboration & Process

- Document build steps, dependencies, and lint commands in `README.md`, `BUILD.md`, or `docs/BUILD.md` so reviewers can reproduce the environment.
- Follow the commit-message format `Verb Scope: Short description` (e.g., `Fix Session: Handle reconnect loop`) and reference related issues when relevant.
- Raise reviewers' blockers early, noting whether they are hard stops or can wait.
- Link to the Google C++ Style Guide (`https://google.github.io/styleguide/cppguide.html`) when teams need the canonical reference.

## Miscellaneous

- Use `auto` when the type is obvious or when iterators/complex types would otherwise clutter the code.
- Encapsulate code in namespaces to avoid collisions, especially across large projects or when integrating third-party libraries.
- Keep coding practice documents updated; include new patterns or learnings under the most appropriate section above.

Refer to the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) for the full, authoritative rules.
