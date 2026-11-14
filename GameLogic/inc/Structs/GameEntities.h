#pragma once

#include "Structs/PlayerState.h"
#include "Structs/EnemyState.h"
#include "Structs/ProjectileState.h"
#include "Structs/PowerUpState.h"

#include <vector>

namespace GameLogic
{
    struct GameEntities
    {
        PlayerState player;
        std::vector<EnemyState> enemies;
        std::vector<ProjectileState> projectiles;
        std::vector<PowerUpState> powerUps;


        void Reset()
        {
            player.Reset();
            enemies.clear();
            projectiles.clear();
            powerUps.clear();
        }
    };
}
