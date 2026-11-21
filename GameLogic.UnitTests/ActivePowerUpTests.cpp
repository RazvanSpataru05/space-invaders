#include "pch.h"
#include "Structs/ActivePowerUp.h"

TEST(ActivePowerUpTests, ConstructorSetsAllFields)
{
	const GameLogic::ActivePowerUp active{ 10.0f, GameLogic::PowerUpType::RAPID_FIRE };

	EXPECT_FLOAT_EQ(active.duration, 10.0f);
	EXPECT_EQ(active.type, GameLogic::PowerUpType::RAPID_FIRE);
}