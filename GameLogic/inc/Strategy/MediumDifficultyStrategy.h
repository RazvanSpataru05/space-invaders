#pragma once

#include "Interfaces/IDifficultyStrategy.h"



namespace GameLogic
{
	class MediumDifficultyStrategy : public IDifficultyStrategy
	{
	public:
		MediumDifficultyStrategy() = default;

		[[nodiscard]] uint8_t	GetPlayerLivesCount()				const noexcept override;
		[[nodiscard]] float		GetEnemyHealthMultiplier()		const noexcept override;
		[[nodiscard]] float		GetScoreMultiplier()			const noexcept override;

		~MediumDifficultyStrategy() override = default;

	private:
		inline static constexpr DifficultyConfig m_config
		{
			.playerLivesCount = 3u,
			.enemyHealthMultiplier = 1.5f,
			.scoreMultiplier = 1.5f
		};
	};
}



