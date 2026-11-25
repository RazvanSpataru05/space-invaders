#include "pch.h"
#include "Structs/Bounds.h"
#include "API/Utils/GameConstants.h"

namespace Constants = GameLogic::Utils::Constants;

class BoundsProjectileTests : public ::testing::Test
{
protected:
	const Bounds target{ 0.0f, 0.0f, 10.0f, 10.0f };
};

TEST_F(BoundsProjectileTests, OverlapsWhenProjectileIsInside)
{
	EXPECT_TRUE(target.Overlaps(GameLogic::Vector2{ 5.0f, 5.0f }));
}

TEST_F(BoundsProjectileTests, DoesNotOverlapWhenProjectileIsFarAway)
{
	EXPECT_FALSE(target.Overlaps(GameLogic::Vector2{ 100.0f, 100.0f }));
}

TEST_F(BoundsProjectileTests, DoesNotOverlapWhenProjectileTouchesLeftEdge)
{
	EXPECT_FALSE(target.Overlaps(GameLogic::Vector2{ -Constants::PROJECTILE_WIDTH, 5.0f }));
}

TEST_F(BoundsProjectileTests, OverlapsWhenProjectilePartlyCrossesLeftEdge)
{
	EXPECT_TRUE(target.Overlaps(GameLogic::Vector2{ -Constants::PROJECTILE_WIDTH / 2.0f, 5.0f }));
}

TEST_F(BoundsProjectileTests, DoesNotOverlapWhenProjectileTouchesRightEdge)
{
	EXPECT_FALSE(target.Overlaps(GameLogic::Vector2{ 10.0f, 5.0f }));
}

TEST_F(BoundsProjectileTests, DoesNotOverlapWhenProjectileTouchesTopEdge)
{
	EXPECT_FALSE(target.Overlaps(GameLogic::Vector2{ 5.0f, -Constants::PROJECTILE_HEIGHT }));
}

TEST_F(BoundsProjectileTests, OverlapsWhenProjectilePartlyCrossesTopEdge)
{
	EXPECT_TRUE(target.Overlaps(GameLogic::Vector2{ 5.0f, -Constants::PROJECTILE_HEIGHT / 2.0f }));
}

TEST_F(BoundsProjectileTests, DoesNotOverlapWhenProjectileTouchesBottomEdge)
{
	EXPECT_FALSE(target.Overlaps(GameLogic::Vector2{ 5.0f, 10.0f }));
}