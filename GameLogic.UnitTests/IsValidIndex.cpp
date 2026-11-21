#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace Components = GameLogic::Utils::Constants;
namespace Utils = GameLogic::Utils;

TEST(IsValidIndexTests, AcceptsFirstCell)
{
	EXPECT_TRUE(Utils::IsValidIndex(0, 0, Components::WALL_ROWS, Components::WALL_COLS));
}

TEST(IsValidIndexTests, AcceptsLastCell)
{
	EXPECT_TRUE(Utils::IsValidIndex(Components::WALL_ROWS - 1, Components::WALL_COLS - 1,
									Components::WALL_ROWS, Components::WALL_COLS));
}

TEST(IsValidIndexTests, RejectsRowEqualToRowCount)
{
	EXPECT_FALSE(Utils::IsValidIndex(Components::WALL_ROWS, 0, Components::WALL_ROWS, Components::WALL_COLS));
}

TEST(IsValidIndexTests, RejectsColumnEqualToColumnCount)
{
	EXPECT_FALSE(Utils::IsValidIndex(0, Components::WALL_COLS, Components::WALL_ROWS, Components::WALL_COLS));
}

TEST(IsValidIndexTests, RejectsColumnBeforeFirst)
{
	const size_t col = 0;

	EXPECT_FALSE(Utils::IsValidIndex(0, col - 1, Components::WALL_ROWS, Components::WALL_COLS));
}

TEST(IsValidIndexTests, RejectsRowBeforeFirst)
{
	const size_t row = 0;

	EXPECT_FALSE(Utils::IsValidIndex(row - 1, 0, Components::WALL_ROWS, Components::WALL_COLS));
}