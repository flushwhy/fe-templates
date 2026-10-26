#include "raylib.h"

int main() {
    constexpr int width = 960;
    constexpr int height = 540;
    InitWindow(width, height, "{{.Name}}");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{24, 30, 38, 255});
        DrawText("{{.Name}}", 32, 32, 32, RAYWHITE);
        DrawText("Your game starts here.", 32, 80, 20, Color{225, 107, 76, 255});
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
