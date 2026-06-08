#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "stats.h"
#include "inventory.h"
#include "equipment.h"

typedef enum
{
    PLAYER_IDLE,
    PLAYER_RUN,
    PLAYER_ATTACK
} PlayerAnimationState;

typedef struct Player
{
    Rectangle body;

    float speed;
    float jumpForce;
    float velocityY;

    bool isGrounded;

    int facingRight;

    Texture2D texture;

    Rectangle frameRec;

    int currentFrame;
    int frameCounter;

    int animationState;

    float attackTimer;

    Stats stats;
    Inventory inventory;
    Equipment equipment;

} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player player);
void UnloadPlayer(Player *player);

// Consumable / heal notifications
void SetHealNotification(const char *message, int healAmount);
void SpawnHealEffect(Player *player, int amount);
void UpdateHealEffects(float delta);
void DrawHealEffects(void);

#endif