#pragma once

#include "Vector2.h"

namespace GameLogic
{
    struct PlayerState
    {
        Vector2 position{};
        int health{};
        int maxHealth{};
        bool isAlive{};
    };
}