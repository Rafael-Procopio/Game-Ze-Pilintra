#ifndef DROPS_H
#define DROPS_H

#include "inventory.h"
#include "enemy.h"

#define DROP_NOTIFICATION_TEXT_LENGTH 256

typedef struct
{
    char text[DROP_NOTIFICATION_TEXT_LENGTH];
    float timer;
    bool active;
} DropNotification;

typedef struct
{
    char itemName[50];
    int chance;
    int minQuantity;
    int maxQuantity;
} DropEntry;

void InitDropNotification(void);
void GenerateDropsByEnemyType(EnemyType enemyType, Inventory *inventory);
void UpdateDropNotification(float delta);
void DrawDropNotification(void);
void DrawDropDebugPanel(void);
Color GetItemRarityColor(const char *itemName);

#endif // DROPS_H
