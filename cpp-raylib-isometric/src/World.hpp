#pragma once
#include "raylib.h"
#include <string>
#include <vector>
enum class TileType { Empty, Grass, Path, Water, Wall };
class World {
public:
 bool Load(const std::string& path); void Unload(); void Draw() const;
 bool IsWalkable(int x,int y) const; Vector2 GridToWorld(Vector2 grid) const;
 Vector2 WorldToGrid(Vector2 world) const; std::vector<Vector2> FindPath(Vector2 start,Vector2 goal) const;
 int Width() const{return width_;} int Height() const{return height_;}
private:
 int width_=0,height_=0,tileWidth_=64,tileHeight_=32; std::vector<TileType> tiles_;
 Texture2D tileset_{}; bool hasTileset_=false;
 TileType TileAt(int x,int y) const; void DrawTile(int x,int y,TileType type) const;
};