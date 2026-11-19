#include "pch.h"
#include "Utils/GameConfigFactory.h"

namespace GameLogic
{
	GameConfig CreateDefaultConfig()
	{
		GameConfig config;

		config.playerMovementSpeed = 4.0f;
		config.powerUpChance = 0.10f;
		config.playerShootCooldown = 0.33f;
		config.enemyShootCooldown = 0.5f;
		config.startCooldown = 2.0f;
		config.ufoSpawnCooldown = 25.0f;
		config.playerHitCooldown = 3.0f;
		config.powerUpDuration = 10.0f;

		config.countUFOSpawn = 8;

		config.projectileSpeed = 400.0f;
		config.playerProjectileDamage = 50u;

		config.ufoMovementSpeed = 100.0f;
		config.enemyMovementSpeed = 60.0f;
		config.enemyDropDistance = 150.0f;

		config.bossDropDistance = 50.0f;
		config.powerUpDropDistance = 40.0f;

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

