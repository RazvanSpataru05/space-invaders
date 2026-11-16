#pragma once

#include "Structs/LevelConfig.h"

#include <cstdint>
#include <array>

namespace GameLogic
{
	struct GameConfig
	{
		std::array<LevelConfig, 3> levels{};
		float playerMovementSpeed{};
		float powerUpChance{};
		float shootCooldown{};
		float startCooldown{};

		float projectileSpeed{};

		float enemyMovementSpeed{};
		float enemyDropDistance{};

		uint16_t projectileDamage{};
		
		uint16_t normalEnemyHealth{};
		uint16_t armoredEnemyHealth{};
		uint16_t shooterEnemyHealth{};
		uint16_t bossEnemyHealth{};

		uint16_t normalEnemyScore{};
		uint16_t armoredEnemyScore{};
		uint16_t shooterEnemyScore{};
		uint16_t bossScore{};

		uint8_t playerLivesCount{};
	};
}
