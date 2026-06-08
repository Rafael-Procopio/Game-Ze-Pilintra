#include <stdio.h>

#include "raylib.h"
#include "game.h"
#include "player.h"
#include "equipment.h"
#include "inventory.h"
#include "drops.h"
#include "map.h"
#include "stats.h"
#include "enemy.h"
#include "combat.h"
#include "wave.h"
#include "logic.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Ze Pilintra");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = SCREEN_MENU;
    bool inventoryOpen = false;
    bool equipmentOpen = false;
    bool godMode = false;
    int inventorySelectedIndex = 0;
    int equipmentSelectedSlot = 0;

    LogicQuestion currentQuestion;
    int logicCorrect = 0;
    int logicWrong = 0;
    int logicRewardXP = 100;
    int lastLogicWave = 0;
    bool logicChallengeActive = false;
    bool lastLogicAnswerCorrect = false;
    char logicResultText[100];
    LogicQuestionType correctAnswer;

    Player player;
    InitPlayer(&player);
    InitDropNotification();

    Enemy enemies[MAX_ENEMIES];
    WaveSystem wave;
    InitWaveSystem(&wave);
    StartWave(&wave, 1, enemies);

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

    // Debug visualization timer for attack hitbox
    float debugHitboxTimer = 0.0f;
    float debugHitboxDuration = 0.3f;

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

                if (IsKeyPressed(KEY_I))
                {
                    inventoryOpen = !inventoryOpen;
                    if (inventoryOpen)
                    {
                        if (player.inventory.itemCount <= 0)
                        {
                            inventorySelectedIndex = 0;
                        }
                        else if (inventorySelectedIndex >= player.inventory.itemCount)
                        {
                            inventorySelectedIndex = player.inventory.itemCount - 1;
                        }
                    }
                }

                if (IsKeyPressed(KEY_L))
                {
                    currentQuestion = GetRandomQuestion();
                    currentScreen = SCREEN_LOGIC_CHALLENGE;
                }

                if (IsKeyPressed(KEY_E))
                {
                    equipmentOpen = !equipmentOpen;
                    if (equipmentOpen)
                    {
                        equipmentSelectedSlot = 0;
                    }
                }

                // Test item pickup keys
                if (IsKeyPressed(KEY_F1))
                {
                    AddItem(&player.inventory, "Potion", 1);
                }

                if (IsKeyPressed(KEY_F2))
                {
                    AddItem(&player.inventory, "Gold Coin", 1);
                }

                if (IsKeyPressed(KEY_F3))
                {
                    AddItem(&player.inventory, "Iron Sword", 1);
                }

                if (IsKeyPressed(KEY_F5))
                {
                    AddItem(&player.inventory, "Iron Sword", 1);
                }

                if (IsKeyPressed(KEY_F6))
                {
                    AddItem(&player.inventory, "Leather Armor", 1);
                }

                if (IsKeyPressed(KEY_F7))
                {
                    AddItem(&player.inventory, "Lucky Ring", 1);
                }

                if (IsKeyPressed(KEY_F8))
                {
                    AddItem(&player.inventory, "Steel Sword", 1);
                }

                if (IsKeyPressed(KEY_F10))
                {
                    godMode = !godMode;
                    if (godMode && player.stats.hp <= 0)
                    {
                        player.stats.hp = 1;
                    }
                }

                // Wave skip (debug)
                if (IsKeyPressed(KEY_F9))
                {
                    Wave_SkipCurrent(&wave);
                }

                if (inventoryOpen)
                {
                    if (IsKeyPressed(KEY_UP))
                    {
                        if (player.inventory.itemCount > 0)
                        {
                            inventorySelectedIndex--;
                            if (inventorySelectedIndex < 0)
                                inventorySelectedIndex = player.inventory.itemCount - 1;
                        }
                    }

                    if (IsKeyPressed(KEY_DOWN))
                    {
                        if (player.inventory.itemCount > 0)
                        {
                            inventorySelectedIndex++;
                            if (inventorySelectedIndex >= player.inventory.itemCount)
                                inventorySelectedIndex = 0;
                        }
                    }

                    if (IsKeyPressed(KEY_ENTER) && player.inventory.itemCount > 0)
                    {
                        // Try to use consumable first, otherwise attempt to equip
                        if (!UseItem(&player.inventory, &player, inventorySelectedIndex))
                        {
                            if (EquipItem(&player, player.inventory.slots[inventorySelectedIndex].name))
                            {
                                if (player.inventory.itemCount == 0)
                                {
                                    inventorySelectedIndex = 0;
                                }
                                else if (inventorySelectedIndex >= player.inventory.itemCount)
                                {
                                    inventorySelectedIndex = player.inventory.itemCount - 1;
                                }
                            }
                        }
                        else
                        {
                            // After using, clamp selected index
                            if (player.inventory.itemCount == 0)
                            {
                                inventorySelectedIndex = 0;
                            }
                            else if (inventorySelectedIndex >= player.inventory.itemCount)
                            {
                                inventorySelectedIndex = player.inventory.itemCount - 1;
                            }
                        }
                    }
                }
                else if (equipmentOpen)
                {
                    if (IsKeyPressed(KEY_UP))
                    {
                        equipmentSelectedSlot--;
                        if (equipmentSelectedSlot < 0)
                            equipmentSelectedSlot = 2;
                    }

                    if (IsKeyPressed(KEY_DOWN))
                    {
                        equipmentSelectedSlot++;
                        if (equipmentSelectedSlot > 2)
                            equipmentSelectedSlot = 0;
                    }

                    if (IsKeyPressed(KEY_U))
                    {
                        switch (equipmentSelectedSlot)
                        {
                            case 0: UnequipWeapon(&player); break;
                            case 1: UnequipArmor(&player); break;
                            case 2: UnequipAccessory(&player); break;
                        }
                    }
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
                        if (!godMode)
                        {
                            player.stats.hp -= enemies[i].damage;

                            if (player.stats.hp < 0)
                            {
                                player.stats.hp = 0;
                            }
                        }
                    }
                }

                if (!godMode && player.stats.hp <= 0)
                {
                    currentScreen = SCREEN_GAMEOVER;
                }

                // Ataque do jogador usando hitbox
                if (IsKeyPressed(KEY_J))
                {
                    // Trigger attack animation
                    player.animationState = PLAYER_ATTACK;
                    player.currentFrame = 0;
                    player.frameCounter = 0;
                    player.attackTimer = 0.25f;

                    Rectangle attackHitbox = CreateAttackHitbox(player.body, player.facingRight);
                    
                    // Activate debug visualization
                    debugHitboxTimer = debugHitboxDuration;

                    for (int i = 0; i < MAX_ENEMIES; i++)
                    {
                        if (
                            enemies[i].alive &&
                            CheckCollisionRecs(
                                attackHitbox,
                                enemies[i].body
                            )
                        )
                        {
                            enemies[i].hp -= GetPlayerAttackValue(&player);

                            if (enemies[i].hp <= 0)
                            {
                                enemies[i].alive = false;
                                enemies[i].respawnTimer = 0.0f; // don't auto-respawn in wave mode

                                AddXP(&player.stats, enemies[i].xpReward);
                                GenerateDropsByEnemyType(enemies[i].type, &player.inventory);

                                Wave_NotifyEnemyDefeated(&wave, enemies[i].type, &player);
                            }
                        }
                    }
                }

                // Update debug visualization timer
                if (debugHitboxTimer > 0)
                {
                    debugHitboxTimer -= GetFrameTime();
                }

                camera.target = (Vector2){
                    player.body.x + player.body.width / 2,
                    player.body.y + player.body.height / 2
                };

                UpdateDropNotification(GetFrameTime());
                UpdateEquipmentNotification(GetFrameTime());
                UpdateHealEffects(GetFrameTime());
                UpdateWaveSystem(&wave, GetFrameTime(), enemies, &player);

                if (
                    wave.currentWave % 3 == 0 &&
                    wave.currentWave != 0 &&
                    wave.currentWave != lastLogicWave
                )
                {
                    lastLogicWave = wave.currentWave;

                    logicRewardXP = wave.currentWave * 50;

                    currentQuestion = GetRandomQuestion();

                    currentScreen = SCREEN_LOGIC_CHALLENGE;
                }

                break;
            }

            case SCREEN_GAMEOVER:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    UnloadPlayer(&player);
                    InitPlayer(&player);

                    // Reset wave system to start from wave 1
                    InitWaveSystem(&wave);
                    StartWave(&wave, 1, enemies);

                    currentScreen = SCREEN_MENU;
                }

                break;
            }

            case SCREEN_PAUSE:
            {
                break;
            }

            case SCREEN_LOGIC_CHALLENGE:
            {
                if (IsKeyPressed(KEY_ONE))
                {
                    if (currentQuestion.answer == LOGIC_TAUTOLOGY)
                    {
                        logicCorrect++;

                        AddXP(
                            &player.stats,
                            logicRewardXP
                        );

                        lastLogicAnswerCorrect = true;

                        sprintf(
                            logicResultText,
                            "+%d XP",
                            logicRewardXP
                        );
                    }
                    else
                    {
                        logicWrong++;

                        lastLogicAnswerCorrect = false;

                        correctAnswer = currentQuestion.answer;
                    }

                    currentScreen = SCREEN_LOGIC_RESULT;
                }

                if (IsKeyPressed(KEY_TWO))
                {
                    if (currentQuestion.answer == LOGIC_CONTRADICTION)
                    {
                        logicCorrect++;

                        AddXP(
                            &player.stats,
                            logicRewardXP
                        );

                        lastLogicAnswerCorrect = true;

                        sprintf(
                            logicResultText,
                            "+%d XP",
                            logicRewardXP
                        );
                    }
                    else
                    {
                        logicWrong++;

                        lastLogicAnswerCorrect = false;

                        correctAnswer = currentQuestion.answer;
                    }

                    currentScreen = SCREEN_LOGIC_RESULT;
                }

                if (IsKeyPressed(KEY_THREE))
                {
                    if (currentQuestion.answer == LOGIC_CONTINGENCY)
                    {
                        logicCorrect++;

                        AddXP(
                            &player.stats,
                            logicRewardXP
                        );

                        lastLogicAnswerCorrect = true;

                        sprintf(
                            logicResultText,
                            "+%d XP",
                            logicRewardXP
                        );
                    }
                    else
                    {
                        logicWrong++;

                        lastLogicAnswerCorrect = false;

                        correctAnswer = currentQuestion.answer;
                    }

                    currentScreen = SCREEN_LOGIC_RESULT;
                }

                break;
            }

            case SCREEN_LOGIC_RESULT:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = SCREEN_GAME;
                }

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

                // Debug visualization of attack hitbox
                if (debugHitboxTimer > 0)
                {
                    Rectangle debugHitbox = CreateAttackHitbox(player.body, player.facingRight);
                    DrawRectangleRec(debugHitbox, GREEN);
                }

                EndMode2D();

                DrawText("FASE 1 - NÃO SEI", 20, 20, 30, BLACK);

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

                DrawText(
                    "I = Abrir/Fechar Inventario",
                    20,
                    520,
                    20,
                    DARKBLUE
                );

                DrawText(
                    "E = Abrir/Fechar Equipment",
                    20,
                    550,
                    20,
                    DARKBLUE
                );

                DrawText(
                    "F1 = Add Potion | F2 = Add Gold Coin | F3 = Add Iron Sword",
                    20,
                    580,
                    20,
                    DARKGRAY
                );

                DrawText(
                    "F5 = Add Iron Sword | F6 = Add Leather Armor | F7 = Add Lucky Ring | F8 = Add Steel Sword",
                    20,
                    610,
                    20,
                    DARKGRAY
                );

                DrawText(
                    "[UP/DOWN] select item or slot | ENTER = Equip | U = Unequip",
                    20,
                    640,
                    20,
                    DARKGRAY
                );

                DrawText(
                    "F10 = Toggle God Mode",
                    20,
                    640,
                    20,
                    DARKBLUE
                );

                DrawText(
                    TextFormat("God Mode: %s", godMode ? "ON" : "OFF"),
                    20,
                    670,
                    20,
                    godMode ? GREEN : RED
                );

                int aliveEnemies = 0;
                int goblinsAlive = 0;
                int banditsAlive = 0;
                int skeletonsAlive = 0;
                int elitesAlive = 0;

                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (!enemies[i].alive) continue;

                    aliveEnemies++;

                    switch (enemies[i].type)
                    {
                        case ENEMY_GOBLIN: goblinsAlive++; break;
                        case ENEMY_BANDIT: banditsAlive++; break;
                        case ENEMY_SKELETON: skeletonsAlive++; break;
                        case ENEMY_ELITE: elitesAlive++; break;
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

                DrawText(
                    TextFormat(
                        "Goblins: %i",
                        goblinsAlive
                    ),
                    20,
                    480,
                    20,
                    DARKGREEN
                );

                DrawText(
                    TextFormat(
                        "Bandits: %i",
                        banditsAlive
                    ),
                    20,
                    510,
                    20,
                    ORANGE
                );

                DrawText(
                    TextFormat(
                        "Skeletons: %i",
                        skeletonsAlive
                    ),
                    20,
                    540,
                    20,
                    LIGHTGRAY
                );

                DrawText(
                    TextFormat(
                        "Elites: %i",
                        elitesAlive
                    ),
                    20,
                    570,
                    20,
                    PURPLE
                );

                if (inventoryOpen)
                {
                    DrawInventory(player.inventory, inventorySelectedIndex);
                }

                DrawHealEffects();

                if (equipmentOpen)
                {
                    DrawEquipmentPanel(&player.equipment, equipmentSelectedSlot);
                    DrawEquipmentBonusPanel(&player.equipment);
                    DrawCharacterPanel(&player);
                }

                DrawWaveHUD(&wave);
                DrawWaveAnnouncement(&wave);

                DrawDropNotification();
                DrawDropDebugPanel();

                break;
            }

            case SCREEN_GAMEOVER:
            {
                DrawText(
                    "GAME OVER",
                    420,
                    220,
                    70,
                    RED
                );

                DrawText(
                    "Press ENTER to return",
                    400,
                    330,
                    30,
                    DARKGRAY
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

            case SCREEN_LOGIC_CHALLENGE:
            {
                DrawText(
                    TextFormat("Reward: %d XP", logicRewardXP),
                    300,
                    270,
                    25,
                    DARKGREEN
                );
                
                DrawText(
                    "LOGIC CHALLENGE",
                    350,
                    100,
                    40,
                    BLACK
                );

                DrawText(
                    currentQuestion.proposition,
                    300,
                    220,
                    40,
                    DARKBLUE
                );

                DrawText(
                    "1 - Tautology",
                    300,
                    320,
                    30,
                    BLACK
                );

                DrawText(
                    "2 - Contradiction",
                    300,
                    370,
                    30,
                    BLACK
                );

                DrawText(
                    "3 - Contingency",
                    300,
                    420,
                    30,
                    BLACK
                );

                break;
            }

            case SCREEN_LOGIC_RESULT:
            {
                if (lastLogicAnswerCorrect)
                {
                    DrawText(
                        "CORRETO!",
                        450,
                        180,
                        50,
                        DARKGREEN
                    );

                    DrawText(
                        logicResultText,
                        500,
                        260,
                        40,
                        GREEN
                    );
                }
                else
                {
                    DrawText(
                        "INCORRETO!",
                        430,
                        180,
                        50,
                        RED
                    );

                    DrawText(
                        "Resposta correta:",
                        420,
                        260,
                        35,
                        BLACK
                    );

                    const char *answerText = "";

                    switch (correctAnswer)
                    {
                        case LOGIC_TAUTOLOGY:
                            answerText = "TAUTOLOGY";
                            break;

                        case LOGIC_CONTRADICTION:
                            answerText = "CONTRADICTION";
                            break;

                        case LOGIC_CONTINGENCY:
                            answerText = "CONTINGENCY";
                            break;
                    }

                    DrawText(
                        answerText,
                        450,
                        320,
                        40,
                        BLUE
                    );
                }

                DrawText(
                    "ENTER para continuar",
                    400,
                    450,
                    30,
                    DARKGRAY
                );

                break;
            }

        }

        EndDrawing();
    }

    UnloadPlayer(&player);

    CloseWindow();

    return 0;
}