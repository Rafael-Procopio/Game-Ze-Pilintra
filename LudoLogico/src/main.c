#include <raylib.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zé Pilintra");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(
            "Zé Pilintra",
            SCREEN_WIDTH / 2 - 180,
            50,
            40,
            BLACK
        );

        DrawText(
            "Primeira versao",
            SCREEN_WIDTH / 2 - 100,
            100,
            20,
            DARKGRAY
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}