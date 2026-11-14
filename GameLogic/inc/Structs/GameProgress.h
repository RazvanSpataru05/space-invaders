#pragma once

#include "Enums/LevelID.h"

#include <cstdint>

namespace GameLogic
{
    struct GameProgress
    {
        uint32_t score{};
        float elapsedTime{};
        LevelID currentLevel{};

        void Reset(LevelID level)
        {
            currentLevel = level;
            score = 0u;
            elapsedTime = 0.0f;
        }
    };
}