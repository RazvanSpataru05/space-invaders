#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace Utils = GameLogic::Utils;

class EnemyMovementTests : public ::testing::Test
{
protected:
	GameLogic::GameConfig config;
	GameLogic::GameProgress progress{ config };
	GameLogic::EnemyState enemy{ GameLogic::Vector2{ 100.0f, 200.0f }, 100, GameLogic::EnemyType::NORMAL };

	void SetUp() override
	{
		progress.baseEnemySpeed = 60.0f;
		progress.dropDistance = 150.0f;
		progress.enemyDirection = 1.0f;
	}
};

TEST_F(EnemyMovementTests, MovesRightAtBaseSpeedWhenAllEnemiesAlive)
{
	Utils::UpdateEnemyHorizontalPosition(enemy, progress, 10, 10, 0.5f);

	EXPECT_FLOAT_EQ(enemy.position.x, 130.0f);
}

TEST_F(EnemyMovementTests, MovesLeftWhenDirectionIsNegative)
{
	progress.enemyDirection = -1.0f;

	Utils::UpdateEnemyHorizontalPosition(enemy, progress, 10, 10, 0.5f);

	EXPECT_FLOAT_EQ(enemy.position.x, 70.0f);
}

TEST_F(EnemyMovementTests, MovesFasterWhenFewerEnemiesRemain)
{
	Utils::UpdateEnemyHorizontalPosition(enemy, progress, 4, 10, 0.5f);

	EXPECT_FLOAT_EQ(enemy.position.x, 175.0f);
}

TEST_F(EnemyMovementTests, HorizontalMoveDoesNotChangeY)
{
	Utils::UpdateEnemyHorizontalPosition(enemy, progress, 10, 10, 0.5f);

	EXPECT_FLOAT_EQ(enemy.position.y, 200.0f);
}

TEST_F(EnemyMovementTests, DoesNotMoveHorizontallyWhenNoTimePassed)
{
	Utils::UpdateEnemyHorizontalPosition(enemy, progress, 10, 10, 0.0f);

	EXPECT_FLOAT_EQ(enemy.position.x, 100.0f);
}

TEST_F(EnemyMovementTests, MovesDownByDropDistanceTimesDeltaTime)
{
	Utils::UpdateEnemyVerticalPosition(enemy, progress, 0.2f);

	EXPECT_FLOAT_EQ(enemy.position.y, 230.0f);
}

TEST_F(EnemyMovementTests, VerticalMoveDoesNotChangeX)
{
	Utils::UpdateEnemyVerticalPosition(enemy, progress, 0.2f);

	EXPECT_FLOAT_EQ(enemy.position.x, 100.0f);
}