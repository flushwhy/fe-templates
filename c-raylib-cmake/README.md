# C + raylib + CMake

A small raylib window using CMake and C11. CMake downloads raylib 5.5 during configuration, so the first configure requires network access.

```sh
cmake -S . -B build
cmake --build build --config Release
```

Run the generated executable from the build directory. Requires CMake 3.20+, Git, a C compiler, and the platform's raylib build dependencies.
