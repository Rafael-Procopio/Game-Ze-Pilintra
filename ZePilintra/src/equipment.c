#include "equipment.h"
#include "player.h"
#include "inventory.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define EQUIPMENT_NOTIFICATION_LENGTH 128
#define EQUIPMENT_NOTIFICATION_DURATION 2.5f

static const EquipmentItem equipmentDefinitions[] =
{
    { "Iron Sword", ITEM_WEAPON, 5, 0, 0 },
    { "Steel Sword", ITEM_WEAPON, 10, 0, 0 },
    { "Leather Armor", ITEM_ARMOR, 0, 3, 10 },
    { "Lucky Ring", ITEM_ACCESSORY, 2, 2, 0 }
};

static const int equipmentDefinitionCount = sizeof(equipmentDefinitions) / sizeof(equipmentDefinitions[0]);

static EquipmentItem emptyItem = { "", ITEM_CONSUMABLE, 0, 0, 0 };

typedef struct
{
    char text[EQUIPMENT_NOTIFICATION_LENGTH];
    float timer;
    bool active;
} EquipmentNotification;

static EquipmentNotification equipmentNotification = { 0 };

static bool GetEquipmentDefinition(const char *itemName, EquipmentItem *outItem)
{
    if (itemName == NULL || outItem == NULL)
        return false;

    for (int i = 0; i < equipmentDefinitionCount; i++)
    {
        if (strcmp(equipmentDefinitions[i].name, itemName) == 0)
        {
            *outItem = equipmentDefinitions[i];
            return true;
        }
    }

    return false;
}

void InitEquipment(Equipment *equipment)
{
    if (equipment == NULL) return;

    equipment->weapon = emptyItem;
    equipment->armor = emptyItem;
    equipment->accessory = emptyItem;

    equipment->weaponEquipped = false;
    equipment->armorEquipped = false;
    equipment->accessoryEquipped = false;
}

static void SetEquipmentNotification(const char *message)
{
    if (message == NULL) return;

    strncpy(equipmentNotification.text, message, EQUIPMENT_NOTIFICATION_LENGTH - 1);
    equipmentNotification.text[EQUIPMENT_NOTIFICATION_LENGTH - 1] = '\0';
    equipmentNotification.timer = EQUIPMENT_NOTIFICATION_DURATION;
    equipmentNotification.active = true;
}

static void UpdatePlayerHpAfterMaxChange(Player *player, int oldMaxHp, int newMaxHp)
{
    if (player == NULL) return;

    if (player->stats.hp >= oldMaxHp)
    {
        player->stats.hp = newMaxHp;
    }
    else if (player->stats.hp > newMaxHp)
    {
        player->stats.hp = newMaxHp;
    }
}

static EquipmentItem *GetEquipmentSlot(Equipment *equipment, ItemType type)
{
    if (equipment == NULL) return NULL;

    switch (type)
    {
        case ITEM_WEAPON:
            return &equipment->weapon;
        case ITEM_ARMOR:
            return &equipment->armor;
        case ITEM_ACCESSORY:
            return &equipment->accessory;
        default:
            return NULL;
    }
}

static bool *GetEquipmentEquippedFlag(Equipment *equipment, ItemType type)
{
    if (equipment == NULL) return NULL;

    switch (type)
    {
        case ITEM_WEAPON:
            return &equipment->weaponEquipped;
        case ITEM_ARMOR:
            return &equipment->armorEquipped;
        case ITEM_ACCESSORY:
            return &equipment->accessoryEquipped;
        default:
            return NULL;
    }
}

int GetEquipmentAttackBonus(const Equipment *equipment)
{
    if (equipment == NULL) return 0;

    int total = 0;
    if (equipment->weaponEquipped) total += equipment->weapon.attackBonus;
    if (equipment->armorEquipped) total += equipment->armor.attackBonus;
    if (equipment->accessoryEquipped) total += equipment->accessory.attackBonus;
    return total;
}

int GetEquipmentDefenseBonus(const Equipment *equipment)
{
    if (equipment == NULL) return 0;

    int total = 0;
    if (equipment->weaponEquipped) total += equipment->weapon.defenseBonus;
    if (equipment->armorEquipped) total += equipment->armor.defenseBonus;
    if (equipment->accessoryEquipped) total += equipment->accessory.defenseBonus;
    return total;
}

int GetEquipmentMaxHpBonus(const Equipment *equipment)
{
    if (equipment == NULL) return 0;

    int total = 0;
    if (equipment->weaponEquipped) total += equipment->weapon.maxHpBonus;
    if (equipment->armorEquipped) total += equipment->armor.maxHpBonus;
    if (equipment->accessoryEquipped) total += equipment->accessory.maxHpBonus;
    return total;
}

int GetPlayerAttackValue(const Player *player)
{
    if (player == NULL) return 0;
    return player->stats.attack + GetEquipmentAttackBonus(&player->equipment);
}

int GetPlayerDefenseValue(const Player *player)
{
    if (player == NULL) return 0;
    return player->stats.defense + GetEquipmentDefenseBonus(&player->equipment);
}

int GetPlayerMaxHpValue(const Player *player)
{
    if (player == NULL) return 0;
    return player->stats.maxHp + GetEquipmentMaxHpBonus(&player->equipment);
}

bool EquipItem(Player *player, const char *itemName)
{
    if (player == NULL || itemName == NULL) return false;

    EquipmentItem definition;
    if (!GetEquipmentDefinition(itemName, &definition))
        return false;

    int oldMaxHp = GetPlayerMaxHpValue(player);

    EquipmentItem *slot = GetEquipmentSlot(&player->equipment, definition.type);
    bool *slotEquipped = GetEquipmentEquippedFlag(&player->equipment, definition.type);
    if (slot == NULL || slotEquipped == NULL)
        return false;

    if (*slotEquipped && strcmp(slot->name, definition.name) == 0)
    {
        return true;
    }

    if (!RemoveItem(&player->inventory, itemName, 1))
        return false;

    EquipmentItem previousItem = emptyItem;
    bool hadPrevious = *slotEquipped;

    if (hadPrevious)
    {
        previousItem = *slot;
    }

    *slot = definition;
    *slotEquipped = true;

    if (hadPrevious)
    {
        if (!AddItem(&player->inventory, previousItem.name, 1))
        {
            *slot = previousItem;
            *slotEquipped = true;
            AddItem(&player->inventory, itemName, 1);
            return false;
        }
    }

    int newMaxHp = GetPlayerMaxHpValue(player);
    UpdatePlayerHpAfterMaxChange(player, oldMaxHp, newMaxHp);

    char message[EQUIPMENT_NOTIFICATION_LENGTH];
    snprintf(message, sizeof(message), "Equipped %s", definition.name);
    SetEquipmentNotification(message);

    return true;
}

bool UnequipWeapon(Player *player)
{
    if (player == NULL) return false;
    if (!player->equipment.weaponEquipped) return false;

    char name[50];
    strncpy(name, player->equipment.weapon.name, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    if (!AddItem(&player->inventory, name, 1))
        return false;

    int oldMaxHp = GetPlayerMaxHpValue(player);

    player->equipment.weapon = emptyItem;
    player->equipment.weaponEquipped = false;

    int newMaxHp = GetPlayerMaxHpValue(player);
    UpdatePlayerHpAfterMaxChange(player, oldMaxHp, newMaxHp);

    char message[EQUIPMENT_NOTIFICATION_LENGTH];
    snprintf(message, sizeof(message), "Unequipped %s", name);
    SetEquipmentNotification(message);

    return true;
}

bool UnequipArmor(Player *player)
{
    if (player == NULL) return false;
    if (!player->equipment.armorEquipped) return false;

    char name[50];
    strncpy(name, player->equipment.armor.name, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    if (!AddItem(&player->inventory, name, 1))
        return false;

    int oldMaxHp = GetPlayerMaxHpValue(player);

    player->equipment.armor = emptyItem;
    player->equipment.armorEquipped = false;

    int newMaxHp = GetPlayerMaxHpValue(player);
    UpdatePlayerHpAfterMaxChange(player, oldMaxHp, newMaxHp);

    char message[EQUIPMENT_NOTIFICATION_LENGTH];
    snprintf(message, sizeof(message), "Unequipped %s", name);
    SetEquipmentNotification(message);

    return true;
}

bool UnequipAccessory(Player *player)
{
    if (player == NULL) return false;
    if (!player->equipment.accessoryEquipped) return false;

    char name[50];
    strncpy(name, player->equipment.accessory.name, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    if (!AddItem(&player->inventory, name, 1))
        return false;

    int oldMaxHp = GetPlayerMaxHpValue(player);

    player->equipment.accessory = emptyItem;
    player->equipment.accessoryEquipped = false;

    int newMaxHp = GetPlayerMaxHpValue(player);
    UpdatePlayerHpAfterMaxChange(player, oldMaxHp, newMaxHp);

    char message[EQUIPMENT_NOTIFICATION_LENGTH];
    snprintf(message, sizeof(message), "Unequipped %s", name);
    SetEquipmentNotification(message);

    return true;
}

void UpdateEquipmentNotification(float delta)
{
    if (!equipmentNotification.active) return;

    equipmentNotification.timer -= delta;
    if (equipmentNotification.timer <= 0.0f)
    {
        equipmentNotification.active = false;
        equipmentNotification.text[0] = '\0';
    }
}

void DrawEquipmentNotification(void)
{
    if (!equipmentNotification.active) return;

    int width = 360;
    int height = 40;
    int x = 30;
    int y = 560;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.75f));
    DrawRectangleLines(x, y, width, height, WHITE);
    DrawText(equipmentNotification.text, x + 12, y + 10, 20, WHITE);
}

void DrawEquipmentPanel(const Equipment *equipment, int selectedSlot)
{
    if (equipment == NULL) return;

    int x = 420;
    int y = 20;
    int width = 380;
    int height = 220;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.65f));
    DrawRectangleLines(x, y, width, height, WHITE);
    DrawText("EQUIPMENT", x + 14, y + 14, 24, GOLD);

    bool weaponSelected = selectedSlot == 0;
    bool armorSelected = selectedSlot == 1;
    bool accessorySelected = selectedSlot == 2;

    DrawText("Weapon:", x + 14, y + 54, 20, WHITE);
    if (weaponSelected) DrawText(">", x + 14, y + 78, 20, BLUE);
    DrawText(equipment->weaponEquipped ? equipment->weapon.name : "None",
        x + 34,
        y + 78,
        20,
        equipment->weaponEquipped ? LIGHTGRAY : DARKGRAY);

    DrawText("Armor:", x + 14, y + 110, 20, WHITE);
    if (armorSelected) DrawText(">", x + 14, y + 134, 20, BLUE);
    DrawText(equipment->armorEquipped ? equipment->armor.name : "None",
        x + 34,
        y + 134,
        20,
        equipment->armorEquipped ? LIGHTGRAY : DARKGRAY);

    DrawText("Accessory:", x + 14, y + 166, 20, WHITE);
    if (accessorySelected) DrawText(">", x + 14, y + 190, 20, BLUE);
    DrawText(equipment->accessoryEquipped ? equipment->accessory.name : "None",
        x + 34,
        y + 190,
        20,
        equipment->accessoryEquipped ? LIGHTGRAY : DARKGRAY);

    DrawText("[UP/DOWN] Select Slot  [U] Unequip", x + 14, y + 204, 18, DARKGRAY);
}

void DrawEquipmentBonusPanel(const Equipment *equipment)
{
    if (equipment == NULL) return;

    int x = 820;
    int y = 20;
    int width = 340;
    int height = 160;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.65f));
    DrawRectangleLines(x, y, width, height, WHITE);
    DrawText("Equipment Bonuses", x + 14, y + 14, 24, GOLD);

    DrawText(TextFormat("Attack +%i", GetEquipmentAttackBonus(equipment)), x + 14, y + 52, 20, WHITE);
    DrawText(TextFormat("Defense +%i", GetEquipmentDefenseBonus(equipment)), x + 14, y + 82, 20, WHITE);
    DrawText(TextFormat("HP +%i", GetEquipmentMaxHpBonus(equipment)), x + 14, y + 112, 20, WHITE);
}

void DrawCharacterPanel(const Player *player)
{
    if (player == NULL) return;

    int x = 820;
    int y = 200;
    int width = 340;
    int height = 260;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.65f));
    DrawRectangleLines(x, y, width, height, WHITE);
    DrawText("Character Stats", x + 14, y + 14, 24, GOLD);

    DrawText(TextFormat("Level: %i", player->stats.level), x + 14, y + 48, 18, WHITE);

    DrawText(TextFormat("Base Attack: %i", player->stats.attack), x + 14, y + 78, 18, WHITE);
    DrawText(TextFormat("Equipment Attack: %i", GetEquipmentAttackBonus(&player->equipment)), x + 14, y + 102, 18, WHITE);
    DrawText(TextFormat("Final Attack: %i", GetPlayerAttackValue(player)), x + 14, y + 126, 18, WHITE);

    DrawText(TextFormat("Base Defense: %i", player->stats.defense), x + 14, y + 156, 18, WHITE);
    DrawText(TextFormat("Equipment Defense: %i", GetEquipmentDefenseBonus(&player->equipment)), x + 14, y + 180, 18, WHITE);
    DrawText(TextFormat("Final Defense: %i", GetPlayerDefenseValue(player)), x + 14, y + 204, 18, WHITE);

    DrawText(TextFormat("HP: %i/%i", player->stats.hp, GetPlayerMaxHpValue(player)), x + 14, y + 232, 18, WHITE);
}
