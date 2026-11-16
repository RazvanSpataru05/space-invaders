#pragma once

#include <cstdint>

namespace GameLogic
{
    enum class GameEventType : uint8_t
    {
        LEVEL_COMPLETED,
        GAME_OVER,
    };
}