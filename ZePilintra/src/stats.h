#ifndef STATS_H
#define STATS_H

typedef struct
{
    int level;

    int xp;
    int xpToNextLevel;

    int hp;
    int maxHp;

    int attack;
    int defense;

    int skillPoints;

} Stats;

void InitStats(Stats *stats);
void AddXP(Stats *stats, int amount);

void UpgradeAttack(Stats *stats);
void UpgradeDefense(Stats *stats);
void UpgradeMaxHp(Stats *stats);

#endif