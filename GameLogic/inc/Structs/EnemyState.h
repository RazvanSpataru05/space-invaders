#pragma once

#include "Enums/EnemyType.h"

#include "Structs/Vector2.h"

#include <cstdint>

namespace GameLogic
{
    struct EnemyState
    {
        Vector2 position{};
        uint8_t health{};
        uint8_t maxHealth{};
        EnemyType type{};
        bool isAlive{};
    };
}