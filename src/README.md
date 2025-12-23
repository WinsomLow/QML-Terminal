# src
Implementation files and private helpers.

## TODO: Terminal Rendering (VT100/ANSI)
- Integrate `libvterm` under `3rdparty/` and add CMake targets for Windows + Linux.
- Replace the current QML log with a terminal screen buffer model (rows/cols, cursor).
- Parse PTY output with `libvterm` and update the screen buffer on each write.
- Render the buffer in QML using a monospaced grid (Text or Repeater).
- Add input handling for arrows, backspace, paste, and basic selection/copy.
- Handle terminal resize (window size -> backend `resize()` -> PTY size).
- Add minimal ANSI support verification (clear screen, cursor move, prompt redraw).
