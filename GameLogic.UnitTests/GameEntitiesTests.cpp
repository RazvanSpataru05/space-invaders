#include "pch.h"
#include "API/Structs/GameEntities.h"
#include "Utils/Constants.h"

namespace C = GameLogic::Utils::Constants;

class GameEntitiesTests : public ::testing::Test
{
protected:
	GameLogic::GameEntities entities;

	void SetUp() override
	{
		entities.player = GameLogic::PlayerState{ GameLogic::Vector2{ 10.0f, 20.0f }, 1u };
		entities.player.isVisible = false;

		entities.enemies.emplace_back(GameLogic::Vector2{ 100.0f, 50.0f }, 100, GameLogic::EnemyType::NORMAL);
		entities.playerProjectiles.emplace_back(GameLogic::Vector2{ 5.0f, 6.0f }, -400.0f, 50u);
		entities.enemyProjectiles.emplace_back(GameLogic::Vector2{ 7.0f, 8.0f }, 400.0f, 1u);
		entities.powerUpPickups.emplace_back(GameLogic::Vector2{ 9.0f, 11.0f }, GameLogic::PowerUpType::EXTRA_LIFE);
		entities.activePowerUps.emplace_back(10.0f, GameLogic::PowerUpType::RAPID_FIRE);
		entities.ufo.emplace(1.0f, C::UFO_LEFT_SPAWN_POSITION);
		entities.boss.emplace(C::BOSS_SPAWN_POSITION, 1000);
	}
};

TEST(GameEntitiesDefaultTests, StartsWithoutUfoAndBoss)
{
	const GameLogic::GameEntities entities;

	EXPECT_FALSE(entities.ufo.has_value());
	EXPECT_FALSE(entities.boss.has_value());
}

TEST(GameEntitiesDefaultTests, StartsWithEmptyContainers)
{
	const GameLogic::GameEntities entities;

	EXPECT_TRUE(entities.enemies.empty());
	EXPECT_TRUE(entities.playerProjectiles.empty());
	EXPECT_TRUE(entities.enemyProjectiles.empty());
	EXPECT_TRUE(entities.powerUpPickups.empty());
	EXPECT_TRUE(entities.activePowerUps.empty());
}

TEST(GameEntitiesDefaultTests, HasOneStateForEachWall)
{
	const GameLogic::GameEntities entities;

	EXPECT_EQ(entities.wallsState.size(), C::WALL_COUNT);
}

TEST_F(GameEntitiesTests, ResetRemovesAllEnemies)
{
	entities.Reset(3u);

	EXPECT_TRUE(entities.enemies.empty());
}

TEST_F(GameEntitiesTests, ResetRemovesPlayerAndEnemyProjectiles)
{
	entities.Reset(3u);

	EXPECT_TRUE(entities.playerProjectiles.empty());
	EXPECT_TRUE(entities.enemyProjectiles.empty());
}

TEST_F(GameEntitiesTests, ResetRemovesPowerUpPickupsAndActivePowerUps)
{
	entities.Reset(3u);

	EXPECT_TRUE(entities.powerUpPickups.empty());
	EXPECT_TRUE(entities.activePowerUps.empty());
}

TEST_F(GameEntitiesTests, ResetRemovesUfoAndBoss)
{
	entities.Reset(3u);

	EXPECT_FALSE(entities.ufo.has_value());
	EXPECT_FALSE(entities.boss.has_value());
}

TEST_F(GameEntitiesTests, ResetPlacesPlayerAtSpawnPositionAndGivesItLives)
{
	entities.Reset(4u);

	EXPECT_FLOAT_EQ(entities.player.position.x, C::PLAYER_SPAWN_POSITION.x);
	EXPECT_FLOAT_EQ(entities.player.position.y, C::PLAYER_SPAWN_POSITION.y);
	EXPECT_EQ(entities.player.livesCount, 4u);
}

TEST_F(GameEntitiesTests, ResetMakesPlayerVisible)
{
	entities.Reset(3u);

	EXPECT_TRUE(entities.player.isVisible);
}