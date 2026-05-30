#include "raylib.h"
#include "game.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Ze Pilintra");
    SetTargetFPS(60);

    // Desabilita o fechamento automático com ESC
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = SCREEN_MENU;

    while (!WindowShouldClose())
    {
        // UPDATE
        switch (currentScreen)
        {
            case SCREEN_MENU:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = SCREEN_GAME;
                }

                // ESC fecha o jogo quando estiver no menu
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    break;
                }

                break;
            }

            case SCREEN_GAME:
            {
                // ESC volta para o menu
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    currentScreen = SCREEN_MENU;
                }

                break;
            }

            case SCREEN_PAUSE:
            {
                break;
            }
        }

        // DRAW
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
                DrawText("FASE 1 - NÃO SEI AINDA", 420, 180, 50, BLACK);
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