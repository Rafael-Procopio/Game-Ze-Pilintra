#include "enemy.h"
#include <stdio.h>

static const char *GetEnemyTypeName(EnemyType type)
{
    switch (type)
    {
        case ENEMY_GOBLIN: return "Goblin";
        case ENEMY_BANDIT: return "Bandit";
        case ENEMY_SKELETON: return "Skeleton";
        case ENEMY_ELITE: return "Elite";
        default: return "Unknown";
    }
}

static Color GetEnemyColor(EnemyType type)
{
    switch (type)
    {
        case ENEMY_GOBLIN: return GREEN;
        case ENEMY_BANDIT: return ORANGE;
        case ENEMY_SKELETON: return LIGHTGRAY;
        case ENEMY_ELITE: return PURPLE;
        default: return RED;
    }
}

void InitEnemy(
    Enemy *enemy,
    float x,
    float y,
    EnemyType type
)
{
    enemy->body = (Rectangle)
    {
        x,
        y,
        50,
        60
    };

    enemy->type = type;
    enemy->alive = true;
    enemy->respawnTimer = 0;
    enemy->direction = 1;

    switch (type)
    {
        case ENEMY_GOBLIN:
            enemy->hp = 30;
            enemy->maxHp = 30;
            enemy->damage = 2;
            enemy->xpReward = 25;
            enemy->speed = 1.5f;
            break;

        case ENEMY_BANDIT:
            enemy->hp = 50;
            enemy->maxHp = 50;
            enemy->damage = 4;
            enemy->xpReward = 50;
            enemy->speed = 2.0f;
            break;

        case ENEMY_SKELETON:
            enemy->hp = 80;
            enemy->maxHp = 80;
            enemy->damage = 6;
            enemy->xpReward = 100;
            enemy->speed = 1.2f;
            break;

        case ENEMY_ELITE:
            enemy->hp = 150;
            enemy->maxHp = 150;
            enemy->damage = 10;
            enemy->xpReward = 250;
            enemy->speed = 2.5f;
            break;

        default:
            enemy->hp = 30;
            enemy->maxHp = 30;
            enemy->damage = 2;
            enemy->xpReward = 25;
            enemy->speed = 1.5f;
            break;
    }
}

void UpdateEnemy(Enemy *enemy, Vector2 playerPos)
{
    if (!enemy->alive)
    {
        // Only respawn if a positive respawn timer was set
        if (enemy->respawnTimer > 0.0f)
        {
            enemy->respawnTimer -= GetFrameTime();
            if (enemy->respawnTimer <= 0.0f)
            {
                enemy->alive = true;
                enemy->hp = enemy->maxHp;
            }
        }

        return;
    }

    float distance = playerPos.x - enemy->body.x;

    if (distance > -250 && distance < 250)
    {
        if (distance > 0)
        {
            enemy->body.x += enemy->speed;
        }
        else
        {
            enemy->body.x -= enemy->speed;
        }
    }
    else
    {
        enemy->body.x += enemy->speed * enemy->direction;

        if (enemy->body.x < 700)
        {
            enemy->direction = 1;
        }

        if (enemy->body.x > 1000)
        {
            enemy->direction = -1;
        }
    }
}

void DrawEnemy(Enemy enemy)
{
    if (!enemy.alive)
        return;

    const char *typeName = GetEnemyTypeName(enemy.type);
    Color enemyColor = GetEnemyColor(enemy.type);
    char hpText[32];

    snprintf(hpText, sizeof(hpText), "HP: %i", enemy.hp);

    int typeTextWidth = MeasureText(typeName, 20);
    int hpTextWidth = MeasureText(hpText, 18);

    DrawRectangleRec(enemy.body, enemyColor);

    DrawText(typeName,
        (int)(enemy.body.x + (enemy.body.width - typeTextWidth) / 2),
        (int)(enemy.body.y - 34),
        20,
        BLACK
    );

    DrawText(hpText,
        (int)(enemy.body.x + (enemy.body.width - hpTextWidth) / 2),
        (int)(enemy.body.y - 16),
        18,
        BLACK
    );
}