# {{.Name}}

A small C++17/raylib starter with a real game loop, a base `Entity` class, and a playable `Player` implementation. The player moves with WASD or the arrow keys.

## Build and run

```sh
cmake -S . -B build
cmake --build build
./build/{{.Name}}
```

On Windows, run the generated executable from the build directory. CMake 3.20+, a C++17 compiler, Git, network access for the initial raylib fetch, and any platform graphics dependencies are required.

## Where to start

- `src/Game.cpp`: window, game loop, update/draw order, and entity ownership.
- `src/Entity.hpp`: abstract base class for world objects.
- `src/Player.cpp`: input, delta-time movement, screen bounds, and drawing.

Add a new class derived from `Entity`, then register it in `Game::Game()`. This is intentionally a small starting architecture, not a full engine.
