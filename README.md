# fe-templates

Starter projects for [fe](https://github.com/flushwhy/fe). Template IDs are stable, lowercase, and hyphen-separated.

## Available templates

| Template ID | Description |
| --- | --- |
| `c-make` | C with a Makefile |
| `c-raylib-cmake` | C game window using raylib and CMake |
| `cpp-raylib-cmake` | C++17 game window using raylib and CMake |
| `cpp-raylib-game` | C++17 raylib game loop with an Entity base class and playable player |
| `cpp-raylib-isometric` | C++17/raylib isometric map with Tiled JSON, camera follow, movement, and collision |
| `cpp-cmake` | C++ with CMake |
| `cpp-raylib-xmake` | C++ with raylib and Xmake |
| `cpp-sdl3-cmake` | C++ game window using SDL3 and CMake |
| `godot-2d` | Godot 4 2D starter with a runnable scene and keyboard movement |
| `go-basic` | Minimal Go application and module |
| `odin-basic` | Minimal Odin application with OLS configuration |
| `rust-basic` | Minimal Rust binary using Cargo |
| `zig-build` | Minimal Zig application using the build system |

Create a project with:

```sh
fe template list
fe init --template cpp-raylib-game --name my-game
```

Language defaults are also available through `fe init c`, `fe init cpp`, `fe init go`, `fe init odin`, and `fe init zig`.

Templates are downloaded on first use and cached locally. Use `fe template update <id>` to refresh a cached copy.

## Build requirements

- `rust-basic`: Rust toolchain with Cargo.
- `c-raylib-cmake`, `cpp-raylib-cmake`, `cpp-raylib-game`, and `cpp-sdl3-cmake`: CMake 3.20+, Git, the relevant compiler, network access for the first configure, and any platform dependencies required by raylib or SDL3.
- `cpp-raylib-isometric`: CMake 3.20+, a C++17 compiler, Git, and network access for the first configure (raylib and nlohmann/json are fetched by CMake).
- `godot-2d`: Godot 4; desktop exports additionally require matching export templates.
