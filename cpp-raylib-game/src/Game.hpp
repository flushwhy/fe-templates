#pragma once

#include <memory>
#include <vector>

#include "Entity.hpp"

class Game {
public:
    Game();
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void Run();

private:
    void Update(float deltaTime);
    void Draw() const;

    std::vector<std::unique_ptr<Entity>> entities_;
};
