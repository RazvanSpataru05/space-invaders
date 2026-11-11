#pragma once

#include "Interfaces/IDifficultyStrategy.h"

namespace GameLogic
{
	class EasyDifficultyStrategy : public IDifficultyStrategy
	{
	public:
		EasyDifficultyStrategy() = default;

		[[nodiscard]] uint8_t GetPlayerLivesCount()				const noexcept override;
		[[nodiscard]] float GetEnemyHealthMultiplier()			const noexcept override;
		[[nodiscard]] float GetScoreMultiplier()				const noexcept override;

		~EasyDifficultyStrategy() override = default;

	private:
		inline static constexpr DifficultyConfig m_config
		{
			.playerLivesCount = 5u,
			.enemyHealthMultiplier = 1.0f,
			.scoreMultiplier = 1.0f
		};
	};
}
