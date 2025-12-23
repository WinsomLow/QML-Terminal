# Google C++ Style: Naming & Formatting

This document distills the relevant sections of the Google C++ Style Guide so you can consistently apply the same naming and layout decisions throughout the repo.

## Naming Rules

- **Types and enums** use `UpperCamelCase`. Example: `class EventDispatcher {};`.
- **Functions, methods, and variables** use `lowerCamelCase`. Example: `void EmitSignal(const std::string& message);` and `bool hasPendingWork = false;`.
- **Constants** (including `constexpr`) have a `k` prefix and use `UpperCamelCase`: `constexpr int kMaxRetries = 5;`.
- **Private data members** have a trailing underscore: `std::string cacheKey_;` and `bool isConnected_ = false;`.
- **Boolean predicates** begin with `Is`, `Has`, `Can`, or `Should`: `bool IsInitialized() const;`, `bool CanRetry() const;`.
- **Namespaces** are `lower_snake_case`, typically matching the directory structure: `namespace example { ... }`.
- Avoid abbreviations unless they are universally understood (e.g., `url`, `sms`).

## File Naming

- Use `lower_snake_case` for filenames (e.g., `event_dispatcher.cc`, `session_manager.h`), matching the directory path.
- Keep header/source pairs synchronized (`foo.h` paired with `foo.cc` or `foo.cpp`) and prefer descriptive names that reflect the primary concept.
- Organize platform-specific or generated files in clearly marked directories (e.g., `docs/`, `tests/`) so their purpose is obvious.

## Formatting Guidelines

- Indent with **two spaces**; never use tabs. Example:
  ```cpp
  if (ShouldRetry()) {
    AttemptRetry();
  }
  ```
- Keep line length under 100 characters. When breaking long expressions, align the continuation with the open parenthesis:
  ```cpp
  auto response = client->Send(
      requestHeaders,
      payload,
      requestOptions);
  ```
- Place opening braces on the same line for functions and control blocks:
  ```cpp
  void Initialize() {
    ConfigureDefault();
  }
  ```
- Surround binary operators with spaces and follow with a space after commas:
  ```cpp
  x = y + z;
  ```
- Write `nullptr` instead of `NULL` or `0` for pointers; use `enum class` for scoped enums.

## File Layout

- Each header defines a single concept. Place the primary declarations near the top, followed by inline helpers if necessary.
- Source files (`.cc`, `.cpp`) include their matching header first, then grouped headers (standard, third-party, project), all sorted alphabetically inside each group.
  ```cpp
  #include "event_dispatcher.h"
  #include <algorithm>
  #include <memory>
  #include "thirdparty/http_client.h"
  #include "project/logging.h"
  ```
- Guard headers with include guards or `#pragma once`:
  ```cpp
  #ifndef EVENT_DISPATCHER_H_
  #define EVENT_DISPATCHER_H_
  ...
  #endif  // EVENT_DISPATCHER_H_
  ```
- Avoid `using namespace` directives in headers; prefer fully qualified names or `using` declarations inside source files.

## Comments & Documentation

- Prefer `//` for short explanations; reserve `/* ... */` for multi-line blocks that cannot be expressed concisely.
- Comments explain *why* something exists, not *what* it does.
  ```cpp
  // Acquire the mutex because callbacks run on worker threads.
  std::lock_guard<std::mutex> guard(state_mutex_);
  ```
- Use `[TODO]`/`[FIXME]` tags consistently and include a short rationale.
- Document public APIs with `///` if the project generates docs via Doxygen or similar tooling.
