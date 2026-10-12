# {{.Name}}

A starter project using C++, [raylib](https://www.raylib.com/), and [Xmake](https://xmake.io/).

## Build

Run these commands from the project directory:

```sh
xmake
xmake run
```

Xmake downloads the raylib dependency on the first build. Source files in `src/` are included automatically.

## Customize

- Change the window title and game loop in `src/main.cpp`.
- Add C++ source files under `src/` and headers under `include/`.
- Change the target name or build settings in `xmake.lua`.

Template ID: `cpp-raylib-xmake`.
