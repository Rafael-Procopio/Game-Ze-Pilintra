#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

#define MAX_ENEMIES 5

typedef enum
{
    ENEMY_GOBLIN,
    ENEMY_BANDIT,
    ENEMY_SKELETON,
    ENEMY_ELITE
} EnemyType;

typedef struct
{
    Rectangle body;

    int hp;
    int maxHp;
    int damage;
    int xpReward;

    bool alive;

    float respawnTimer;

    float speed;

    int direction;

    EnemyType type;

} Enemy;

void InitEnemy(Enemy *enemy, float x, float y, EnemyType type);

void UpdateEnemy(
    Enemy *enemy,
    Vector2 playerPos
);

void DrawEnemy(Enemy enemy);

#endif