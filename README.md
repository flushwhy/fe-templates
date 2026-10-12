# fe-templates

Starter projects for [fe](https://github.com/flushwhy/fe). Template IDs are stable, lowercase, and hyphen-separated.

## Available templates

| Template ID | Description |
| --- | --- |
| `c-make` | C with a Makefile |
| `cpp-cmake` | C++ with CMake |
| `cpp-raylib-xmake` | C++ with raylib and Xmake |
| `go-basic` | Minimal Go application and module |
| `odin-basic` | Minimal Odin application with OLS configuration |
| `zig-build` | Minimal Zig application using the build system |

Create a project with:

```sh
fe template list
fe init --template cpp-raylib-xmake --name my-game
```

Language defaults are also available through `fe init c`, `fe init cpp`, `fe init go`, `fe init odin`, and `fe init zig`.

Templates are downloaded on first use and cached locally. Use `fe template update <id>` to refresh a cached copy.
