#include "player.h"
#include "stats.h"
#include "map.h"
#include "inventory.h"
#include <stdio.h>

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

    player->facingRight = 1;

    // Load player texture
    // Try to load the texture, but fallback gracefully if it doesn't exist
    Image checkImage = {0};
    if (FileExists("assets/sprites/player.png"))
    {
        player->texture = LoadTexture("assets/sprites/player.png");
    }
    else
    {
        // Create a placeholder texture (1x1 white pixel) if file doesn't exist
        Image placeholderImage = GenImageColor(1, 1, WHITE);
        player->texture = LoadTextureFromImage(placeholderImage);
        UnloadImage(placeholderImage);
    }

    // Initialize animation frame rectangle
    // Assuming 64x64 frame size
    player->frameRec.width = 64;
    player->frameRec.height = 64;
    player->frameRec.x = 0;
    player->frameRec.y = 0;

    // Initialize animation variables
    player->currentFrame = 0;
    player->frameCounter = 0;

    // Initialize animation state
    player->animationState = PLAYER_IDLE;

    // Attack animation timer
    player->attackTimer = 0.0f;

    InitStats(&player->stats);
    InitInventory(&player->inventory);
}

// Helper function to update animation frames
static void UpdateAnimation(Player *player)
{
    int framesPerState = 0;
    int maxFrames = 0;

    // Determine frame count based on animation state
    if (player->animationState == PLAYER_IDLE)
    {
        maxFrames = 4;  // IDLE has 4 frames
    }
    else if (player->animationState == PLAYER_RUN)
    {
        maxFrames = 6;  // RUN has 6 frames
    }
    else if (player->animationState == PLAYER_ATTACK)
    {
        maxFrames = 4;  // ATTACK has 4 frames
    }

    // Update frame counter
    player->frameCounter++;

    // Change frame every 6 counter ticks
    if (player->frameCounter >= 6)
    {
        player->frameCounter = 0;
        player->currentFrame++;

        // Loop animation
        if (player->currentFrame >= maxFrames)
        {
            player->currentFrame = 0;

            // If attack animation finished, return to IDLE
            if (player->animationState == PLAYER_ATTACK)
            {
                player->animationState = PLAYER_IDLE;
                player->attackTimer = 0.0f;
            }
        }
    }

    // Update frame rectangle X position
    player->frameRec.x = (float)player->currentFrame * 64;
}

void UpdatePlayer(Player *player)
{
    // Movimento horizontal
    bool isMoving = false;

    if (IsKeyDown(KEY_A))
    {
        player->body.x -= player->speed;
        player->facingRight = 0;
        isMoving = true;
    }

    if (IsKeyDown(KEY_D))
    {
        player->body.x += player->speed;
        player->facingRight = 1;
        isMoving = true;
    }

    // Update animation state based on movement
    if (player->animationState != PLAYER_ATTACK)
    {
        if (isMoving)
        {
            player->animationState = PLAYER_RUN;
        }
        else
        {
            player->animationState = PLAYER_IDLE;
        }
    }

    // Update attack timer
    if (player->attackTimer > 0)
    {
        player->attackTimer -= GetFrameTime();
    }

    // Pulo
    if (IsKeyPressed(KEY_SPACE) && player->isGrounded)
    {
        player->velocityY = player->jumpForce;
        player->isGrounded = false;
    }

    // Gravidade
    player->velocityY += GRAVITY;
    player->body.y += player->velocityY;

    player->isGrounded = false;

    // Colisão com o chão principal
    float groundY = 600;

    if (player->body.y + player->body.height >= groundY)
    {
        player->body.y = groundY - player->body.height;
        player->velocityY = 0;
        player->isGrounded = true;
    }

    // Colisão com plataformas
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        Rectangle platform = platforms[i];

        bool touchingTop =
            player->velocityY >= 0 &&
            player->body.y + player->body.height >= platform.y &&
            player->body.y + player->body.height <= platform.y + 20 &&
            player->body.x + player->body.width > platform.x &&
            player->body.x < platform.x + platform.width;

        if (touchingTop)
        {
            player->body.y = platform.y - player->body.height;
            player->velocityY = 0;
            player->isGrounded = true;
        }
    }

    // Update animation frames
    UpdateAnimation(player);
}

void DrawPlayer(Player player)
{
    // Draw a colored rectangle for the player based on animation state/frame
    Color drawColor = BLUE;

    if (player.animationState == PLAYER_IDLE)
    {
        Color idleColors[4] = {BLUE, SKYBLUE, DARKBLUE, VIOLET};
        drawColor = idleColors[player.currentFrame % 4];
    }
    else if (player.animationState == PLAYER_RUN)
    {
        Color runColors[6] = {GREEN, LIME, DARKGREEN, YELLOW, GOLD, ORANGE};
        drawColor = runColors[player.currentFrame % 6];
    }
    else if (player.animationState == PLAYER_ATTACK)
    {
        Color attackColors[4] = {RED, MAROON, PINK, MAGENTA};
        drawColor = attackColors[player.currentFrame % 4];
    }

    DrawRectangleRec(player.body, drawColor);

    // Debug information: show animation state and current frame
    const char *stateName = "UNKNOWN";
    if (player.animationState == PLAYER_IDLE) stateName = "IDLE";
    else if (player.animationState == PLAYER_RUN) stateName = "RUN";
    else if (player.animationState == PLAYER_ATTACK) stateName = "ATTACK";

    char bufState[64];
    char bufFrame[64];
    sprintf(bufState, "Animation: %s", stateName);
    sprintf(bufFrame, "Frame: %d", player.currentFrame);

    DrawText(bufState, 10, 10, 20, WHITE);
    DrawText(bufFrame, 10, 34, 20, WHITE);
}

void UnloadPlayer(Player *player)
{
    if (player->texture.id > 0)
    {
        UnloadTexture(player->texture);
    }
}