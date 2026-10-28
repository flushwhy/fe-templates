#include "Player.hpp"

void Player::Update(float deltaTime, const World& world) {
    moveCooldown_ -= deltaTime;
    if (moveCooldown_ > 0.0f) return;

    Vector2 direction{0.0f, 0.0f};
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) direction.x += 1.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) direction.y += 1.0f;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) direction.y -= 1.0f;

    // Resolve one grid axis at a time for predictable tile movement.
    if (direction.x != 0.0f) direction.y = 0.0f;
    if (direction.x == 0.0f && direction.y == 0.0f) return;

    const int nextX = static_cast<int>(gridPosition_.x + direction.x);
    const int nextY = static_cast<int>(gridPosition_.y + direction.y);
    if (world.IsWalkable(nextX, nextY)) {
        gridPosition_.x = static_cast<float>(nextX);
        gridPosition_.y = static_cast<float>(nextY);
        moveCooldown_ = 0.14f;
    }
}

void Player::Draw(const World& world) const {
    const Vector2 ground = world.GridToWorld(gridPosition_);
    DrawCircleV(Vector2{ground.x, ground.y - 9.0f}, 9.0f, Color{232, 118, 73, 255});
    DrawCircleLines(static_cast<int>(ground.x), static_cast<int>(ground.y - 9.0f), 9.0f, MAROON);
}
