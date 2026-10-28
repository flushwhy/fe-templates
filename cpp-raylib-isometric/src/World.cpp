#include "World.hpp"

#include "raymath.h"
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>

using nlohmann::json;

bool World::Load(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open map: " << path << '\n';
        return false;
    }

    try {
        const json map = json::parse(file);
        width_ = map.at("width").get<int>();
        height_ = map.at("height").get<int>();
        tileWidth_ = map.value("tilewidth", 64);
        tileHeight_ = map.value("tileheight", 32);

        if (map.value("orientation", std::string("isometric")) != "isometric") {
            std::cerr << "Map must use Tiled's isometric orientation.\n";
            return false;
        }

        const auto& layers = map.at("layers");
        const auto layer = std::find_if(layers.begin(), layers.end(), [](const json& item) {
            return item.value("type", std::string()) == "tilelayer";
        });
        if (layer == layers.end()) {
            std::cerr << "Map has no tile layer.\n";
            return false;
        }

        const auto data = layer->at("data").get<std::vector<int>>();
        if (width_ <= 0 || height_ <= 0 ||
            data.size() != static_cast<std::size_t>(width_ * height_)) {
            std::cerr << "Map dimensions don't match tile layer data.\n";
            return false;
        }

        tiles_.clear();
        tiles_.reserve(data.size());
        for (int gid : data) {
            // The demo uses a tiny semantic palette: 0=empty, 1=grass,
            // 2=path, 3=water, 4=wall. Tiled's firstgid defaults to 1.
            switch (gid) {
                case 0: tiles_.push_back(TileType::Grass); break;
                case 2: tiles_.push_back(TileType::Path); break;
                case 3: tiles_.push_back(TileType::Water); break;
                case 4: tiles_.push_back(TileType::Wall); break;
                default: tiles_.push_back(TileType::Grass); break;
            }
        }
        return true;
    } catch (const std::exception& error) {
        std::cerr << "Could not read map " << path << ": " << error.what() << '\n';
        return false;
    }
}

TileType World::TileAt(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) return TileType::Wall;
    return tiles_[static_cast<std::size_t>(y * width_ + x)];
}

bool World::IsWalkable(int x, int y) const {
    const auto tile = TileAt(x, y);
    return tile == TileType::Grass || tile == TileType::Path;
}

Vector2 World::GridToWorld(Vector2 grid) const {
    return Vector2{
        (grid.x - grid.y) * tileWidth_ * 0.5f,
        (grid.x + grid.y) * tileHeight_ * 0.5f
    };
}

Vector2 World::WorldToGrid(Vector2 world) const {
    const float halfW = tileWidth_ * 0.5f;
    const float halfH = tileHeight_ * 0.5f;
    return Vector2{
        world.x / (2.0f * halfW) + world.y / (2.0f * halfH),
        world.y / (2.0f * halfH) - world.x / (2.0f * halfW)
    };
}

void World::DrawTile(int x, int y, TileType type) const {
    const Vector2 center = GridToWorld(Vector2{static_cast<float>(x), static_cast<float>(y)});
    Color color;
    switch (type) {
        case TileType::Grass: color = Color{82, 142, 91, 255}; break;
        case TileType::Path: color = Color{190, 157, 105, 255}; break;
        case TileType::Water: color = Color{65, 133, 178, 255}; break;
        case TileType::Wall: color = Color{104, 108, 112, 255}; break;
    }

    const Vector2 top{center.x, center.y - tileHeight_ * 0.5f};
    const Vector2 right{center.x + tileWidth_ * 0.5f, center.y};
    const Vector2 bottom{center.x, center.y + tileHeight_ * 0.5f};
    const Vector2 left{center.x - tileWidth_ * 0.5f, center.y};

    DrawTriangle(top, left, bottom, color);
    DrawTriangle(top, bottom, right, color);
    DrawLineV(top, right, Color{30, 48, 39, 90});
    DrawLineV(right, bottom, Color{30, 48, 39, 90});
    DrawLineV(bottom, left, Color{30, 48, 39, 90});
    DrawLineV(left, top, Color{30, 48, 39, 90});
}

void World::Draw() const {
    for (int sum = 0; sum < width_ + height_ - 1; ++sum) {
        for (int y = 0; y < height_; ++y) {
            const int x = sum - y;
            if (x >= 0 && x < width_) DrawTile(x, y, TileAt(x, y));
        }
    }
}
