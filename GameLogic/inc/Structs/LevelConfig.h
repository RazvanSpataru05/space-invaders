#pragma once

#include "Enums/GameEventType.h"

#include "Structs/EnemyState.h"

#include <cstdint>
#include <vector>

namespace GameLogic
{
	struct LevelConfig
	{
		uint8_t enemyCount{};
		std::vector<EnemyState> enemies;
		bool hasBoss{};
	};
}
