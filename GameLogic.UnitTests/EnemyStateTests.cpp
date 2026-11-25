#include "pch.h"
#include "Structs/EnemyState.h"

TEST(EnemyStateTests, ConstructorSetsAllFields)
{
	const GameLogic::EnemyState enemy{ GameLogic::Vector2{ 10.0f, 20.0f }, 150, GameLogic::EnemyType::ARMORED };

	EXPECT_FLOAT_EQ(enemy.position.x, 10.0f);
	EXPECT_FLOAT_EQ(enemy.position.y, 20.0f);
	EXPECT_EQ(enemy.health, 150);
	EXPECT_EQ(enemy.type, GameLogic::EnemyType::ARMORED);
}