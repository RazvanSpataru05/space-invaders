#include "pch.h"
#include "API/Structs/Vector2.h"

TEST(Vector2Tests, DefaultConstructorSetsCoordinatesToZero)
{
	const GameLogic::Vector2 vector;

	EXPECT_FLOAT_EQ(vector.x, 0.0f);
	EXPECT_FLOAT_EQ(vector.y, 0.0f);
}

TEST(Vector2Tests, ParameterizedConstructor)
{
	GameLogic::Vector2 vector(3.5f, -2.1f);
	EXPECT_FLOAT_EQ(vector.x, 3.5f);
	EXPECT_FLOAT_EQ(vector.y, -2.1f);
}