#include "Game.hpp"

#include <iostream>

int main() {
    Game game;
    if (!game.Initialize()) {
        std::cerr << "Failed to initialize game. Run it from the project root so assets/world.tmj can be found.\n";
        return 1;
    }
    game.Run();
    return 0;
}
