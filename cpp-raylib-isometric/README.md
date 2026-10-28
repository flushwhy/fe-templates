# {{.Name}}

A runnable C++17 + raylib isometric starter with a camera-following player, grid movement, tile collision, and a map authored in Tiled's JSON format.

## Build and run

Run these commands from the project root:

```sh
cmake -S . -B build
cmake --build build
./build/{{.Name}}
```

On Windows, run the executable from the project root so the relative map path resolves. Requirements: CMake 3.20+, a C++17 compiler, Git, and network access on first configure.

## Controls

- WASD or arrow keys: move one tile at a time.
- Escape: quit.

## Editing the map

Open `assets/world.tmj` in [Tiled](https://www.mapeditor.org/). The demo deliberately uses a tiny semantic palette so it runs without downloaded art assets:

- GID 1: grass (walkable)
- GID 2: path (walkable)
- GID 3: water (blocked)
- GID 4: wall (blocked)

The renderer draws placeholder diamonds by tile ID; this keeps the starter self-contained while you get movement, projection, camera, and collision working. Replace `World::DrawTile` with tileset rendering when you're ready to add art. The map loader reads the first tile layer from Tiled JSON (`.tmj`) and expects an isometric map with finite tile-layer data.

## Where to extend it

- `src/World.cpp`: isometric grid/world conversion, map loading, collision, and tile rendering.
- `src/Player.cpp`: movement and player drawing.
- `src/Game.cpp`: camera, game loop, and HUD.

This is a small foundation, not a full engine. It intentionally leaves pathfinding, animation, entities, inventory, and save data for the game to add as needed.
