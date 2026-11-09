#include "pch.h"
#include "Strategy/HardDifficultyStrategy.h"

namespace GameLogic
{
	uint8_t HardDifficultyStrategy::GetPlayerMaxLives() const noexcept
	{
		return m_config.playerMaxLives;
	}

	float HardDifficultyStrategy::GetEnemyHealthMultiplier() const noexcept
	{
		return m_config.enemyHealthMultiplier;
	}

	float HardDifficultyStrategy::GetScoreMultiplier() const noexcept
	{
		return m_config.scoreMultiplier;
	}

}
