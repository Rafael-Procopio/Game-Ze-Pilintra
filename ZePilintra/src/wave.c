#include "wave.h"
#include "enemy.h"
#include "player.h"
#include "stats.h"
#include <string.h>
#include <stdio.h>

// spawn positions for up to MAX_ENEMIES
static const float spawnX[MAX_ENEMIES] = { 800, 1200, 1700, 2200, 2800 };
static const float spawnY = 540;

static void SetNotification(WaveSystem *ws, const char *text, float duration)
{
    if (!ws) return;
    strncpy(ws->notification, text, sizeof(ws->notification) - 1);
    ws->notification[sizeof(ws->notification) - 1] = '\0';
    ws->notificationTimer = duration;
}

void InitWaveSystem(WaveSystem *ws)
{
    if (!ws) return;
    memset(ws, 0, sizeof(*ws));
    ws->currentWave = 0;
    ws->announcementTimer = 0;
    ws->completionTimer = 0;
    ws->notificationTimer = 0;
}

static void FillSpawnQueueForWave(WaveSystem *ws, int wave)
{
    ws->spawnQueueCount = 0;
    ws->isBossWave = false;

    switch (wave)
    {
        case 1:
            // 5 Goblins
            for (int i = 0; i < 5; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_GOBLIN;
            break;
        case 2:
            // 3 Goblins, 3 Bandits
            for (int i = 0; i < 3; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_GOBLIN;
            for (int i = 0; i < 3; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_BANDIT;
            break;
        case 3:
            // 5 Bandits, 3 Skeletons
            for (int i = 0; i < 5; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_BANDIT;
            for (int i = 0; i < 3; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_SKELETON;
            break;
        case 4:
            // 5 Skeletons, 2 Elites
            for (int i = 0; i < 5; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_SKELETON;
            for (int i = 0; i < 2; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_ELITE;
            break;
        case 5:
            // 5 Elites
            for (int i = 0; i < 5; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_ELITE;
            break;
        default:
            // For waves beyond 5, scale up mixture: add elites progressively
            for (int i = 0; i < 5; i++) ws->spawnQueue[ws->spawnQueueCount++] = ENEMY_ELITE;
            break;
    }

    ws->totalToSpawn = ws->spawnQueueCount;
    ws->spawnedCount = 0;
    ws->killedCount = 0;
    ws->waveCompleted = false;
}

static void TrySpawnFromQueue(WaveSystem *ws, Enemy enemies[])
{
    if (!ws) return;
    if (ws->spawnedCount >= ws->totalToSpawn) return;

    // find empty slots and spawn until either queue exhausted or no free slots
    for (int i = 0; i < MAX_ENEMIES && ws->spawnedCount < ws->totalToSpawn; i++)
    {
        if (enemies[i].alive) continue;

        EnemyType t = ws->spawnQueue[ws->spawnedCount];
        InitEnemy(&enemies[i], spawnX[i], spawnY, t);

        // Apply scaling: HP +10% per wave (relative to wave-1), damage +5% per wave
        float hpFactor = 1.0f + 0.10f * (ws->currentWave - 1);
        float dmgFactor = 1.0f + 0.05f * (ws->currentWave - 1);

        enemies[i].maxHp = (int)(enemies[i].maxHp * hpFactor + 0.5f);
        enemies[i].hp = enemies[i].maxHp;
        enemies[i].damage = (int)(enemies[i].damage * dmgFactor + 0.5f);

        ws->spawnedCount++;
    }
}

void StartWave(WaveSystem *ws, int waveNumber, Enemy enemies[])
{
    if (!ws) return;
    ws->currentWave = waveNumber;
    FillSpawnQueueForWave(ws, waveNumber);
    ws->announcementTimer = 3.0f;
    ws->completionTimer = 0.0f;
    ws->notificationTimer = 0.0f;

    // Clear any existing enemies
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].alive = false;
        enemies[i].respawnTimer = 0.0f;
    }

    // Spawn initial batch
    TrySpawnFromQueue(ws, enemies);

    char ann[64];
    snprintf(ann, sizeof(ann), "WAVE %i", ws->currentWave);
    SetNotification(ws, ann, 3.0f);
}

static int GetWaveRewardXP(int wave)
{
    switch (wave)
    {
        case 1: return 50;
        case 2: return 100;
        case 3: return 200;
        case 4: return 300;
        case 5: return 500;
        default: return 500 + (wave - 5) * 200;
    }
}

void Wave_NotifyEnemyDefeated(WaveSystem *ws, EnemyType type, Player *player)
{
    if (!ws) return;
    ws->killedCount++;

    // Attempt to spawn next from queue into free slot (spawnedCount still < totalToSpawn)
    // The caller should have already marked that enemy slot dead; we'll rely on TrySpawnFromQueue next update

    if (ws->killedCount >= ws->totalToSpawn)
    {
        ws->waveCompleted = true;
        ws->completionTimer = 3.0f; // wait before next wave

        int reward = GetWaveRewardXP(ws->currentWave);
        if (player)
        {
            AddXP(&player->stats, reward);
        }

        char msg[128];
        snprintf(msg, sizeof(msg), "Wave Complete! +%i XP", reward);
        SetNotification(ws, msg, 3.0f);
    }
}

void Wave_SkipCurrent(WaveSystem *ws)
{
    if (!ws) return;
    if (ws->waveCompleted) return;
    ws->killedCount = ws->totalToSpawn;
    ws->waveCompleted = true;
    ws->completionTimer = 0.5f;
    SetNotification(ws, "Wave Skipped", 2.0f);
}

void UpdateWaveSystem(WaveSystem *ws, float delta, Enemy enemies[], Player *player)
{
    if (!ws) return;

    // Update announcement and notification timers
    if (ws->announcementTimer > 0.0f)
    {
        ws->announcementTimer -= delta;
    }

    if (ws->notificationTimer > 0.0f)
    {
        ws->notificationTimer -= delta;
        if (ws->notificationTimer <= 0.0f)
        {
            ws->notification[0] = '\0';
        }
    }

    // If wave completed, count down and start next wave when timer expires
    if (ws->waveCompleted)
    {
        ws->completionTimer -= delta;
        if (ws->completionTimer <= 0.0f)
        {
            // start next wave
            StartWave(ws, ws->currentWave + 1, enemies);
        }
        return;
    }

    // Try to spawn into empty slots
    TrySpawnFromQueue(ws, enemies);

}

void DrawWaveHUD(const WaveSystem *ws)
{
    if (!ws) return;
    int x = 20;
    int y = 320;
    DrawText(TextFormat("Wave: %i", ws->currentWave), x, y, 22, GOLD);
    DrawText(TextFormat("Enemies Remaining: %i", (ws->totalToSpawn - ws->killedCount)), x, y + 30, 20, RED);

    if (ws->notification[0] != '\0')
    {
        DrawRectangle(300, 20, 360, 40, Fade(BLACK, 0.65f));
        DrawRectangleLines(300, 20, 360, 40, WHITE);
        DrawText(ws->notification, 312, 30, 20, WHITE);
    }
}

void DrawWaveAnnouncement(const WaveSystem *ws)
{
    if (!ws) return;
    if (ws->announcementTimer <= 0.0f) return;

    float t = ws->announcementTimer;
    float alpha = t / 3.0f;
    Color c = Fade(WHITE, alpha);
    const char *txt = TextFormat("WAVE %i", ws->currentWave);
    int fontSize = 80;
    int w = MeasureText(txt, fontSize);
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    DrawText(txt, (screenW - w) / 2, screenH / 2 - 40, fontSize, c);
}
