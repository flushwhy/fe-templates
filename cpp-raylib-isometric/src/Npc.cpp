#include "Npc.hpp"

#include "raymath.h"
#include <utility>

Npc::Npc(Vector2 start, Vector2 goal, std::string name, std::string dialogue)
    : renderGrid_(start), home_(start), patrolGoal_(goal), goal_(goal),
      name_(std::move(name)), dialogue_(std::move(dialogue)) {}

void Npc::LoadAssets(const std::string& path) {
    sprite_.Load(path, 32, 32, 4);
}

void Npc::Unload() {
    sprite_.Unload();
}

void Npc::Update(float deltaTime, const World& world) {
    if (path_.empty()) {
        if (Vector2Distance(renderGrid_, goal_) < 0.5f)
            goal_ = Vector2Distance(renderGrid_, home_) < 0.5f ? patrolGoal_ : home_;
        path_ = world.FindPath(renderGrid_, goal_);
        nextStep_ = 0;
    }
    if (nextStep_ >= path_.size()) {
        path_.clear();
        sprite_.Update(deltaTime, false);
        return;
    }

    const Vector2 target = path_[nextStep_];
    const Vector2 delta = Vector2Subtract(target, renderGrid_);
    const float distance = Vector2Length(delta), step = speed_ * deltaTime;
    if (distance <= step) {
        renderGrid_ = target;
        ++nextStep_;
    } else if (distance > 0.0f) {
        renderGrid_ = Vector2Add(renderGrid_, Vector2Scale(Vector2Normalize(delta), step));
    }
    sprite_.Update(deltaTime, true);
}

void Npc::Draw(const World& world) const {
    const Vector2 ground = world.GridToWorld(renderGrid_);
    if (sprite_.IsLoaded()) sprite_.Draw({ground.x - 16.0f, ground.y - 32.0f}, SKYBLUE);
    else {
        DrawCircleV({ground.x, ground.y - 8.0f}, 8.0f, SKYBLUE);
        DrawCircleLines(static_cast<int>(ground.x), static_cast<int>(ground.y - 8.0f), 8.0f, DARKBLUE);
    }
    DrawText(name_.c_str(), static_cast<int>(ground.x - 24), static_cast<int>(ground.y - 30), 12, RAYWHITE);
}