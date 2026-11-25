#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace Components = GameLogic::Utils::Constants;
namespace Utils = GameLogic::Utils;

TEST(AABBCollisionTests, CollidesWhenProjectileIsInside)
{
	EXPECT_TRUE(Utils::AABBCollision(0.0f, 10.0f, 0.0f, 10.0f, 5.0f, 5.0f));
}

TEST(AABBCollisionTests, DoesNotCollideWhenProjectileTouchesLeftEdge)
{
	EXPECT_FALSE(Utils::AABBCollision(0.0f, 10.0f, 0.0f, 10.0f, -Components::PROJECTILE_WIDTH, 5.0f));
}

TEST(AABBCollisionTests, DoesNotCollideWhenProjectileTouchesRightEdge)
{
	EXPECT_FALSE(Utils::AABBCollision(0.0f, 10.0f, 0.0f, 10.0f, 10.0f, 5.0f));
}

TEST(AABBCollisionTests, DoesNotCollideWhenProjectileTouchesTopEdge)
{
	EXPECT_FALSE(Utils::AABBCollision(0.0f, 10.0f, 0.0f, 10.0f, 5.0f, -Components::PROJECTILE_HEIGHT));
}

TEST(AABBCollisionTests, DoesNotCollideWhenProjectileTouchesBottomEdge)
{
	EXPECT_FALSE(Utils::AABBCollision(0.0f, 10.0f, 0.0f, 10.0f, 5.0f, 10.0f));
}