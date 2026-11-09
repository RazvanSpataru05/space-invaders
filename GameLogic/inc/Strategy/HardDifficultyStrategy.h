#pragma once

#include "Interfaces/IDifficultyStrategy.h"

namespace GameLogic
{
	class HardDifficultyStrategy : public IDifficultyStrategy
	{
	public:
		HardDifficultyStrategy() = default;

		[[nodiscard]] uint8_t	GetPlayerMaxLives()				const noexcept override;
		[[nodiscard]] float		GetEnemyHealthMultiplier()		const noexcept override;
		[[nodiscard]] float		GetScoreMultiplier()			const noexcept override;

		~HardDifficultyStrategy() override = default;

	private:
		inline static constexpr DifficultyConfig m_config
		{
			.playerMaxLives = 2u,
			.enemyHealthMultiplier = 2.0f,
			.scoreMultiplier = 3.0f
		};
	};
}
