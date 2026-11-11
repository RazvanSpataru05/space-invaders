#include "pch.h"
#include "Strategy/EasyDifficultyStrategy.h"

namespace GameLogic
{
	uint8_t EasyDifficultyStrategy::GetPlayerLivesCount() const noexcept
	{
		return m_config.playerLivesCount;
	}

	float EasyDifficultyStrategy::GetEnemyHealthMultiplier() const noexcept
	{
		return m_config.enemyHealthMultiplier;
	}

	float EasyDifficultyStrategy::GetScoreMultiplier() const noexcept
	{
		return m_config.scoreMultiplier;
	}
}

