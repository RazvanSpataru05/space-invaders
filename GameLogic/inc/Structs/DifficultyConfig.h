#pragma once

#include <cstdint>

namespace GameLogic
{
	struct DifficultyConfig
	{
		uint8_t playerMaxLives;
		float enemyHealthMultiplier;
		float scoreMultiplier;
	};
}