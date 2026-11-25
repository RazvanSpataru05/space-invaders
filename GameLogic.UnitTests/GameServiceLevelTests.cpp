#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceLevelTests : public GameServiceFixture
{};

TEST_F(GameServiceLevelTests, StartLevelSpawnsEnemiesFromConfig)
{
	AddEnemy(100.0f, 50.0f, 150, GameLogic::EnemyType::NORMAL);
	AddEnemy(200.0f, 60.0f, 300, GameLogic::EnemyType::ARMORED);
	Start();

	const auto enemies = service->GetEnemiesState();

	ASSERT_EQ(enemies.size(), 2u);
	EXPECT_FLOAT_EQ(enemies[0].position.x, 100.0f);
	EXPECT_FLOAT_EQ(enemies[0].position.y, 50.0f);
	EXPECT_EQ(enemies[0].health, 150);
	EXPECT_EQ(enemies[0].type, GameLogic::EnemyType::NORMAL);
	EXPECT_FLOAT_EQ(enemies[1].position.x, 200.0f);
	EXPECT_EQ(enemies[1].health, 300);
	EXPECT_EQ(enemies[1].type, GameLogic::EnemyType::ARMORED);
}

TEST_F(GameServiceLevelTests, StartLevelUsesTheSelectedLevel)
{
	AddEnemy(100.0f, 50.0f, 150, GameLogic::EnemyType::NORMAL, 0);
	AddEnemy(300.0f, 70.0f, 200, GameLogic::EnemyType::SHOOTER, 1);
	service = std::make_unique<GameService>(config, 1u);

	service->GetLevel() = 1;
	service->StartLevel();

	ASSERT_EQ(EnemyCount(), 1u);
	EXPECT_EQ(service->GetEnemiesState()[0].type, GameLogic::EnemyType::SHOOTER);
}

TEST_F(GameServiceLevelTests, StartLevelPlacesPlayerAtSpawnWithConfiguredLives)
{
	config.playerLivesCount = 4u;
	AddTargetAndBystander();
	Start();

	const auto player = service->GetPlayerState();

	EXPECT_FLOAT_EQ(player.position.x, C::PLAYER_SPAWN_POSITION.x);
	EXPECT_FLOAT_EQ(player.position.y, C::PLAYER_SPAWN_POSITION.y);
	EXPECT_EQ(player.livesCount, 4u);
	EXPECT_TRUE(player.isVisible);
}

TEST_F(GameServiceLevelTests, StartLevelStartsWithEmptyBoardAndZeroScore)
{
	AddTargetAndBystander();
	Start();

	EXPECT_EQ(service->GetScore(), 0u);
	EXPECT_TRUE(service->GetPlayerProjectilesState().empty());
	EXPECT_TRUE(service->GetEnemyProjectilesState().empty());
	EXPECT_TRUE(service->GetPowerUpPickups().empty());
	EXPECT_FALSE(service->GetUFOState().has_value());
	EXPECT_FALSE(service->GetBossState().has_value());
}

TEST_F(GameServiceLevelTests, StartLevelBuildsWallsFromTemplate)
{
	AddTargetAndBystander();
	Start();

	const auto walls = service->GetWallsState();
	const float wallStep = C::WALL_COLS * C::CELL_SIZE + C::WALLS_GAP;

	ASSERT_EQ(walls.size(), C::WALL_COUNT);
	for (size_t w = 0; w < walls.size(); ++w)
	{
		EXPECT_FLOAT_EQ(walls[w].position.x, C::FIRST_WALL_POSITION.x + w * wallStep) << "wall " << w;
		EXPECT_FLOAT_EQ(walls[w].position.y, C::FIRST_WALL_POSITION.y) << "wall " << w;

		for (size_t row = 0; row < C::WALL_ROWS; ++row)
		{
			for (size_t col = 0; col < C::WALL_COLS; ++col)
			{
				EXPECT_EQ(walls[w].cells[row * C::WALL_COLS + col], C::WALL_TEMPLATE[row][col] == '#')
					<< "wall " << w << " row " << row << " col " << col;
			}
		}
	}
}

TEST_F(GameServiceLevelTests, StartingTheLevelAgainDoesNotDuplicateEnemies)
{
	AddTargetAndBystander();
	Start();

	service->StartLevel();

	EXPECT_EQ(EnemyCount(), 2u);
}

TEST_F(GameServiceLevelTests, ResetRemovesEnemiesAndProjectiles)
{
	AddTargetAndBystander();
	Start();
	ShootNow();

	service->Reset();

	EXPECT_EQ(EnemyCount(), 0u);
	EXPECT_TRUE(service->GetPlayerProjectilesState().empty());
}
