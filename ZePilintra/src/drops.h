#ifndef DROPS_H
#define DROPS_H

#include "inventory.h"

#define DROP_NOTIFICATION_TEXT_LENGTH 100

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
} Drop;

void InitDropNotification(void);
void GenerateEnemyDrops(Inventory *inventory);
void UpdateDropNotification(float delta);
void DrawDropNotification(void);

#endif // DROPS_H
