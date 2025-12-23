# Cpp-Project-Template
C++ General Project Template

## QML Terminal
The QML terminal example uses a cross-platform PTY backend:
- Windows uses ConPTY (`_WIN32` code path).
- Linux/macOS use `forkpty` (`!_WIN32` code path).

The backend is exposed to QML as `terminalBackend` and streams raw text; the UI renders it in `src/qml/Main.qml`.

## Layout
- `3rdparty`: Third-party and vendored dependencies that are consumed by the build.
- `include`: Public headers exposed by the repository.
- `src`: Application and library source files.
- `tests`: Automated tests covering the project.
- `scripts`: Automation, build, and deployment helpers.
- `cmake`: CMake scripts, modules, and helper files.
- `docs`: Design notes, user guides, and other documentation.
- `examples`: Sample apps or playgrounds for exercising components.
- `pocs`: Proofs of concept that explore ideas not yet integrated in the main sources.
