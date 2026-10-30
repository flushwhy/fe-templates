#include "Player.hpp"
#include "raymath.h"

void Player::LoadAssets(const std::string& path) {
    sprite_.Load(path, 32, 32, 4);
}

void Player::Unload() {
    sprite_.Unload();
}

void Player::BeginMove(Vector2 direction, const World& world) {
    const int x = static_cast<int>(gridPosition_.x + direction.x);
    const int y = static_cast<int>(gridPosition_.y + direction.y);
    if (world.IsWalkable(x, y)) {
        targetGrid_ = {static_cast<float>(x), static_cast<float>(y)};
        moving_ = true;
    }
}

void Player::Update(float deltaTime, const World& world) {
    if (moving_) {
        const Vector2 delta = Vector2Subtract(targetGrid_, renderGrid_);
        const float distance = Vector2Length(delta), step = moveSpeed_ * deltaTime;
        if (distance <= step) {
            renderGrid_ = targetGrid_;
            gridPosition_ = targetGrid_;
            moving_ = false;
        } else {
            renderGrid_ = Vector2Add(renderGrid_, Vector2Scale(Vector2Normalize(delta), step));
        }
        sprite_.Update(deltaTime, true);
        return;
    }

    Vector2 direction{0, 0};
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) direction.x = 1;
    else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) direction.x = -1;
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) direction.y = 1;
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) direction.y = -1;
    if (direction.x != 0 || direction.y != 0) BeginMove(direction, world);
    sprite_.Update(deltaTime, moving_);
}

void Player::Draw(const World& world) const {
    const Vector2 p = world.GridToWorld(renderGrid_);
    if (sprite_.IsLoaded()) {
        sprite_.Draw({p.x - 16.0f, p.y - 32.0f}, WHITE);
        return;
    }
    DrawCircleV({p.x, p.y - 9.0f}, 9.0f, {232, 118, 73, 255});
    DrawCircleLines(static_cast<int>(p.x), static_cast<int>(p.y - 9.0f), 9.0f, MAROON);
}