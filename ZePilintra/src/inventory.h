#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdbool.h>

#define MAX_INVENTORY_SLOTS 20
#define ITEM_NAME_LENGTH 50

typedef struct
{
    char name[ITEM_NAME_LENGTH];
    int quantity;
} Item;

typedef struct
{
    Item slots[MAX_INVENTORY_SLOTS];
    int itemCount;
} Inventory;

void InitInventory(Inventory *inventory);
bool AddItem(Inventory *inventory, const char *name, int quantity);
bool RemoveItem(Inventory *inventory, const char *name, int quantity);
void DrawInventory(Inventory inventory, int selectedIndex);

#endif // INVENTORY_H
