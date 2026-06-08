#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <stdbool.h>
#include "items.h"

typedef struct
{
    char name[50];
    ItemType type;
    int attackBonus;
    int defenseBonus;
    int maxHpBonus;
} EquipmentItem;

typedef struct
{
    EquipmentItem weapon;
    EquipmentItem armor;
    EquipmentItem accessory;

    bool weaponEquipped;
    bool armorEquipped;
    bool accessoryEquipped;
} Equipment;

typedef struct Player Player;

void InitEquipment(Equipment *equipment);
bool EquipItem(Player *player, const char *itemName);
bool UnequipWeapon(Player *player);
bool UnequipArmor(Player *player);
bool UnequipAccessory(Player *player);

int GetEquipmentAttackBonus(const Equipment *equipment);
int GetEquipmentDefenseBonus(const Equipment *equipment);
int GetEquipmentMaxHpBonus(const Equipment *equipment);

int GetPlayerAttackValue(const Player *player);
int GetPlayerDefenseValue(const Player *player);
int GetPlayerMaxHpValue(const Player *player);

void UpdateEquipmentNotification(float delta);
void DrawEquipmentNotification(void);
void DrawEquipmentPanel(const Equipment *equipment, int selectedSlot);
void DrawEquipmentBonusPanel(const Equipment *equipment);
void DrawCharacterPanel(const struct Player *player);

#endif // EQUIPMENT_H
