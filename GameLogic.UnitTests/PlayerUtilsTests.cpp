#include "pch.h"
#include "Utils/PlayerUtils.h"

namespace Utils = GameLogic::Utils;

class UpdateProjectilesPositionTests : public ::testing::Test
{
protected:
	static GameLogic::ProjectileState ProjectileWithVelocity(float velocity)
	{
		return GameLogic::ProjectileState{ GameLogic::Vector2{ 300.0f, 600.0f }, velocity, 1u };
	}
};

TEST_F(UpdateProjectilesPositionTests, MovesUpWhenVelocityIsNegative)
{
	auto projectile = ProjectileWithVelocity(-400.0f);

	Utils::UpdateProjectilesPosition(projectile, 0.5f);

	EXPECT_FLOAT_EQ(projectile.position.y, 400.0f);
}

TEST_F(UpdateProjectilesPositionTests, MovesDownWhenVelocityIsPositive)
{
	auto projectile = ProjectileWithVelocity(400.0f);

	Utils::UpdateProjectilesPosition(projectile, 0.5f);

	EXPECT_FLOAT_EQ(projectile.position.y, 800.0f);
}

TEST_F(UpdateProjectilesPositionTests, DoesNotChangeX)
{
	auto projectile = ProjectileWithVelocity(-400.0f);

	Utils::UpdateProjectilesPosition(projectile, 0.5f);

	EXPECT_FLOAT_EQ(projectile.position.x, 300.0f);
}

TEST_F(UpdateProjectilesPositionTests, DoesNotMoveWhenNoTimePassed)
{
	auto projectile = ProjectileWithVelocity(-400.0f);

	Utils::UpdateProjectilesPosition(projectile, 0.0f);

	EXPECT_FLOAT_EQ(projectile.position.y, 600.0f);
}