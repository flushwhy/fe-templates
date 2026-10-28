#include "Npc.hpp"

#include "raymath.h"

Npc::Npc(Vector2 start, Vector2 goal, std::string name, std::string dialogue)
    : renderGrid_(start), name_(std::move(name)), dialogue_(std::move(dialogue)) {}

void Npc::Update(float deltaTime, const World& world) {
    if (path_.empty()) {
        const Vector2 goal{renderGrid_.x < 5.0f ? 7.0f : 2.0f, renderGrid_.y < 5.0f ? 7.0f : 2.0f};
        path_ = world.FindPath(renderGrid_, goal);
        nextStep_ = 0;
    }
    if (nextStep_ >= path_.size()) {
        path_.clear();
        return;
    }
    const Vector2 target = path_[nextStep_];
    const Vector2 delta = Vector2Subtract(target, renderGrid_);
    const float distance = Vector2Length(delta);
    const float step = speed_ * deltaTime;
    if (distance <= step) {
        renderGrid_ = target;
        ++nextStep_;
    } else if (distance > 0.0f) {
        renderGrid_ = Vector2Add(renderGrid_, Vector2Scale(Vector2Normalize(delta), step));
    }
}

void Npc::Draw(const World& world) const {
    const Vector2 ground = world.GridToWorld(renderGrid_);
    DrawCircleV(Vector2{ground.x, ground.y - 8.0f}, 8.0f, SKYBLUE);
    DrawCircleLines(static_cast<int>(ground.x), static_cast<int>(ground.y - 8.0f), 8.0f, DARKBLUE);
    DrawText(name_.c_str(), static_cast<int>(ground.x - 24), static_cast<int>(ground.y - 30), 12, RAYWHITE);
}
