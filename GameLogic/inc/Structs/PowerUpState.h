#pragma once

#include "Enums/PowerUpType.h"

#include "Structs/Vector2.h"

namespace GameLogic
{
    struct PowerUpState
    {
        Vector2 position{};
        PowerUpType type{};
        bool isActive{};
        float remainingDuration{};
    };
}
