#include "pch.h"
#include "Structs/BossState.h"

class BossStateTests : public ::testing::Test
{
protected:
	const GameLogic::Vector2 position{ 3.5f, -2.1f };
	GameLogic::BossState boss{ position, 100 };
};

TEST_F(BossStateTests, ConstructorSetsValuesCorrectly)
{
	EXPECT_FLOAT_EQ(boss.position.x, 3.5f);
	EXPECT_FLOAT_EQ(boss.position.y, -2.1f);
	EXPECT_FLOAT_EQ(boss.direction, 1.0f);
	EXPECT_EQ(boss.health, 100);
	EXPECT_EQ(boss.maxHealth, 100);
	EXPECT_FALSE(boss.isVisible);
}

TEST_F(BossStateTests, ChangeDirectionToLeft)
{
	const GameLogic::Vector2 position{ 3.5f, -2.1f };
	GameLogic::BossState boss{ position, 100 };

	boss.ChangeDirection();

	EXPECT_FLOAT_EQ(boss.direction, -1.0f);
}

TEST_F(BossStateTests, ChangeDirectionToRight)
{
	const GameLogic::Vector2 position{ 3.5f, -2.1f };
	GameLogic::BossState boss{ position, 100 };
	boss.direction = -1.0f;

	boss.ChangeDirection();

	EXPECT_FLOAT_EQ(boss.direction, 1.0f);
}