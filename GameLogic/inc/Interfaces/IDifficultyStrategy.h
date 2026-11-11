#pragma once

#include "Structs/DifficultyConfig.h"

#include <cstdint>

namespace GameLogic
{
	class IDifficultyStrategy
	{
	public:
		virtual ~IDifficultyStrategy() = default;

		[[nodiscard]] virtual uint8_t	GetPlayerLivesCount()		const noexcept;
		[[nodiscard]] virtual float		GetEnemyHealthMultiplier()	const noexcept;
		[[nodiscard]] virtual float		GetScoreMultiplier()		const noexcept;

	private:
	};
}
