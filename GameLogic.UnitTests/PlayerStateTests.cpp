#include "pch.h"
#include "Structs/PlayerState.h"

class PlayerStateTests : public ::testing::Test
{
protected:
	const GameLogic::Vector2 position{ 3.5f, -2.1f };
};

TEST_F(PlayerStateTests, DefaultConstructorSetsValuesToZero)
{
	const GameLogic::PlayerState playerState;

	EXPECT_FLOAT_EQ(playerState.position.x, 0.0f);
	EXPECT_FLOAT_EQ(playerState.position.y, 0.0f);
	EXPECT_EQ(playerState.livesCount, 0u);
	EXPECT_FALSE(playerState.isVisible);
}

TEST_F(PlayerStateTests, ParameterizedConstructorSetsValuesCorrectly)
{
	const GameLogic::PlayerState player(position, 5u);

	EXPECT_FLOAT_EQ(player.position.x, 3.5f);
	EXPECT_FLOAT_EQ(player.position.y, -2.1f);
	EXPECT_EQ(player.livesCount, 5u);
	EXPECT_FALSE(player.isVisible);
}

TEST_F(PlayerStateTests, ResetSetsValuesCorrectly)
{
	GameLogic::PlayerState player(position, 5u);
	const GameLogic::Vector2 resetPosition{ 1.0f, 2.0f };

	player.Reset(resetPosition, 3u);

	EXPECT_FLOAT_EQ(player.position.x, 1.0f);
	EXPECT_FLOAT_EQ(player.position.y, 2.0f);
	EXPECT_EQ(player.livesCount, 3u);
	EXPECT_TRUE(player.isVisible);
}