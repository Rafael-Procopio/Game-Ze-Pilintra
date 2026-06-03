#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "stats.h"

typedef struct
{
    Rectangle body;

    float speed;
    float jumpForce;
    float velocityY;

    bool isGrounded;

    int facingRight;

    Stats stats;

} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player player);

#endif