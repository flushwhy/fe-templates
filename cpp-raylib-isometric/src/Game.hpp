#pragma once

#include "Player.hpp"
#include "World.hpp"
#include "raylib.h"

class Game {
public:
    Game();
    ~Game();
    bool Initialize();
    void Run();

private:
    World world_;
    Player player_{{5.0f, 5.0f}};
    Camera2D camera_{};

    void Update(float deltaTime);
    void Draw() const;
};
