#include "Game.hpp"

#include <algorithm>

Game::Game() = default;

Game::~Game() {
    CloseWindow();
}

bool Game::Initialize() {
    InitWindow(960, 640, "{{.Name}}");
    if (!IsWindowReady()) return false;
    SetTargetFPS(60);

    if (!world_.Load("assets/world.tmj")) return false;
    camera_.offset = Vector2{GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f};
    camera_.rotation = 0.0f;
    camera_.zoom = 1.8f;
    return true;
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update(GetFrameTime());
        BeginDrawing();
        ClearBackground(Color{25, 32, 39, 255});
        BeginMode2D(camera_);
        Draw();
        EndMode2D();
        DrawText("WASD / arrows: move   |   ESC: quit", 18, 18, 18, RAYWHITE);
        DrawText("Map: assets/world.tmj (Tiled isometric JSON)", 18, 42, 16, Color{185, 195, 202, 255});
        EndDrawing();
    }
}

void Game::Update(float deltaTime) {
    player_.Update(deltaTime, world_);
    const Vector2 playerWorld = world_.GridToWorld(player_.GridPosition());
    camera_.target = Vector2{playerWorld.x, playerWorld.y};
}

void Game::Draw() const {
    world_.Draw();
    player_.Draw(world_);
}
