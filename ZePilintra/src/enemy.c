#include "enemy.h"

void InitEnemy(
    Enemy *enemy,
    float x,
    float y
)
{
    enemy->body = (Rectangle)
    {
        x,
        y,
        50,
        60
    };

    enemy->hp = 30;
    enemy->maxHp = 30;

    enemy->alive = true;

    enemy->respawnTimer = 0;

    enemy->speed = 2.0f;

    enemy->direction = 1;
}

void UpdateEnemy(Enemy *enemy, Vector2 playerPos)
{
    if (!enemy->alive)
    {
        enemy->respawnTimer -= GetFrameTime();

        if (enemy->respawnTimer <= 0)
        {
            enemy->alive = true;
            enemy->hp = enemy->maxHp;
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

    DrawRectangleRec(enemy.body, RED);
}