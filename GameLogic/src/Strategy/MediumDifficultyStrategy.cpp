#include "pch.h"
#include "Strategy/MediumDifficultyStrategy.h"

namespace GameLogic
{
	uint8_t MediumDifficultyStrategy::GetPlayerLivesCount() const noexcept
	{
		return m_config.playerLivesCount;
	}

	float MediumDifficultyStrategy::GetEnemyHealthMultiplier() const noexcept
	{
		return m_config.enemyHealthMultiplier;
	}

	float MediumDifficultyStrategy::GetScoreMultiplier() const noexcept
	{
		return m_config.scoreMultiplier;
	}
}

