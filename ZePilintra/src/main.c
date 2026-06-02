#include "raylib.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "stats.h"
#include "enemy.h"

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

    Enemy enemies[MAX_ENEMIES];

    InitEnemy(&enemies[0], 800, 540);
    InitEnemy(&enemies[1], 1200, 540);
    InitEnemy(&enemies[2], 1700, 540);
    InitEnemy(&enemies[3], 2200, 540);
    InitEnemy(&enemies[4], 2800, 540);

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

                // XP de teste
                if (IsKeyPressed(KEY_K))
                {
                    AddXP(&player.stats, 50);
                }

                // Sistema de upgrades
                if (IsKeyPressed(KEY_ONE))
                {
                    UpgradeAttack(&player.stats);
                }

                if (IsKeyPressed(KEY_TWO))
                {
                    UpgradeDefense(&player.stats);
                }

                if (IsKeyPressed(KEY_THREE))
                {
                    UpgradeMaxHp(&player.stats);
                }

                UpdatePlayer(&player);

                // Atualiza todos os inimigos
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    UpdateEnemy(
                        &enemies[i],
                        (Vector2)
                        {
                            player.body.x,
                            player.body.y
                        }
                    );
                }

                // Dano recebido dos inimigos
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (
                        enemies[i].alive &&
                        CheckCollisionRecs(
                            player.body,
                            enemies[i].body
                        )
                    )
                    {
                        player.stats.hp--;

                        if (player.stats.hp < 0)
                        {
                            player.stats.hp = 0;
                        }
                    }
                }

                // Ataque do jogador
                if (IsKeyPressed(KEY_J))
                {
                    for (int i = 0; i < MAX_ENEMIES; i++)
                    {
                        if (
                            enemies[i].alive &&
                            CheckCollisionRecs(
                                player.body,
                                enemies[i].body
                            )
                        )
                        {
                            enemies[i].hp -= player.stats.attack;

                            if (enemies[i].hp <= 0)
                            {
                                enemies[i].alive = false;

                                enemies[i].respawnTimer = 5.0f;

                                AddXP(
                                    &player.stats,
                                    50
                                );
                            }
                        }
                    }
                }

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

                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    DrawEnemy(enemies[i]);
                }

                EndMode2D();

                DrawText("FASE 1 - LAPA", 20, 20, 30, BLACK);

                DrawText(
                    TextFormat("Nivel: %i", player.stats.level),
                    20,
                    60,
                    20,
                    BLACK
                );

                DrawText(
                    TextFormat(
                        "XP: %i/%i",
                        player.stats.xp,
                        player.stats.xpToNextLevel
                    ),
                    20,
                    90,
                    20,
                    BLACK
                );

                DrawText(
                    TextFormat(
                        "HP: %i/%i",
                        player.stats.hp,
                        player.stats.maxHp
                    ),
                    20,
                    120,
                    20,
                    BLACK
                );

                DrawText(
                    TextFormat(
                        "Ataque: %i",
                        player.stats.attack
                    ),
                    20,
                    150,
                    20,
                    BLACK
                );

                DrawText(
                    TextFormat(
                        "Defesa: %i",
                        player.stats.defense
                    ),
                    20,
                    180,
                    20,
                    BLACK
                );

                DrawText(
                    TextFormat(
                        "Skill Points: %i",
                        player.stats.skillPoints
                    ),
                    20,
                    210,
                    20,
                    BLACK
                );

                DrawText(
                    "J = Atacar",
                    20,
                    250,
                    20,
                    DARKGRAY
                );

                DrawText(
                    "K = Ganhar XP",
                    20,
                    280,
                    20,
                    DARKGRAY
                );

                DrawText(
                    "1 = +Ataque",
                    20,
                    350,
                    20,
                    DARKBLUE
                );

                DrawText(
                    "2 = +Defesa",
                    20,
                    380,
                    20,
                    DARKBLUE
                );

                DrawText(
                    "3 = +HP Max",
                    20,
                    410,
                    20,
                    DARKBLUE
                );

                int aliveEnemies = 0;

                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemies[i].alive)
                    {
                        aliveEnemies++;
                    }
                }

                DrawText(
                    TextFormat(
                        "Enemies Alive: %i",
                        aliveEnemies
                    ),
                    20,
                    450,
                    20,
                    RED
                );

                break;
            }

            case SCREEN_PAUSE:
            {
                DrawText(
                    "PAUSADO",
                    500,
                    200,
                    50,
                    BLACK
                );

                break;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}