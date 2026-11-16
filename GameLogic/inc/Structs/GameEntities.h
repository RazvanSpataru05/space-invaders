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
        std::vector<ProjectileState> playerProjectiles;
        std::vector<ProjectileState> enemyProjectiles;
        std::vector<PowerUpState> powerUps;


        void Reset(uint8_t lives)
        {
            player.Reset(lives);
            enemies.clear();
            enemyProjectiles.clear();
            playerProjectiles.clear();
            powerUps.clear();
        }
    };
}
