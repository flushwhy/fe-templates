#include "Player.hpp"

#include "raymath.h"

#include <algorithm>

Player::Player(Vector2 startPosition) : position_(startPosition) {}

void Player::Update(float deltaTime) {
    Vector2 direction{0.0f, 0.0f};

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) direction.x += 1.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) direction.y += 1.0f;

    if (Vector2LengthSqr(direction) > 0.0f) {
        direction = Vector2Normalize(direction);
        position_.x += direction.x * speed_ * deltaTime;
        position_.y += direction.y * speed_ * deltaTime;
    }

    position_.x = std::clamp(position_.x, 0.0f, static_cast<float>(GetScreenWidth()) - size_);
    position_.y = std::clamp(position_.y, 0.0f, static_cast<float>(GetScreenHeight()) - size_);
}

void Player::Draw() const {
    DrawRectangleV(position_, Vector2{size_, size_}, Color{225, 107, 76, 255});
}
