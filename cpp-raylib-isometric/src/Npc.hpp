#pragma once

#include "AnimatedSprite.hpp"
#include "Entity.hpp"
#include "World.hpp"

#include <string>
#include <vector>

class Npc : public Entity {
public:
    Npc(Vector2 start, Vector2 goal, std::string name, std::string dialogue);
    void LoadAssets(const std::string& path);
    void Unload();
    void Update(float deltaTime, const World& world) override;
    void Draw(const World& world) const override;
    Vector2 GridPosition() const override { return renderGrid_; }
    Vector2 RenderGridPosition() const override { return renderGrid_; }
    const std::string& Name() const { return name_; }
    const std::string& Dialogue() const { return dialogue_; }

private:
    Vector2 renderGrid_, home_, patrolGoal_, goal_;
    std::vector<Vector2> path_;
    std::size_t nextStep_ = 0;
    float speed_ = 2.5f;
    std::string name_, dialogue_;
    AnimatedSprite sprite_;
};