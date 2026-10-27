#pragma once

#include "Entity.hpp"

class Player final : public Entity {
public:
    explicit Player(Vector2 startPosition);

    void Update(float deltaTime) override;
    void Draw() const override;

private:
    Vector2 position_;
    float speed_ = 260.0f;
    float size_ = 32.0f;
};
