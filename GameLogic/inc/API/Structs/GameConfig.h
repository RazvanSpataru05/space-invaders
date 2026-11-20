#pragma once

#include "Structs/LevelConfig.h"

#include <cstdint>
#include <array>

namespace GameLogic
{
	struct GameConfig
	{
		std::array<LevelConfig, 3> levels{};
		float powerUpChance{};
		float playerShootCooldown{};
		float enemyShootCooldown{};
		float startCooldown{};
		float ufoSpawnCooldown{};
		float playerHitCooldown{};
		float powerUpDuration{};
		float playerRespawnTimer{};

		float playerMovementSpeed{};
		float projectileSpeed{};
		float ufoMovementSpeed{};
		float enemyMovementSpeed{};
		float bossMovementSpeed{};
		float enemyDropDistance{};
		
		float bossDropDistance{};
		float powerUpDropDistance{};
		
		size_t countUFOSpawn{};

		uint16_t normalEnemyHealth{};
		uint16_t armoredEnemyHealth{};
		uint16_t shooterEnemyHealth{};
		uint16_t bossEnemyHealth{};

		uint16_t normalEnemyScore{};
		uint16_t armoredEnemyScore{};
		uint16_t shooterEnemyScore{};
		uint16_t bossScore{};
		uint16_t powerUpBonusScore{};

		uint8_t maxEnemyProjectileCount{};
		uint8_t enemyProjectileDamage{};
		uint8_t playerProjectileDamage{};
		uint8_t playerLivesCount{};
	};
}
