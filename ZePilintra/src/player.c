#include "player.h"

#define GRAVITY 0.5f

void InitPlayer(Player *player)
{
    player->body.x = 100;
    player->body.y = 500;
    player->body.width = 50;
    player->body.height = 80;

    player->speed = 5.0f;
    player->jumpForce = -12.0f;
    player->velocityY = 0.0f;

    player->isGrounded = true;
}

void UpdatePlayer(Player *player)
{
    if (IsKeyDown(KEY_A))
    {
        player->body.x -= player->speed;
    }

    if (IsKeyDown(KEY_D))
    {
        player->body.x += player->speed;
    }

    if (IsKeyPressed(KEY_SPACE) && player->isGrounded)
    {
        player->velocityY = player->jumpForce;
        player->isGrounded = false;
    }

    player->velocityY += GRAVITY;
    player->body.y += player->velocityY;

    float groundY = 600;

    if (player->body.y + player->body.height >= groundY)
    {
        player->body.y = groundY - player->body.height;
        player->velocityY = 0;
        player->isGrounded = true;
    }
}

void DrawPlayer(Player player)
{
    DrawRectangleRec(player.body, BLUE);
}