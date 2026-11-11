#pragma once

#include "Structs/EnemyConfig.h"

#include <cstdint>
#include <vector>

namespace GameLogic
{
	struct LevelConfig
	{
		std::vector<EnemyConfig> enemies;
		bool hasBoss{};
	};
}
