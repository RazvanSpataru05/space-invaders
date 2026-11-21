#include "pch.h"
#include "Utils/EnemyUtils.h"
#include "Utils/Constants.h"

namespace Components = GameLogic::Utils::Constants;
namespace Utils = GameLogic::Utils;

class CheckEdgeTests : public ::testing::Test
{
protected:
	static GameLogic::EnemyState EnemyAt(float x)
	{
		return GameLogic::EnemyState{ GameLogic::Vector2{ x, 100.0f }, 100, GameLogic::EnemyType::NORMAL };
	}

	static constexpr float leftLimit = Components::WIDTH_OFFSET;
	static constexpr float rightLimit = Components::SCREEN_WIDTH - Components::WIDTH_OFFSET - Components::SHIP_WIDTH;
};

TEST_F(CheckEdgeTests, DetectsLeftEdgeWhenMovingLeftPastLimit)
{
	EXPECT_TRUE(Utils::CheckEdge(EnemyAt(leftLimit - 1.0f), -1.0f));
}

TEST_F(CheckEdgeTests, DoesNotDetectLeftEdgeExactlyAtLimit)
{
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(leftLimit), -1.0f));
}

TEST_F(CheckEdgeTests, DetectsRightEdgeWhenMovingRightPastLimit)
{
	EXPECT_TRUE(Utils::CheckEdge(EnemyAt(rightLimit + 1.0f), 1.0f));
}

TEST_F(CheckEdgeTests, DoesNotDetectRightEdgeExactlyAtLimit)
{
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(rightLimit), 1.0f));
}

TEST_F(CheckEdgeTests, IgnoresLeftEdgeWhenMovingRight)
{
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(leftLimit - 1.0f), 1.0f));
}

TEST_F(CheckEdgeTests, IgnoresRightEdgeWhenMovingLeft)
{
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(rightLimit + 1.0f), -1.0f));
}

TEST_F(CheckEdgeTests, IgnoresBothEdgesWhenNotMoving)
{
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(leftLimit - 1.0f), 0.0f));
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(rightLimit + 1.0f), 0.0f));
}

TEST_F(CheckEdgeTests, DoesNotDetectEdgeInTheMiddleOfTheScreen)
{
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(Components::SCREEN_WIDTH / 2.0f), 1.0f));
	EXPECT_FALSE(Utils::CheckEdge(EnemyAt(Components::SCREEN_WIDTH / 2.0f), -1.0f));
}