# {{.Name}}
A C++17 + raylib isometric RPG foundation: Tiled JSON map loading, SVG tileset rendering, isometric coordinates, smooth grid movement, tile collision, camera follow/zoom, A* pathfinding, a roaming NPC, dialogue interaction, and character depth ordering.

## Build and run
Run from the project root:
```sh
cmake -S . -B build
cmake --build build
./build/{{.Name}}
```
Requirements: CMake 3.20+, C++17 compiler, Git, and network access on first configure. CMake fetches raylib 5.5 and nlohmann/json.

## Controls
- WASD or arrow keys: move one tile at a time, with smooth movement between tiles.
- E: talk to Mira when nearby.
- Mouse wheel: zoom camera.
- Escape: quit.

## Map and tiles
Open `assets/world.tmj` in [Tiled](https://www.mapeditor.org/). The sample map uses a four-tile placeholder SVG tileset and does not need external art. The tile GID mapping in `World::Load` is grass (walkable), path (walkable), water (blocked), and wall (blocked). Keep GIDs aligned when editing the map.

## Included systems
- `World`: Tiled JSON parsing, isometric transforms, tile drawing, walkability, and A* pathfinding.
- `Player`: smooth tile movement and collision.
- `Npc`: autonomous path following and dialogue data.
- `Game`: camera follow/zoom, interaction, and depth ordering.

This is a playable foundation, not a full engine. Next extensions can add Tiled object-layer spawn points, sprite animation, inventory, combat, and save data.
