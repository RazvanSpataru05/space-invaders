#pragma once

#include "Enums/EnemyType.h"

#include "Structs/Vector2.h"

#include <cstdint>

namespace GameLogic
{
    struct EnemyState
    {
        uint32_t id{};
        Vector2 position{};
        int health{};
        int maxHealth{};
        EnemyType type{};
        bool isAlive{};
    };
}