#include "drops.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define DROP_TABLE_SIZE 3
#define NOTIFICATION_DURATION 3.0f

static DropNotification dropNotification = { 0 };

static const Drop dropTable[DROP_TABLE_SIZE] =
{
    { "Potion", 50, 1, 3 },
    { "Gold Coin", 80, 5, 20 },
    { "Iron Sword", 10, 1, 1 }
};

static void SetDropNotification(const char *text)
{
    if (text == NULL) return;

    strncpy(dropNotification.text, text, DROP_NOTIFICATION_TEXT_LENGTH - 1);
    dropNotification.text[DROP_NOTIFICATION_TEXT_LENGTH - 1] = '\0';
    dropNotification.timer = NOTIFICATION_DURATION;
    dropNotification.active = true;
}

void InitDropNotification(void)
{
    dropNotification.text[0] = '\0';
    dropNotification.timer = 0.0f;
    dropNotification.active = false;
}

void GenerateEnemyDrops(Inventory *inventory)
{
    if (inventory == NULL) return;

    char notificationText[DROP_NOTIFICATION_TEXT_LENGTH] = "Obtained:";
    bool hasDrop = false;

    for (int i = 0; i < DROP_TABLE_SIZE; i++)
    {
        int roll = GetRandomValue(1, 100);

        if (roll <= dropTable[i].chance)
        {
            int quantity = GetRandomValue(dropTable[i].minQuantity, dropTable[i].maxQuantity);

            if (quantity <= 0)
                continue;

            AddItem(inventory, dropTable[i].itemName, quantity);

            if (!hasDrop)
            {
                char line[DROP_NOTIFICATION_TEXT_LENGTH];
                snprintf(line, DROP_NOTIFICATION_TEXT_LENGTH, "\n%s x%i", dropTable[i].itemName, quantity);
                strncat(notificationText, line, DROP_NOTIFICATION_TEXT_LENGTH - strlen(notificationText) - 1);
                hasDrop = true;
            }
            else
            {
                char line[DROP_NOTIFICATION_TEXT_LENGTH];
                snprintf(line, DROP_NOTIFICATION_TEXT_LENGTH, "\n%s x%i", dropTable[i].itemName, quantity);
                strncat(notificationText, line, DROP_NOTIFICATION_TEXT_LENGTH - strlen(notificationText) - 1);
            }
        }
    }

    if (hasDrop)
    {
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

static void DrawMultilineText(const char *text, int x, int y, int fontSize, int spacing, Color color)
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
            DrawText(line, x, lineY, fontSize, color);
        }

        if (*lineEnd == '\0')
            break;

        lineStart = lineEnd + 1;
        lineY += fontSize + spacing;
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

    DrawMultilineText(dropNotification.text, x + 16, y + 16, 20, 4, RAYWHITE);
}
