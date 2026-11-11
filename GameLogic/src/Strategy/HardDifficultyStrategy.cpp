#include "pch.h"
#include "Strategy/HardDifficultyStrategy.h"

namespace GameLogic
{
	uint8_t HardDifficultyStrategy::GetPlayerLivesCount() const noexcept
	{
		return m_config.playerLivesCount;
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
