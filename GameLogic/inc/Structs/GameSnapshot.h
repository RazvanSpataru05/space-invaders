#pragma once

#include "Enums/LevelID.h"
#include "Enums/GameState.h"

#include "Structs/PlayerState.h"
#include "Structs/EnemyState.h"
#include "Structs/ProjectileState.h"
#include "Structs/PowerUpState.h"

#include <vector>
#include <cstdint>

namespace GameLogic
{
    struct GameSnapshot
    {
        PlayerState player{};
        std::vector<EnemyState> enemies;
        std::vector<ProjectileState> projectiles;
        std::vector<PowerUpState> powerUps;
        uint32_t score{};
        LevelID currentLevel{};
        GameState gameState{};
    };
}