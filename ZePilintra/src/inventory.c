#include "inventory.h"
#include "raylib.h"
#include "player.h"
#include <string.h>
#include <stdio.h>

void InitInventory(Inventory *inventory)
{
    if (inventory == NULL) return;

    inventory->itemCount = 0;

    for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        inventory->slots[i].name[0] = '\0';
        inventory->slots[i].quantity = 0;
    }
}

bool AddItem(Inventory *inventory, const char *name, int quantity)
{
    if (inventory == NULL || name == NULL || quantity <= 0) return false;

    for (int i = 0; i < inventory->itemCount; i++)
    {
        if (strcmp(inventory->slots[i].name, name) == 0)
        {
            inventory->slots[i].quantity += quantity;
            return true;
        }
    }

    if (inventory->itemCount >= MAX_INVENTORY_SLOTS)
    {
        return false;
    }

    Item *slot = &inventory->slots[inventory->itemCount];
    strncpy(slot->name, name, ITEM_NAME_LENGTH - 1);
    slot->name[ITEM_NAME_LENGTH - 1] = '\0';
    slot->quantity = quantity;
    inventory->itemCount++;

    return true;
}

bool RemoveItem(Inventory *inventory, const char *name, int quantity)
{
    if (inventory == NULL || name == NULL || quantity <= 0) return false;

    for (int i = 0; i < inventory->itemCount; i++)
    {
        if (strcmp(inventory->slots[i].name, name) == 0)
        {
            if (inventory->slots[i].quantity < quantity)
            {
                return false;
            }

            inventory->slots[i].quantity -= quantity;

            if (inventory->slots[i].quantity == 0)
            {
                for (int j = i; j < inventory->itemCount - 1; j++)
                {
                    inventory->slots[j] = inventory->slots[j + 1];
                }
                inventory->itemCount--;
                inventory->slots[inventory->itemCount].name[0] = '\0';
                inventory->slots[inventory->itemCount].quantity = 0;
            }

            return true;
        }
    }

    return false;
}

void DrawInventory(Inventory inventory, int selectedIndex)
{
    const int width = 720;
    const int height = 520;
    const int x = (GetScreenWidth() - width) / 2;
    const int y = (GetScreenHeight() - height) / 2;

    DrawRectangle(x - 4, y - 4, width + 8, height + 8, BLACK);
    DrawRectangle(x, y, width, height, Fade(RAYWHITE, 0.95f));
    DrawRectangleLines(x, y, width, height, GRAY);

    DrawText("Inventory", x + 24, y + 24, 40, BLACK);
    DrawText("[I] Close Inventory", x + 24, y + 70, 20, DARKGRAY);
    DrawText("[UP/DOWN] Select Item  [ENTER] Equip", x + 24, y + 100, 20, DARKGRAY);

    if (inventory.itemCount == 0)
    {
        DrawText("Empty", x + 24, y + 140, 30, DARKGRAY);
        return;
    }

    if (selectedIndex < 0) selectedIndex = 0;
    if (selectedIndex >= inventory.itemCount) selectedIndex = inventory.itemCount - 1;

    for (int i = 0; i < inventory.itemCount; i++)
    {
        int itemY = y + 140 + i * 30;
        bool isSelected = i == selectedIndex;
        int textX = x + 24;

        if (isSelected)
        {
            DrawText(">", x + 14, itemY, 24, BLUE);
            textX += 20;
        }

        DrawText(
            TextFormat("[%i] %s x%i", i + 1, inventory.slots[i].name, inventory.slots[i].quantity),
            textX,
            itemY,
            24,
            isSelected ? BLUE : BLACK
        );
    }
}

// Consumable definitions
typedef struct
{
    const char *name;
    int healAmount;
} ConsumableDef;

static const ConsumableDef consumables[] =
{
    { "Potion", 25 },
    { "Greater Potion", 75 }
};

static const int consumableCount = sizeof(consumables) / sizeof(consumables[0]);

static int GetConsumableHealAmount(const char *name)
{
    if (name == NULL) return 0;
    for (int i = 0; i < consumableCount; i++)
    {
        if (strcmp(consumables[i].name, name) == 0)
            return consumables[i].healAmount;
    }
    return 0;
}

bool UseItem(Inventory *inventory, struct Player *player, int slotIndex)
{
    if (inventory == NULL || player == NULL) return false;
    if (slotIndex < 0 || slotIndex >= inventory->itemCount) return false;

    Item *it = &inventory->slots[slotIndex];
    if (it->quantity <= 0) return false;

    int heal = GetConsumableHealAmount(it->name);
    if (heal <= 0) return false; // Not a consumable

    // Apply heal with clamp to player's max HP
    int oldHp = player->stats.hp;
    int maxHp = GetPlayerMaxHpValue(player);
    int amount = heal;
    if (oldHp >= maxHp) amount = 0;
    if (oldHp + amount > maxHp) amount = maxHp - oldHp;

    if (amount > 0)
    {
        player->stats.hp += amount;
        if (player->stats.hp > maxHp) player->stats.hp = maxHp;
    }

    // Reduce quantity and remove slot if needed
    RemoveItem(inventory, it->name, 1);

    // Show notifications and floating text
    char buf[128];
    snprintf(buf, sizeof(buf), "Used %s", it->name);
    SetHealNotification(buf, amount);
    SpawnHealEffect(player, amount);

    return true;
}
