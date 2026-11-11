#pragma once

#include "Enums/LevelID.h"

#include <cstdint>

namespace GameLogic
{
    struct GameProgress
    {
        uint32_t score{};
        float elapsedTime{};
        LevelID currentLevel;
    };
}