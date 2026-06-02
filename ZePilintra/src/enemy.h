#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

#define MAX_ENEMIES 5

typedef struct
{
    Rectangle body;

    int hp;
    int maxHp;

    bool alive;

    float respawnTimer;

    float speed;

    int direction;

} Enemy;

void InitEnemy(Enemy *enemy, float x, float y);

void UpdateEnemy(
    Enemy *enemy,
    Vector2 playerPos
);

void DrawEnemy(Enemy enemy);

#endif