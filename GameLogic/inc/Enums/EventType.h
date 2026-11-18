#pragma once

#include <cstdint>

namespace GameLogic
{
    enum class EventType : uint8_t
    {
        LEVEL_COMPLETED,
        GAME_OVER,
        BOSS_SPAWNED
    };
}