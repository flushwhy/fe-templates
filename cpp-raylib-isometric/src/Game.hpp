#pragma once
#include "Npc.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "raylib.h"
#include <string>
class Game {
public: Game();~Game();bool Initialize();void Run();
private:
 World world_;Player player_{{2,2}};Npc npc_{{7,7},{2,2},"Mira","Welcome! Use WASD or arrows to explore."};
 Camera2D camera_{};std::string dialogue_;float dialogueTimer_=0;
 void Update(float dt);void Draw()const;
};