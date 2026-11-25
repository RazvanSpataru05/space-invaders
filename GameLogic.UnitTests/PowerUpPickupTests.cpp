#include "pch.h"
#include "Structs/PowerUpPickup.h"

TEST(PowerUpPickupTests, ConstructorSetsAllFields)
{
	const GameLogic::PowerUpPickup pickup{ GameLogic::Vector2{ 3.0f, 4.0f }, GameLogic::PowerUpType::MULTI_SHOT };

	EXPECT_FLOAT_EQ(pickup.position.x, 3.0f);
	EXPECT_FLOAT_EQ(pickup.position.y, 4.0f);
	EXPECT_EQ(pickup.type, GameLogic::PowerUpType::MULTI_SHOT);
}