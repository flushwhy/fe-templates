#include "Game.hpp"
#include <algorithm>
#include <cmath>

Game::Game() = default;
Game::~Game() { world_.Unload(); CloseWindow(); }

bool Game::Initialize() {
    InitWindow(960, 640, "{{.Name}}");
    if (!IsWindowReady()) return false;
    SetTargetFPS(60);
    if (!world_.Load("assets/world.tmj")) return false;
    player_ = Player(world_.ObjectPosition("PlayerSpawn", {2, 2}));
    npc_ = Npc(world_.ObjectPosition("Mira", {7, 7}), world_.ObjectPosition("MiraGoal", {2, 2}),
               "Mira", "Welcome! Use WASD or arrows to explore.");
    camera_.offset = {GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f};
    camera_.rotation = 0.0f;
    camera_.zoom = 1.8f;
    return true;
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update(GetFrameTime());
        BeginDrawing();
        ClearBackground({25, 32, 39, 255});
        BeginMode2D(camera_);
        Draw();
        EndMode2D();
        DrawText("WASD/arrows: move   E: talk   wheel: zoom   ESC: quit", 18, 18, 18, RAYWHITE);
        if (dialogueTimer_ > 0.0f) {
            DrawRectangle(18, GetScreenHeight() - 84, GetScreenWidth() - 36, 64, {18, 22, 28, 230});
            DrawText(dialogue_.c_str(), 32, GetScreenHeight() - 62, 18, RAYWHITE);
        }
        EndDrawing();
    }
}

void Game::Update(float dt) {
    player_.Update(dt, world_);
    npc_.Update(dt, world_);
    camera_.target = world_.GridToWorld(player_.RenderGridPosition());
    camera_.zoom = Clamp(camera_.zoom + GetMouseWheelMove() * 0.1f, 0.8f, 3.2f);
    dialogueTimer_ = std::max(0.0f, dialogueTimer_ - dt);
    if (IsKeyPressed(KEY_E)) {
        const Vector2 player = player_.GridPosition(), npc = npc_.GridPosition();
        if (std::abs(player.x - npc.x) + std::abs(player.y - npc.y) <= 1.5f) {
            dialogue_ = npc_.Dialogue();
            dialogueTimer_ = 4.0f;
        }
        for (const auto& object : world_.Objects()) {
            if (object.type != "interactable") continue;
            if (std::abs(player.x - object.gridPosition.x) + std::abs(player.y - object.gridPosition.y) <= 1.5f) {
                dialogue_ = object.name;
                dialogueTimer_ = 4.0f;
            }
        }
    }
}

void Game::Draw() const {
    world_.Draw();
    if (player_.RenderGridPosition().x + player_.RenderGridPosition().y <
        npc_.GridPosition().x + npc_.GridPosition().y) {
        player_.Draw(world_);
        npc_.Draw(world_);
    } else {
        npc_.Draw(world_);
        player_.Draw(world_);
    }
    for (const auto& object : world_.Objects()) {
        if (object.type == "interactable") {
            const Vector2 p = world_.GridToWorld(object.gridPosition);
            DrawCircleV({p.x, p.y - 5.0f}, 5.0f, GOLD);
        }
    }
}
