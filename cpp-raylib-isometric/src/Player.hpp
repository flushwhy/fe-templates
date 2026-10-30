#pragma once

#include "AnimatedSprite.hpp"
#include "Entity.hpp"
#include "World.hpp"

class Player : public Entity {
public:
    explicit Player(Vector2 position) : gridPosition_(position), targetGrid_(position), renderGrid_(position) {}
    void LoadAssets(const std::string& path);
    void Unload();
    void Update(float deltaTime, const World& world) override;
    void Draw(const World& world) const override;
    Vector2 GridPosition() const override { return gridPosition_; }
    Vector2 RenderGridPosition() const override { return renderGrid_; }

private:
    Vector2 gridPosition_, targetGrid_, renderGrid_;
    bool moving_ = false;
    float moveSpeed_ = 6.0f;
    AnimatedSprite sprite_;

    void BeginMove(Vector2 direction, const World& world);
};