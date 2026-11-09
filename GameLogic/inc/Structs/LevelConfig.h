#pragma once

#include "Enums/GameEventType.h"

#include <cstdint>
#include <vector>

namespace GameLogic
{
	struct LevelConfig
	{
		uint8_t enemyCount{};
		std::vector<GameEventType> enemyTypes;
		bool hasBoss{};
	};
}
