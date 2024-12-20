#include "broadcast.h"
#include "blockchain.h"
#include "design.h"
constexpr int PORT = 12345;

int main() {
    LOG("INIT: program started");

    network net(PORT);
    blockchain chain(0, std::move(net));
    const float screenWidth = 1000;
    const float screenHeight = 1000;
    InitWindow(screenWidth, screenWidth, "Thick Arrow Example");
    float rotationAngle = 0.0f;

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0 }; // Центр камеры
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 }; // Смещение для корректного отображения
    camera.zoom = 1.0f;
    while (!WindowShouldClose()) {
        chain.run();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            camera.target.x = mousePosition.x + camera.target.x - screenWidth / 2; // Перемещение камеры по X
            camera.target.y = mousePosition.y + camera.target.y - screenHeight / 2; // Перемещение камеры по Y
        }
        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(RAYWHITE);
        float child_coord_x = 0.0f, child_coord_y =  0.0f;
        bool is_root = true;
        DrawTree(chain.root(), 50, 30, rotationAngle, child_coord_x, child_coord_y, is_root);

        EndMode2D();
        EndDrawing();
           
    }
    

    CloseWindow();
}

