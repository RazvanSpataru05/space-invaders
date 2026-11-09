#pragma once

#include <cstdint>

namespace GameLogic
{
    enum class GameEventType : uint8_t
    {
        SCORE_CHANGED,
        PLAYER_HEALTH_CHANGED,
        PLAYER_DESTROYED,
        ENEMY_DESTROYED,
        POWER_UP_COLLECTED,
        LEVEL_COMPLETED,
        BOSS_SPAWNED,
        GAME_OVER,
        GAME_WON
    };
}