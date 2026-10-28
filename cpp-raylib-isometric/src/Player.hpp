#pragma once

#include "raylib.h"
#include "World.hpp"

class Player {
public:
    explicit Player(Vector2 gridPosition) : gridPosition_(gridPosition) {}
    void Update(float deltaTime, const World& world);
    void Draw(const World& world) const;
    Vector2 GridPosition() const { return gridPosition_; }

private:
    Vector2 gridPosition_;
    float moveCooldown_ = 0.0f;
};
