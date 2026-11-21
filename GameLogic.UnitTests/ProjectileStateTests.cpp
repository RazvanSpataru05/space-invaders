#include "pch.h"
#include "Structs/ProjectileState.h"

TEST(ProjectileStateTests, ConstructorSetsAllFields)
{
	const GameLogic::ProjectileState projectile{ GameLogic::Vector2{ 1.0f, 2.0f }, -400.0f, 50u };

	EXPECT_FLOAT_EQ(projectile.position.x, 1.0f);
	EXPECT_FLOAT_EQ(projectile.position.y, 2.0f);
	EXPECT_FLOAT_EQ(projectile.velocity, -400.0f);
	EXPECT_EQ(projectile.damage, 50u);
}