#pragma once

#include "Structs/LevelConfig.h"

#include <cstdint>
#include <array>

namespace GameLogic
{
	struct GameConfig
	{
		uint8_t playerInitialLives{};
		float powerUpDropChance{};
		float playerMovementSpeed{};
		float projectileSpeed{};
		float enemyMovementSpeed{};
		std::array<LevelConfig, 3> levels{};	
	};
}
