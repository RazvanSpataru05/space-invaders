#pragma once

#include <cstdint>

namespace GameLogic
{
    enum class PowerUpType : uint8_t
    {
        EXTRA_LIFE,
        BONUS_SCORE,
        RAPID_FIRE,
        MULTI_SHOT
    };
}
