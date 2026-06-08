#ifndef MAP_H
#define MAP_H

#include "raylib.h"

#define PLATFORM_COUNT 5

extern Rectangle platforms[PLATFORM_COUNT];

void InitMap(void);
void DrawMap(void);

#endif