#pragma once

#include <cstdint>

namespace GameLogic
{
    enum class GameState : uint8_t
    {
        PLAYING,
        PAUSED,
        GAME_OVER,
        VICTORY
    };
}