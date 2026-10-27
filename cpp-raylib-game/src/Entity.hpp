#pragma once

#include "raylib.h"

// Base class for anything that updates and draws in the game world.
class Entity {
public:
    virtual ~Entity() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() const = 0;
};
