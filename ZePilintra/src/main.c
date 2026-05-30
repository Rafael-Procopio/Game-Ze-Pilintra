#include "raylib.h"
#include "game.h"
#include "player.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Ze Pilintra");
    SetTargetFPS(60);

    // Desabilita fechamento automático com ESC
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = SCREEN_MENU;

    Player player;
    InitPlayer(&player);

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

        ClearBackground(RAYWHITE);

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
                DrawText("FASE 1 - AINDA NÃO SEI", 20, 20, 30, BLACK);

                // Chão
                DrawRectangle(
                    0,
                    600,
                    screenWidth,
                    120,
                    DARKGREEN
                );

                DrawPlayer(player);

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