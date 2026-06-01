#include "raylib.h"
#include "game.h"
#include "player.h"
#include "map.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Ze Pilintra");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = SCREEN_MENU;

    Player player;
    InitPlayer(&player);

    InitMap();

    Camera2D camera = { 0 };

    camera.target = (Vector2){
        player.body.x + player.body.width / 2,
        player.body.y + player.body.height / 2
    };

    camera.offset = (Vector2){
        screenWidth / 2.0f,
        screenHeight / 2.0f
    };

    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        // =====================
        // UPDATE
        // =====================

        switch (currentScreen)
        {
            case SCREEN_MENU:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = SCREEN_GAME;
                }

                break;
            }

            case SCREEN_GAME:
            {
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    currentScreen = SCREEN_MENU;
                }

                UpdatePlayer(&player);

                camera.target = (Vector2){
                    player.body.x + player.body.width / 2,
                    player.body.y + player.body.height / 2
                };

                break;
            }

            case SCREEN_PAUSE:
            {
                break;
            }
        }

        // =====================
        // DRAW
        // =====================

        BeginDrawing();
        ClearBackground(SKYBLUE);

        switch (currentScreen)
        {
            case SCREEN_MENU:
            {
                DrawText("ZE PILINTRA", 400, 180, 60, BLACK);
                DrawText("ENTER - Iniciar", 450, 320, 30, DARKGRAY);
                DrawText("Versao 0.1", 20, 680, 20, GRAY);

                break;
            }

            case SCREEN_GAME:
            {
                BeginMode2D(camera);

                DrawMap();

                DrawPlayer(player);

                EndMode2D();

                DrawText("FASE 1 - LAPA", 20, 20, 30, BLACK);

                break;
            }

            case SCREEN_PAUSE:
            {
                DrawText("PAUSADO", 500, 200, 50, BLACK);

                break;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}