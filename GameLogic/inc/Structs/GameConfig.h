#pragma once

#include "Structs/LevelConfig.h"

#include <cstdint>
#include <array>

namespace GameLogic
{
	struct GameConfig
	{
		std::array<LevelConfig, 3> levels{};	
		float powerUpDropChance{};
		float playerMovementSpeed{};
		float enemyMovementSpeed{};
		float projectileSpeed{};
	};
}
