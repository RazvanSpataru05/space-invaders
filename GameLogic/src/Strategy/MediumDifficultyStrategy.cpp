#include "pch.h"
#include "Strategy/MediumDifficultyStrategy.h"

namespace GameLogic
{
	uint8_t MediumDifficultyStrategy::GetPlayerMaxLives() const noexcept
	{
		return m_config.playerMaxLives;
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

