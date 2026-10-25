# C++ + SDL3 + CMake

A minimal SDL3 window application. CMake fetches SDL3 during configuration, so the first configure requires network access.

```sh
cmake -S . -B build
cmake --build build --config Release
```

Requires CMake 3.20+, Git, a C++17 compiler, and platform dependencies needed to build SDL3.
