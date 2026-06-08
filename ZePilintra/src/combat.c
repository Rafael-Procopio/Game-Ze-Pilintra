#include "combat.h"

Rectangle CreateAttackHitbox(Rectangle playerBody, int facingRight)
{
    Rectangle hitbox;

    hitbox.width = 60;
    hitbox.height = 100;

    // Vertically centered on the player
    hitbox.y = playerBody.y + (playerBody.height - hitbox.height) / 2;

    // Horizontally positioned in front of the player
    if (facingRight)
    {
        // Facing right: hitbox extends to the right
        hitbox.x = playerBody.x + playerBody.width;
    }
    else
    {
        // Facing left: hitbox extends to the left
        hitbox.x = playerBody.x - hitbox.width;
    }

    return hitbox;
}
