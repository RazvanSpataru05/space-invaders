#include "pch.h"
#include "GameConfigFactory.h"

namespace GameLogic
{
	GameConfig CreateDefaultConfig()
	{
		GameConfig config;

		config.powerUpDropChance = 0.10f;
		config.playerMovementSpeed = 300.0f;
		config.enemyMovementSpeed = 100.0f;
		config.projectileSpeed = 400.0f;
		config.levels = LoadLevels();

		return config;
	}
}

