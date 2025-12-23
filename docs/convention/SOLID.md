# SOLID + Google Style

The SOLID principles complement the Google C++ Style Guide by keeping classes focused, easy to test, and safe to extend.

- **Single Responsibility**: Keep each class responsible for one part of the system and limit its public surface. Google-style conventions (such as descriptive names and trailing-underscore members) make small classes easier to read.
- **Open/Closed**: Favor interfaces (often abstract base classes ending with `Interface` or `Base`) and let implementations extend them without modifying existing code. Use `enum class` for open-ended options to avoid leaking details.
- **Liskov Substitution**: Write contracts (pre-/post-conditions) in comments or unit tests when overriding behavior. Prefer `override` keyword and avoid slicing.
- **Interface Segregation**: Break large interfaces into smaller ones, and name them clearly (`Renderable`, `Serializable`). Google-style naming helps keep these names expressive.
- **Dependency Inversion**: Depend on abstractions (`std::function`, interfaces) in constructors and use `std::unique_ptr`/`std::shared_ptr` for ownership. Document dependencies in constructors and tests.

By combining SOLID thinking with the Google conventions in this folder, your code will stay modular, readable, and maintainable.
