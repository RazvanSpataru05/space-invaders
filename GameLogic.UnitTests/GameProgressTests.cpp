#include "pch.h"
#include "API/Structs/GameProgress.h"
#include "Utils/Constants.h"

namespace Constants = GameLogic::Utils::Constants;

class GameProgressTests : public ::testing::Test
{
protected:
	GameLogic::GameConfig config;

	void SetUp() override
	{
		config.enemyMovementSpeed = 60.0f;
		config.enemyDropDistance = 150.0f;
	}

	static void MakeDirty(GameLogic::GameProgress& progress)
	{
		progress.score = 1234u;
		progress.projectileCount = 7u;
		progress.elapsedTime = 12.5f;
		progress.playerShootCooldown = 0.3f;
		progress.enemyShootCooldown = 0.4f;
		progress.ufoSpawnTimer = 9.0f;
		progress.playerRespawnTimer = 1.5f;
		progress.enemyDirection = -1.0f;
		progress.baseEnemySpeed = 999.0f;
		progress.dropDistance = 888.0f;
		progress.fireRateMultiplier = 2.0f;
		progress.bossDefeated = true;
		progress.ufoApproaching = true;
		progress.ufoLeaving = true;
		progress.multiShot = true;
		progress.isPlayerRespawning = true;
	}
};

TEST_F(GameProgressTests, ConstructorTakesEnemySpeedAndDropDistanceFromConfig)
{
	const GameLogic::GameProgress progress{ config };

	EXPECT_FLOAT_EQ(progress.baseEnemySpeed, 60.0f);
	EXPECT_FLOAT_EQ(progress.dropDistance, 150.0f);
}

TEST_F(GameProgressTests, ConstructorSetsInitialEnemyDirection)
{
	const GameLogic::GameProgress progress{ config };

	EXPECT_FLOAT_EQ(progress.enemyDirection, Constants::INITIAL_ENEMY_DIRECTION);
}

TEST_F(GameProgressTests, ConstructorStartsWithZeroCountersAndTimers)
{
	const GameLogic::GameProgress progress{ config };

	EXPECT_EQ(progress.score, 0u);
	EXPECT_EQ(progress.projectileCount, 0u);
	EXPECT_FLOAT_EQ(progress.elapsedTime, 0.0f);
	EXPECT_FLOAT_EQ(progress.playerShootCooldown, 0.0f);
	EXPECT_FLOAT_EQ(progress.enemyShootCooldown, 0.0f);
	EXPECT_FLOAT_EQ(progress.ufoSpawnTimer, 0.0f);
	EXPECT_FLOAT_EQ(progress.playerRespawnTimer, 0.0f);
}

TEST_F(GameProgressTests, ConstructorStartsWithAllFlagsOff)
{
	const GameLogic::GameProgress progress{ config };

	EXPECT_FALSE(progress.bossDefeated);
	EXPECT_FALSE(progress.ufoApproaching);
	EXPECT_FALSE(progress.ufoLeaving);
	EXPECT_FALSE(progress.multiShot);
	EXPECT_FALSE(progress.isPlayerRespawning);
}

TEST_F(GameProgressTests, ConstructorSetsNormalFireRate)
{
	const GameLogic::GameProgress progress{ config };

	EXPECT_FLOAT_EQ(progress.fireRateMultiplier, 1.0f);
}

TEST_F(GameProgressTests, ChangeDirectionTurnsRightIntoLeft)
{
	GameLogic::GameProgress progress{ config };
	progress.enemyDirection = 1.0f;

	progress.ChangeDirection();

	EXPECT_FLOAT_EQ(progress.enemyDirection, -1.0f);
}

TEST_F(GameProgressTests, ChangeDirectionTurnsLeftIntoRight)
{
	GameLogic::GameProgress progress{ config };
	progress.enemyDirection = -1.0f;

	progress.ChangeDirection();

	EXPECT_FLOAT_EQ(progress.enemyDirection, 1.0f);
}


TEST_F(GameProgressTests, ResetRestoresMovementValues)
{
	GameLogic::GameProgress progress{ config };
	MakeDirty(progress);

	progress.Reset(config);

	EXPECT_FLOAT_EQ(progress.enemyDirection, Constants::INITIAL_ENEMY_DIRECTION);
	EXPECT_FLOAT_EQ(progress.baseEnemySpeed, 60.0f);
	EXPECT_FLOAT_EQ(progress.dropDistance, 150.0f);
}

TEST_F(GameProgressTests, ResetClearsCountersAndTimers)
{
	GameLogic::GameProgress progress{ config };
	MakeDirty(progress);

	progress.Reset(config);

	EXPECT_EQ(progress.score, 0u);
	EXPECT_EQ(progress.projectileCount, 0u);
	EXPECT_FLOAT_EQ(progress.elapsedTime, 0.0f);
	EXPECT_FLOAT_EQ(progress.playerShootCooldown, 0.0f);
	EXPECT_FLOAT_EQ(progress.enemyShootCooldown, 0.0f);
	EXPECT_FLOAT_EQ(progress.ufoSpawnTimer, 0.0f);
	EXPECT_FLOAT_EQ(progress.playerRespawnTimer, 0.0f);
}

TEST_F(GameProgressTests, ResetRemovesPowerUpEffects)
{
	GameLogic::GameProgress progress{ config };
	MakeDirty(progress);

	progress.Reset(config);

	EXPECT_FLOAT_EQ(progress.fireRateMultiplier, 1.0f);
	EXPECT_FALSE(progress.multiShot);
}

TEST_F(GameProgressTests, ResetTurnsAllFlagsOff)
{
	GameLogic::GameProgress progress{ config };
	MakeDirty(progress);

	progress.Reset(config);

	EXPECT_FALSE(progress.bossDefeated);
	EXPECT_FALSE(progress.ufoApproaching);
	EXPECT_FALSE(progress.ufoLeaving);
	EXPECT_FALSE(progress.isPlayerRespawning);
}

TEST_F(GameProgressTests, ResetUsesValuesFromGameConfig)
{
	GameLogic::GameProgress progress{ config };
	GameLogic::GameConfig otherConfig = config;
	otherConfig.enemyMovementSpeed = 90.0f;
	otherConfig.enemyDropDistance = 40.0f;

	progress.Reset(otherConfig);

	EXPECT_FLOAT_EQ(progress.baseEnemySpeed, 90.0f);
	EXPECT_FLOAT_EQ(progress.dropDistance, 40.0f);
}