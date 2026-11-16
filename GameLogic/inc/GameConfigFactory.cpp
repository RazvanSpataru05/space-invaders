#include "pch.h"
#include "Utils/GameConfigFactory.h"

namespace GameLogic
{
	GameConfig CreateDefaultConfig()
	{
		GameConfig config;

		config.playerMovementSpeed = 4.0f;
		config.powerUpChance = 0.10f;
		config.playerShootCooldown = 0.25f;
		config.enemyShootCooldown = 0.5f;
		config.startCooldown = 2.0f;

		config.projectileSpeed = 400.0f;
		config.playerProjectileDamage = 50u;

		config.enemyMovementSpeed = 60.0f;
		config.enemyDropDistance = 150.0f;

		config.normalEnemyHealth = 100u;
		config.armoredEnemyHealth = 250u;
		config.shooterEnemyHealth = 150u;
		config.bossEnemyHealth = 1000u;

		config.normalEnemyScore = 30u;
		config.armoredEnemyScore = 50u;
		config.shooterEnemyScore = 80u;
		config.bossScore = 500u;

		config.maxEnemyProjectileCount = 10u;
		config.playerLivesCount = 3u;
		config.enemyProjectileDamage = 1u;

		LoadLevels(config);

		return config;


	}
}

