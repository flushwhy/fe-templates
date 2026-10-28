# {{.Name}}

A C++17 + raylib isometric RPG starter with Tiled map loading, image tilesets, smooth grid movement, tile collision, A* pathfinding, a roaming NPC, object-layer spawn points and interactions, camera follow/zoom, and depth ordering.

## Build and run

Run from the project root:

```sh
cmake -S . -B build
cmake --build build
./build/{{.Name}}
```

Requirements: CMake 3.20+, a C++17 compiler, Git, and network access on first configure. CMake fetches raylib 5.5 and nlohmann/json.

## Controls

- WASD or arrow keys: move one tile at a time, with smooth movement between tiles.
- E: talk to Mira or inspect the sign when nearby.
- Mouse wheel: zoom camera.
- Escape: quit.

## Editing the map in Tiled

Open `assets/world.tmj` in [Tiled](https://www.mapeditor.org/). The sample uses a four-tile SVG placeholder tileset, but the loader also reads regular image tilesets (such as PNG) and external JSON tileset files. Keep the map orientation isometric and use finite tile layers.

- Add tile layers to draw more terrain layers; layer visibility and opacity are respected.
- Add an object layer named however you like. Point objects named `PlayerSpawn`, `Mira`, or `MiraGoal` configure the sample player and NPC positions.
- Objects with type/class `interactable` show a marker and display their name when the player presses E nearby.
- In a tileset's tile properties, set `walkable` to `true` or `false` to control movement. Tiles without this property use the starter's default: local tile IDs 2 and 3 are blocked.

The renderer supports regular grid image tilesets with columns, tile dimensions, and GID ranges. Tiled flip flags are safely masked but not visually applied yet; infinite/chunked maps and animated tiles are not supported yet.

## Included systems

- `World`: Tiled JSON parsing, image tilesets, multiple tile layers, isometric transforms, tile-property collision, and A* pathfinding.
- `Player`: smooth tile-to-tile movement and collision.
- `Npc`: autonomous path following and dialogue data.
- `Game`: camera follow/zoom, object-layer interactions, and character depth ordering.

This is a playable foundation rather than a full engine. Sprite-sheet animation, animated Tiled tiles, and a reusable entity lifecycle are good next additions.
