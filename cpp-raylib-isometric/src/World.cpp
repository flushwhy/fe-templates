#include "World.hpp"
#include "raymath.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>

using nlohmann::json;

namespace {
constexpr unsigned int kGidMask = 0x1fffffffU;

Texture2D LoadTilesetTexture(const std::filesystem::path& path, int width, int height) {
    const std::string filename = path.string();
    Image image{};
    if (path.extension() == ".svg") image = LoadImageSvg(filename.c_str(), width, height);
    else image = LoadImage(filename.c_str());
    if (!image.data) return {};
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}
}

bool World::Load(const std::string& path) {
    Unload();
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open map: " << path << '\n';
        return false;
    }

    try {
        const json map = json::parse(file);
        if (map.value("orientation", std::string("isometric")) != "isometric") {
            std::cerr << "Map must use Tiled's isometric orientation.\n";
            return false;
        }
        width_ = map.at("width").get<int>();
        height_ = map.at("height").get<int>();
        tileWidth_ = map.value("tilewidth", 64);
        tileHeight_ = map.value("tileheight", 32);
        if (width_ <= 0 || height_ <= 0 || tileWidth_ <= 0 || tileHeight_ <= 0) {
            std::cerr << "Invalid map dimensions.\n";
            return false;
        }

        const auto mapDirectory = std::filesystem::path(path).parent_path();
        for (const auto& source : map.value("tilesets", json::array())) {
            json tileset = source;
            if (source.contains("source")) {
                std::ifstream external(mapDirectory / source.at("source").get<std::string>());
                if (!external) throw std::runtime_error("could not open external tileset: " + source.at("source").get<std::string>());
                external >> tileset;
            }
            TileSet set;
            set.firstGid = source.at("firstgid").get<int>();
            set.tileWidth = tileset.value("tilewidth", tileWidth_);
            set.tileHeight = tileset.value("tileheight", tileHeight_);
            set.columns = tileset.value("columns", 0);
            set.tileCount = tileset.value("tilecount", 0);
            if (tileset.contains("image")) {
                const auto imagePath = mapDirectory / tileset.at("image").get<std::string>();
                set.texture = LoadTilesetTexture(imagePath, tileset.value("imagewidth", 0), tileset.value("imageheight", 0));
                set.loaded = IsTextureValid(set.texture);
                if (set.columns <= 0 && set.loaded) set.columns = static_cast<int>(set.texture.width) / set.tileWidth;
                if (set.tileCount <= 0 && set.loaded && set.columns > 0)
                    set.tileCount = (static_cast<int>(set.texture.height) / set.tileHeight) * set.columns;
            }
            for (const auto& tile : tileset.value("tiles", json::array())) {
                bool walkable = true;
                for (const auto& property : tile.value("properties", json::array())) {
                    if (property.value("name", std::string()) == "walkable")
                        walkable = property.value("value", true);
                }
                set.walkable[tile.at("id").get<int>()] = walkable;
            }
            tilesets_.push_back(std::move(set));
        }
        std::sort(tilesets_.begin(), tilesets_.end(), [](const TileSet& a, const TileSet& b) {
            return a.firstGid < b.firstGid;
        });

        for (const auto& source : map.at("layers")) {
            const std::string type = source.value("type", std::string());
            if (type == "tilelayer") {
                Layer layer;
                layer.name = source.value("name", std::string());
                layer.visible = source.value("visible", true);
                layer.opacity = source.value("opacity", 1.0f);
                if (!source.contains("data")) {
                    std::cerr << "Only finite Tiled tile layers are supported (layer '" << layer.name << "').\n";
                    return false;
                }
                layer.gids = source.at("data").get<std::vector<unsigned int>>();
                if (layer.gids.size() != static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_)) {
                    std::cerr << "Invalid tile layer dimensions in '" << layer.name << "'.\n";
                    return false;
                }
                layers_.push_back(std::move(layer));
            } else if (type == "objectgroup") {
                for (const auto& object : source.value("objects", json::array())) {
                    MapObject item;
                    item.name = object.value("name", std::string());
                    item.type = object.value("type", object.value("class", std::string()));
                    const Vector2 pixel{object.value("x", 0.0f), object.value("y", 0.0f)};
                    item.gridPosition = WorldToGrid(pixel);
                    objects_.push_back(std::move(item));
                }
            }
        }
        if (layers_.empty()) {
            std::cerr << "Map has no finite tile layers.\n";
            return false;
        }
        return true;
    } catch (const std::exception& error) {
        std::cerr << "Could not read map " << path << ": " << error.what() << '\n';
        Unload();
        return false;
    }
}

void World::Unload() {
    for (auto& tileset : tilesets_) if (tileset.loaded) UnloadTexture(tileset.texture);
    tilesets_.clear();
    layers_.clear();
    objects_.clear();
}

const World::TileSet* World::TilesetFor(unsigned int gid) const {
    const TileSet* found = nullptr;
    for (const auto& set : tilesets_) {
        if (gid >= static_cast<unsigned int>(set.firstGid)) found = &set;
        else break;
    }
    return found;
}

bool World::TileWalkable(unsigned int rawGid) const {
    const unsigned int gid = rawGid & kGidMask;
    if (gid == 0) return true;
    const TileSet* set = TilesetFor(gid);
    if (!set) return true;
    const auto custom = set->walkable.find(static_cast<int>(gid) - set->firstGid);
    if (custom != set->walkable.end()) return custom->second;
    // Preserve the starter map's convention: third and fourth tiles block movement.
    const int localId = static_cast<int>(gid) - set->firstGid;
    return localId != 2 && localId != 3;
}

bool World::IsWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) return false;
    const auto index = static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) + static_cast<std::size_t>(x);
    for (const auto& layer : layers_) {
        if (!layer.visible) continue;
        if (!TileWalkable(layer.gids[index])) return false;
    }
    return true;
}

Vector2 World::GridToWorld(Vector2 grid) const {
    return {(grid.x - grid.y) * tileWidth_ * 0.5f, (grid.x + grid.y) * tileHeight_ * 0.5f};
}

Vector2 World::WorldToGrid(Vector2 world) const {
    const float halfW = tileWidth_ * 0.5f, halfH = tileHeight_ * 0.5f;
    return {world.x / (2.0f * halfW) + world.y / (2.0f * halfH),
            world.y / (2.0f * halfH) - world.x / (2.0f * halfW)};
}

Vector2 World::ObjectPosition(const std::string& name, Vector2 fallback) const {
    for (const auto& object : objects_) if (object.name == name) return object.gridPosition;
    return fallback;
}

std::vector<Vector2> World::FindPath(Vector2 start, Vector2 goal) const {
    const int sx = static_cast<int>(std::round(start.x)), sy = static_cast<int>(std::round(start.y));
    const int gx = static_cast<int>(std::round(goal.x)), gy = static_cast<int>(std::round(goal.y));
    if (!IsWalkable(sx, sy) || !IsWalkable(gx, gy)) return {};
    const auto key = [this](int x, int y) { return y * width_ + x; };
    struct Node { int x, y, cost, score; };
    struct Later { bool operator()(const Node& a, const Node& b) const { return a.score > b.score; } };
    std::priority_queue<Node, std::vector<Node>, Later> open;
    const std::size_t size = static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_);
    std::vector<int> previous(size, -1), cost(size, 1000000);
    const auto heuristic = [gx, gy](int x, int y) { return std::abs(gx - x) + std::abs(gy - y); };
    cost[static_cast<std::size_t>(key(sx, sy))] = 0;
    open.push({sx, sy, 0, heuristic(sx, sy)});
    const int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
    while (!open.empty()) {
        const auto node = open.top(); open.pop();
        const int nodeKey = key(node.x, node.y);
        if (node.cost != cost[static_cast<std::size_t>(nodeKey)]) continue;
        if (node.x == gx && node.y == gy) break;
        for (int i = 0; i < 4; ++i) {
            const int x = node.x + dx[i], y = node.y + dy[i];
            if (!IsWalkable(x, y)) continue;
            const int nextKey = key(x, y), nextCost = node.cost + 1;
            if (nextCost >= cost[static_cast<std::size_t>(nextKey)]) continue;
            cost[static_cast<std::size_t>(nextKey)] = nextCost;
            previous[static_cast<std::size_t>(nextKey)] = nodeKey;
            open.push({x, y, nextCost, nextCost + heuristic(x, y)});
        }
    }
    int cursor = key(gx, gy), source = key(sx, sy);
    if (cursor != source && previous[static_cast<std::size_t>(cursor)] == -1) return {};
    std::vector<Vector2> path;
    while (cursor != source) {
        path.push_back({static_cast<float>(cursor % width_), static_cast<float>(cursor / width_)});
        cursor = previous[static_cast<std::size_t>(cursor)];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void World::DrawGid(int x, int y, unsigned int rawGid, float opacity) const {
    const unsigned int gid = rawGid & kGidMask; // Tiled flip flags are masked; diagonal flips are not yet applied.
    if (gid == 0) return;
    const Vector2 center = GridToWorld({static_cast<float>(x), static_cast<float>(y)});
    const TileSet* set = TilesetFor(gid);
    if (set && set->loaded && set->columns > 0) {
        const int localId = static_cast<int>(gid) - set->firstGid;
        const Rectangle source{static_cast<float>((localId % set->columns) * set->tileWidth),
                               static_cast<float>((localId / set->columns) * set->tileHeight),
                               static_cast<float>(set->tileWidth), static_cast<float>(set->tileHeight)};
        const Rectangle dest{center.x - tileWidth_ * 0.5f, center.y - tileHeight_ * 0.5f,
                             static_cast<float>(tileWidth_), static_cast<float>(tileHeight_)};
        DrawTexturePro(set->texture, source, dest, {0, 0}, 0, Fade(WHITE, opacity));
        return;
    }
    Color color = Color{82, 142, 91, 255};
    const TileSet* fallback = set;
    const int localId = fallback ? static_cast<int>(gid) - fallback->firstGid : static_cast<int>(gid) - 1;
    if (localId == 1) color = Color{190, 157, 105, 255};
    else if (localId == 2) color = Color{65, 133, 178, 255};
    else if (localId == 3) color = Color{104, 108, 112, 255};
    const Vector2 top{center.x, center.y - tileHeight_ * 0.5f}, right{center.x + tileWidth_ * 0.5f, center.y};
    const Vector2 bottom{center.x, center.y + tileHeight_ * 0.5f}, left{center.x - tileWidth_ * 0.5f, center.y};
    DrawTriangle(top, left, bottom, Fade(color, opacity)); DrawTriangle(top, bottom, right, Fade(color, opacity));
    DrawLineV(top, right, Fade(BLACK, 0.25f * opacity)); DrawLineV(right, bottom, Fade(BLACK, 0.25f * opacity));
    DrawLineV(bottom, left, Fade(BLACK, 0.25f * opacity)); DrawLineV(left, top, Fade(BLACK, 0.25f * opacity));
}

void World::Draw() const {
    for (const auto& layer : layers_) {
        if (!layer.visible) continue;
        for (int sum = 0; sum < width_ + height_ - 1; ++sum) {
            for (int y = 0; y < height_; ++y) {
                const int x = sum - y;
                if (x < 0 || x >= width_) continue;
                const auto index = static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) + static_cast<std::size_t>(x);
                DrawGid(x, y, layer.gids[index], layer.opacity);
            }
        }
    }
}
