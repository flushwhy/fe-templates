# {{.Name}}

A minimal C++17 game window using raylib and CMake. CMake fetches raylib on the first configure.

## Build and run

```sh
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
./build/{{.Name}}
```

On Windows, run the generated executable from the build directory. CMake, a C++ compiler, Git, and network access for the initial dependency fetch are required.
