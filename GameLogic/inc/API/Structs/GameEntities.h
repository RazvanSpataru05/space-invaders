#pragma once

#include "Structs/ActivePowerUp.h"
#include "Structs/EnemyState.h"
#include "Structs/PlayerState.h"
#include "Structs/ProjectileState.h"
#include "Structs/PowerUpPickup.h"
#include "Structs/UFOState.h"
#include "Structs/BossState.h"
#include "Structs/WallState.h"

#include <array>
#include <vector>
#include <optional>

namespace GameLogic
{
    struct GameEntities
    {
        PlayerState player;
        std::vector<EnemyState> enemies;
        std::vector<ProjectileState> playerProjectiles;
        std::vector<ProjectileState> enemyProjectiles;
        std::vector<PowerUpPickup> powerUpPickups;
        std::vector<ActivePowerUp> activePowerUps;
        std::array<WallState, Utils::Constants::WALL_COUNT> wallsState;
        std::optional<UFOState> ufo{ std::nullopt };
        std::optional<BossState> boss{ std::nullopt };

        void Reset(uint8_t lives)
        {
            player.Reset(lives);
            enemies.clear();
            enemyProjectiles.clear();
            playerProjectiles.clear();
            activePowerUps.clear();
            powerUpPickups.clear();
            ufo = std::nullopt;
            boss = std::nullopt;
        }
    };
}
