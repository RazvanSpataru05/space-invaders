#pragma once

#include "Structs/LevelConfig.h"

#include <cstdint>
#include <array>

namespace GameLogic
{
	struct GameConfig
	{
		float powerUpDropChance{};
		float playerMovementSpeed{};
		float enemyMovementSpeed{};

		float projectileSpeed{};
		std::array<LevelConfig, 3> levels{};	
	};
}
