#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace Components = GameLogic::Utils::Constants;
namespace Utils = GameLogic::Utils;

TEST(GetUniformEnemyDistributionTests, ReturnsNothingWhenThereAreNoEnemies)
{
	EXPECT_FALSE(Utils::GetUniformEnemyDistribution(0).has_value());
}

TEST(GetUniformEnemyDistributionTests, CoversLastTwoRowsWhenManyEnemies)
{
	const auto distribution = Utils::GetUniformEnemyDistribution(30);

	ASSERT_TRUE(distribution.has_value());
	EXPECT_EQ(distribution->a(), 18u);
	EXPECT_EQ(distribution->b(), 29u);
}

TEST(GetUniformEnemyDistributionTests, CoversAllEnemiesWhenExactlyTwoRowsRemain)
{
	const auto distribution = Utils::GetUniformEnemyDistribution(Components::TWO_ROWS_COUNT);

	ASSERT_TRUE(distribution.has_value());
	EXPECT_EQ(distribution->a(), 0u);
	EXPECT_EQ(distribution->b(), static_cast<size_t>(Components::TWO_ROWS_COUNT - 1));
}

TEST(GetUniformEnemyDistributionTests, StartsAtZeroWhenFewerThanTwoRowsRemain)
{
	const auto distribution = Utils::GetUniformEnemyDistribution(5);

	ASSERT_TRUE(distribution.has_value());
	EXPECT_EQ(distribution->a(), 0u);
	EXPECT_EQ(distribution->b(), 4u);
}

TEST(GetUniformEnemyDistributionTests, WorksWithASingleEnemy)
{
	const auto distribution = Utils::GetUniformEnemyDistribution(1);

	ASSERT_TRUE(distribution.has_value());
	EXPECT_EQ(distribution->a(), 0u);
	EXPECT_EQ(distribution->b(), 0u);
}