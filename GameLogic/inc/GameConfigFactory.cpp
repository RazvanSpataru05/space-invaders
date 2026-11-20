#include "pch.h"
#include "Utils/GameConfigFactory.h"

namespace GameLogic
{
	GameConfig CreateDefaultConfig()
	{
		GameConfig config;

		config.playerMovementSpeed = 4.0f;
		config.powerUpChance = 0.25f;
		config.playerShootCooldown = 0.20f;
		config.enemyShootCooldown = 0.50f;
		config.startCooldown = 2.5f;
		config.ufoSpawnCooldown = 25.0f;
		config.powerUpDuration = 10.0f;
		config.playerRespawnTimer = 2.5f;

		config.countUFOSpawn = 8;

		config.projectileSpeed = 400.0f;
		config.playerProjectileDamage = 50u;

		config.ufoMovementSpeed = 100.0f;
		config.enemyMovementSpeed = 60.0f;
		config.bossMovementSpeed = 80.0f;
		config.enemyDropDistance = 150.0f;

		config.bossDropDistance = 50.0f;
		config.powerUpDropDistance = 75.0f;

		config.normalEnemyHealth = 150u;
		config.armoredEnemyHealth = 300u;
		config.shooterEnemyHealth = 200u;
		config.bossEnemyHealth = 2500u;

		config.normalEnemyScore = 30u;
		config.armoredEnemyScore = 50u;
		config.shooterEnemyScore = 80u;
		config.bossScore = 3000u;
		config.powerUpBonusScore = 1000u;

		config.maxEnemyProjectileCount = 15u;
		config.playerLivesCount = 3u;
		config.enemyProjectileDamage = 1u;

		LoadLevels(config);

		return config;
	}
}

