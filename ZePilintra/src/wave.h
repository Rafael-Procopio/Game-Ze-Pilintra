#ifndef WAVE_H
#define WAVE_H

#include <stdbool.h>
#include "enemy.h"
#include "player.h"

#define MAX_WAVE_QUEUE 32

typedef struct
{
    int currentWave;
    int totalToSpawn;    // total enemies in this wave
    int spawnedCount;    // how many have been spawned
    int killedCount;     // how many have been killed
    bool waveCompleted;
    bool isBossWave;

    // Timers
    float announcementTimer; // wave start announcement
    float completionTimer;   // wait before starting next wave

    // Internal queue of types to spawn
    EnemyType spawnQueue[MAX_WAVE_QUEUE];
    int spawnQueueCount;

    // Notification text
    char notification[128];
    float notificationTimer;

} WaveSystem;

void InitWaveSystem(WaveSystem *ws);
void StartWave(WaveSystem *ws, int waveNumber, Enemy enemies[]);
void UpdateWaveSystem(WaveSystem *ws, float delta, Enemy enemies[], Player *player);
void DrawWaveHUD(const WaveSystem *ws);
void DrawWaveAnnouncement(const WaveSystem *ws);
void Wave_NotifyEnemyDefeated(WaveSystem *ws, EnemyType type, Player *player);
void Wave_SkipCurrent(WaveSystem *ws);

#endif // WAVE_H
