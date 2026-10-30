#pragma once

#include "raylib.h"

class World;

// Shared update/draw contract for anything that lives in the isometric world.
class Entity {
public:
    virtual ~Entity() = default;
    virtual void Update(float deltaTime, const World& world) = 0;
    virtual void Draw(const World& world) const = 0;
    virtual Vector2 GridPosition() const = 0;
    virtual Vector2 RenderGridPosition() const = 0;
};