#include "stats.h"

void InitStats(Stats *stats)
{
    stats->level = 1;

    stats->xp = 0;
    stats->xpToNextLevel = 100;

    stats->hp = 100;
    stats->maxHp = 100;

    stats->attack = 10;
    stats->defense = 5;

    stats->skillPoints = 0;
}

void AddXP(Stats *stats, int amount)
{
    stats->xp += amount;

    while (stats->xp >= stats->xpToNextLevel)
    {
        stats->xp -= stats->xpToNextLevel;

        stats->level++;

        stats->skillPoints++;

        stats->xpToNextLevel += 50;
    }
}