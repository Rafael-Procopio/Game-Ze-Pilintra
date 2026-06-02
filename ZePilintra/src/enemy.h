#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

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

void InitEnemy(Enemy *enemy);
void UpdateEnemy(Enemy *enemy, Vector2 playerPos);
void DrawEnemy(Enemy enemy);

#endif