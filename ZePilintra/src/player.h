#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct
{
    Rectangle body;
    float speed;
    float jumpForce;
    float velocityY;
    bool isGrounded;
} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player player);

#endif