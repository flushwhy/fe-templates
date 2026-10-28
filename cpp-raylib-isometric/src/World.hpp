#pragma once

#include "raylib.h"

#include <string>
#include <vector>

enum class TileType {
    Grass,
    Path,
    Water,
    Wall
};

class World {
public:
    bool Load(const std::string& path);
    void Draw() const;
    bool IsWalkable(int x, int y) const;
    Vector2 GridToWorld(Vector2 grid) const;
    Vector2 WorldToGrid(Vector2 world) const;

    int Width() const { return width_; }
    int Height() const { return height_; }

private:
    int width_ = 0;
    int height_ = 0;
    int tileWidth_ = 64;
    int tileHeight_ = 32;
    std::vector<TileType> tiles_;

    TileType TileAt(int x, int y) const;
    void DrawTile(int x, int y, TileType type) const;
};
