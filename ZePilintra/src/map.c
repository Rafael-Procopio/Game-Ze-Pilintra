#include "map.h"

Rectangle platforms[PLATFORM_COUNT];

void InitMap(void)
{
    platforms[0] = (Rectangle){400, 500, 200, 30};
    platforms[1] = (Rectangle){900, 420, 200, 30};
    platforms[2] = (Rectangle){1500, 350, 250, 30};
    platforms[3] = (Rectangle){2200, 450, 200, 30};
    platforms[4] = (Rectangle){3000, 300, 300, 30};
}

void DrawMap(void)
{
    DrawRectangle(0, 600, 4000, 120, DARKGREEN);

    for(int i = 0; i < PLATFORM_COUNT; i++)
    {
        DrawRectangleRec(platforms[i], BROWN);
    }
}