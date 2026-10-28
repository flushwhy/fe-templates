#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

struct MapObject {
    std::string name;
    std::string type;
    Vector2 gridPosition{};
};

class World {
public:
    bool Load(const std::string& path);
    void Unload();
    void Draw() const;
    bool IsWalkable(int x, int y) const;
    Vector2 GridToWorld(Vector2 grid) const;
    Vector2 WorldToGrid(Vector2 world) const;
    std::vector<Vector2> FindPath(Vector2 start, Vector2 goal) const;
    Vector2 ObjectPosition(const std::string& name, Vector2 fallback) const;
    const std::vector<MapObject>& Objects() const { return objects_; }
    int Width() const { return width_; }
    int Height() const { return height_; }

private:
    struct TileSet {
        int firstGid = 1;
        int tileWidth = 0;
        int tileHeight = 0;
        int columns = 0;
        int tileCount = 0;
        Texture2D texture{};
        bool loaded = false;
        std::unordered_map<int, bool> walkable;
    };
    struct Layer {
        std::string name;
        std::vector<unsigned int> gids;
        bool visible = true;
        float opacity = 1.0f;
    };

    int width_ = 0, height_ = 0, tileWidth_ = 64, tileHeight_ = 32;
    std::vector<TileSet> tilesets_;
    std::vector<Layer> layers_;
    std::vector<MapObject> objects_;

    const TileSet* TilesetFor(unsigned int gid) const;
    bool TileWalkable(unsigned int gid) const;
    void DrawGid(int x, int y, unsigned int gid, float opacity) const;
};