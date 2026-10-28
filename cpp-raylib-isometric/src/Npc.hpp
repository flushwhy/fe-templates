#pragma once

#include "raylib.h"
#include "World.hpp"

#include <string>
#include <vector>

class Npc {
public:
    Npc(Vector2 start, Vector2 goal, std::string name, std::string dialogue);
    void Update(float deltaTime, const World& world);
    void Draw(const World& world) const;
    Vector2 GridPosition() const { return renderGrid_; }
    const std::string& Name() const { return name_; }
    const std::string& Dialogue() const { return dialogue_; }

private:
    Vector2 renderGrid_;\n    Vector2 goal_;
    std::vector<Vector2> path_;
    std::size_t nextStep_ = 0;
    float speed_ = 2.5f;
    std::string name_;
    std::string dialogue_;
};
