#include "enemy.h"

void InitEnemy(Enemy *enemy)
{
    enemy->body.x = 800;
    enemy->body.y = 540;

    enemy->body.width = 50;
    enemy->body.height = 60;

    enemy->hp = 30;
    enemy->maxHp = 30;

    enemy->alive = true;

    enemy->respawnTimer = 0;
}

void UpdateEnemy(Enemy *enemy)
{
    if (!enemy->alive)
    {
        enemy->respawnTimer -= GetFrameTime();

        if (enemy->respawnTimer <= 0)
        {
            enemy->alive = true;
            enemy->hp = enemy->maxHp;
        }
    }
}

void DrawEnemy(Enemy enemy)
{
    if (!enemy.alive)
        return;

    DrawRectangleRec(enemy.body, RED);
}