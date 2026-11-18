#pragma once

#include <cstdint>

namespace GameLogic
{
    enum class EnemyType : uint8_t
    {
        NORMAL,
        ARMORED,
        SHOOTER,
        BOSS
    };
}