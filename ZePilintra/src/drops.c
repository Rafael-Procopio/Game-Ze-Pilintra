#include "drops.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define NOTIFICATION_DURATION 3.0f

static DropNotification dropNotification = { 0 };
static char lastEnemyKilled[50] = "None";
static char lastDropsText[DROP_NOTIFICATION_TEXT_LENGTH] = "None";

static const DropEntry goblinDrops[] =
{
    { "Gold Coin", 90, 5, 20 },
    { "Potion", 50, 1, 3 }
};

static const DropEntry banditDrops[] =
{
    { "Gold Coin", 100, 5, 20 },
    { "Potion", 40, 1, 3 },
    { "Iron Sword", 15, 1, 1 }
};

static const DropEntry skeletonDrops[] =
{
    { "Gold Coin", 100, 5, 20 },
    { "Bone Fragment", 80, 1, 3 },
    { "Leather Armor", 10, 1, 1 }
};

static const DropEntry eliteDrops[] =
{
    { "Gold Coin", 100, 10, 25 },
    { "Greater Potion", 60, 1, 2 },
    { "Steel Sword", 25, 1, 1 },
    { "Lucky Ring", 5, 1, 1 }
};

static const DropEntry *GetDropsByEnemyType(EnemyType enemyType, int *outCount)
{
    switch (enemyType)
    {
        case ENEMY_GOBLIN:
            *outCount = sizeof(goblinDrops) / sizeof(goblinDrops[0]);
            return goblinDrops;
        case ENEMY_BANDIT:
            *outCount = sizeof(banditDrops) / sizeof(banditDrops[0]);
            return banditDrops;
        case ENEMY_SKELETON:
            *outCount = sizeof(skeletonDrops) / sizeof(skeletonDrops[0]);
            return skeletonDrops;
        case ENEMY_ELITE:
            *outCount = sizeof(eliteDrops) / sizeof(eliteDrops[0]);
            return eliteDrops;
        default:
            *outCount = 0;
            return NULL;
    }
}

Color GetItemRarityColor(const char *itemName)
{
    if (itemName == NULL) return WHITE;

    if (strcmp(itemName, "Potion") == 0) return GREEN;
    if (strcmp(itemName, "Gold Coin") == 0) return WHITE;
    if (strcmp(itemName, "Iron Sword") == 0) return BLUE;
    if (strcmp(itemName, "Bone Fragment") == 0) return BLUE;
    if (strcmp(itemName, "Leather Armor") == 0) return BLUE;
    if (strcmp(itemName, "Greater Potion") == 0) return PURPLE;
    if (strcmp(itemName, "Steel Sword") == 0) return PURPLE;
    if (strcmp(itemName, "Lucky Ring") == 0) return GOLD;

    return WHITE;
}

static void SetDropNotification(const char *text)
{
    if (text == NULL) return;

    strncpy(dropNotification.text, text, DROP_NOTIFICATION_TEXT_LENGTH - 1);
    dropNotification.text[DROP_NOTIFICATION_TEXT_LENGTH - 1] = '\0';
    dropNotification.timer = NOTIFICATION_DURATION;
    dropNotification.active = true;
}

static void SetLastEnemyKilled(EnemyType enemyType)
{
    const char *enemyName = "Unknown";

    switch (enemyType)
    {
        case ENEMY_GOBLIN: enemyName = "Goblin"; break;
        case ENEMY_BANDIT: enemyName = "Bandit"; break;
        case ENEMY_SKELETON: enemyName = "Skeleton"; break;
        case ENEMY_ELITE: enemyName = "Elite"; break;
    }

    strncpy(lastEnemyKilled, enemyName, sizeof(lastEnemyKilled) - 1);
    lastEnemyKilled[sizeof(lastEnemyKilled) - 1] = '\0';
}

static void SetLastDrops(const char *text)
{
    if (text == NULL || text[0] == '\0')
    {
        strncpy(lastDropsText, "None", sizeof(lastDropsText) - 1);
        lastDropsText[sizeof(lastDropsText) - 1] = '\0';
        return;
    }

    strncpy(lastDropsText, text, sizeof(lastDropsText) - 1);
    lastDropsText[sizeof(lastDropsText) - 1] = '\0';
}

static const char *GetItemNameFromLine(const char *line)
{
    const char *separator = NULL;

    for (const char *p = line; *p != '\0'; p++)
    {
        if (*p == 'x' && p > line && p[-1] == ' ') {
            separator = p - 1;
        }
    }

    if (separator == NULL)
    {
        return line;
    }

    size_t len = separator - line;
    static char itemName[50];
    if (len >= sizeof(itemName)) len = sizeof(itemName) - 1;
    strncpy(itemName, line, len);
    itemName[len] = '\0';
    return itemName;
}

static void DrawMultilineText(const char *text, int x, int y, int fontSize, int spacing)
{
    if (text == NULL || text[0] == '\0') return;

    int lineY = y;
    const char *lineStart = text;

    while (*lineStart != '\0')
    {
        const char *lineEnd = lineStart;

        while (*lineEnd != '\0' && *lineEnd != '\n')
        {
            lineEnd++;
        }

        int len = (int)(lineEnd - lineStart);
        if (len > 0)
        {
            char line[DROP_NOTIFICATION_TEXT_LENGTH];
            strncpy(line, lineStart, len);
            line[len] = '\0';
            Color lineColor = WHITE;

            if (strcmp(line, "Obtained:") == 0 || strcmp(line, "None") == 0)
            {
                lineColor = WHITE;
            }
            else
            {
                lineColor = GetItemRarityColor(GetItemNameFromLine(line));
            }

            DrawText(line, x, lineY, fontSize, lineColor);
        }

        if (*lineEnd == '\0')
            break;

        lineStart = lineEnd + 1;
        lineY += fontSize + spacing;
    }
}

void InitDropNotification(void)
{
    dropNotification.text[0] = '\0';
    dropNotification.timer = 0.0f;
    dropNotification.active = false;
    strncpy(lastEnemyKilled, "None", sizeof(lastEnemyKilled) - 1);
    lastEnemyKilled[sizeof(lastEnemyKilled) - 1] = '\0';
    SetLastDrops("None");
}

void GenerateDropsByEnemyType(EnemyType enemyType, Inventory *inventory)
{
    if (inventory == NULL) return;

    SetLastEnemyKilled(enemyType);

    const DropEntry *drops = NULL;
    int dropCount = 0;
    drops = GetDropsByEnemyType(enemyType, &dropCount);

    char notificationText[DROP_NOTIFICATION_TEXT_LENGTH] = "Obtained:";
    char lastDropsBuffer[DROP_NOTIFICATION_TEXT_LENGTH] = "";
    bool hasDrop = false;

    for (int i = 0; i < dropCount; i++)
    {
        int roll = GetRandomValue(1, 100);
        if (roll <= drops[i].chance)
        {
            int quantity = GetRandomValue(drops[i].minQuantity, drops[i].maxQuantity);
            if (quantity <= 0)
                continue;

            AddItem(inventory, drops[i].itemName, quantity);

            char line[DROP_NOTIFICATION_TEXT_LENGTH];
            snprintf(line, DROP_NOTIFICATION_TEXT_LENGTH, "\n%s x%i", drops[i].itemName, quantity);
            strncat(notificationText, line, DROP_NOTIFICATION_TEXT_LENGTH - strlen(notificationText) - 1);

            if (hasDrop)
            {
                strncat(lastDropsBuffer, "\n", sizeof(lastDropsBuffer) - strlen(lastDropsBuffer) - 1);
            }
            strncat(lastDropsBuffer, line + 1, sizeof(lastDropsBuffer) - strlen(lastDropsBuffer) - 1);
            hasDrop = true;
        }
    }

    if (!hasDrop)
    {
        SetLastDrops("None");
    }
    else
    {
        SetLastDrops(lastDropsBuffer);
        SetDropNotification(notificationText);
    }
}

void UpdateDropNotification(float delta)
{
    if (!dropNotification.active) return;

    dropNotification.timer -= delta;
    if (dropNotification.timer <= 0.0f)
    {
        dropNotification.active = false;
        dropNotification.text[0] = '\0';
        dropNotification.timer = 0.0f;
    }
}

void DrawDropNotification(void)
{
    if (!dropNotification.active) return;

    int width = 360;
    int height = 100;
    int x = 30;
    int y = 620;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.75f));
    DrawRectangleLines(x, y, width, height, WHITE);

    DrawMultilineText(dropNotification.text, x + 16, y + 16, 20, 4);
}

void DrawDropDebugPanel(void)
{
    int width = 320;
    int height = 140;
    int x = 900;
    int y = 20;

    DrawRectangle(x, y, width, height, Fade(BLACK, 0.65f));
    DrawRectangleLines(x, y, width, height, WHITE);

    DrawText("Last Enemy Killed:", x + 12, y + 12, 20, WHITE);
    DrawText(lastEnemyKilled, x + 12, y + 38, 22, GOLD);

    DrawText("Last Drops:", x + 12, y + 70, 20, WHITE);
    DrawMultilineText(lastDropsText, x + 12, y + 96, 18, 2);
}
