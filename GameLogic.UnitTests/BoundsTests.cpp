#include "pch.h"
#include "Structs/Bounds.h"

namespace Constants = GameLogic::Utils::Constants;

TEST(BoundsTests, ConstructorSetsAllFields)
{
	const Bounds bounds{ 1.0f, 2.0f, 3.0f, 4.0f };

	EXPECT_FLOAT_EQ(bounds.x, 1.0f);
	EXPECT_FLOAT_EQ(bounds.y, 2.0f);
	EXPECT_FLOAT_EQ(bounds.width, 3.0f);
	EXPECT_FLOAT_EQ(bounds.height, 4.0f);
}

TEST(BoundsTests, GetBoundsCreatesSquareAtPosition)
{
	const Bounds bounds = GetBounds(GameLogic::Vector2{ 10.0f, 20.0f }, 40.0f);

	EXPECT_FLOAT_EQ(bounds.x, 10.0f);
	EXPECT_FLOAT_EQ(bounds.y, 20.0f);
	EXPECT_FLOAT_EQ(bounds.width, 40.0f);
	EXPECT_FLOAT_EQ(bounds.height, 40.0f);
}

TEST(BoundsTests, OverlapsWhenRectanglesIntersect)
{
	const Bounds a{ 0.0f, 0.0f, 10.0f, 10.0f };
	const Bounds b{ 5.0f, 5.0f, 10.0f, 10.0f };

	EXPECT_TRUE(a.Overlaps(b));
	EXPECT_TRUE(b.Overlaps(a));
}

TEST(BoundsTests, OverlapsWhenOneRectangleContainsTheOther)
{
	const Bounds outer{ 0.0f, 0.0f, 10.0f, 10.0f };
	const Bounds inner{ 2.0f, 2.0f, 3.0f, 3.0f };

	EXPECT_TRUE(outer.Overlaps(inner));
	EXPECT_TRUE(inner.Overlaps(outer));
}

TEST(BoundsTests, DoesNotOverlapWhenSeparatedHorizontally)
{
	const Bounds a{ 0.0f, 0.0f, 10.0f, 10.0f };
	const Bounds b{ 20.0f, 0.0f, 10.0f, 10.0f };

	EXPECT_FALSE(a.Overlaps(b));
	EXPECT_FALSE(b.Overlaps(a));
}

TEST(BoundsTests, DoesNotOverlapWhenSeparatedVertically)
{
	const Bounds a{ 0.0f, 0.0f, 10.0f, 10.0f };
	const Bounds b{ 0.0f, 20.0f, 10.0f, 10.0f };

	EXPECT_FALSE(a.Overlaps(b));
	EXPECT_FALSE(b.Overlaps(a));
}

TEST(BoundsTests, DoesNotOverlapWhenTouchingOnVerticalEdge)
{
	const Bounds a{ 0.0f, 0.0f, 10.0f, 10.0f };
	const Bounds b{ 10.0f, 0.0f, 10.0f, 10.0f };

	EXPECT_FALSE(a.Overlaps(b));
	EXPECT_FALSE(b.Overlaps(a));
}

TEST(BoundsTests, DoesNotOverlapWhenTouchingOnHorizontalEdge)
{
	const Bounds a{ 0.0f, 0.0f, 10.0f, 10.0f };
	const Bounds b{ 0.0f, 10.0f, 10.0f, 10.0f };

	EXPECT_FALSE(a.Overlaps(b));
	EXPECT_FALSE(b.Overlaps(a));
}