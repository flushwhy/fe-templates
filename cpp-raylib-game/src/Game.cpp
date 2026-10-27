#include "Game.hpp"

#include "Player.hpp"

Game::Game() {
    InitWindow(960, 540, "{{.Name}}");
    SetTargetFPS(60);

    entities_.push_back(std::make_unique<Player>(
        Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}));
}

Game::~Game() {
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update(GetFrameTime());

        BeginDrawing();
        Draw();
        EndDrawing();
    }
}

void Game::Update(float deltaTime) {
    for (const auto& entity : entities_) {
        entity->Update(deltaTime);
    }
}

void Game::Draw() const {
    ClearBackground(Color{24, 30, 38, 255});

    for (const auto& entity : entities_) {
        entity->Draw();
    }

    DrawText("{{.Name}}", 24, 20, 24, RAYWHITE);
    DrawText("Move with WASD or arrow keys", 24, 52, 18, Color{180, 190, 200, 255});
}
