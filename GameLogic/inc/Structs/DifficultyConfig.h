#pragma once

#include <cstdint>

namespace GameLogic
{
	struct DifficultyConfig
	{
		uint8_t playerLivesCount{};
		float enemyHealthMultiplier{};
		float scoreMultiplier{};
	};
}