#include "pch.h"
#include "Structs/WallState.h"
#include "Utils/Constants.h"

#include <algorithm>

namespace Constants = GameLogic::Utils::Constants;

TEST(WallStateTests, HasOneCellForEachRowAndColumn)
{
	const GameLogic::WallState wall;

	EXPECT_EQ(wall.cells.size(), Constants::WALL_ROWS * Constants::WALL_COLS);
}

TEST(WallStateTests, DefaultConstructorSetsPositionToZeroAndEmptyCells)
{
	const GameLogic::WallState wall;

	EXPECT_FLOAT_EQ(wall.position.x, 0.0f);
	EXPECT_FLOAT_EQ(wall.position.y, 0.0f);
	EXPECT_TRUE(std::none_of(wall.cells.begin(), wall.cells.end(),
		[](bool cell) { return cell; }));
}

TEST(WallStateTests, ConstructorWithPositionSetsPositionAndFillsCells)
{
	const GameLogic::WallState wall{ Constants::FIRST_WALL_POSITION };

	EXPECT_FLOAT_EQ(wall.position.x, Constants::FIRST_WALL_POSITION.x);
	EXPECT_FLOAT_EQ(wall.position.y, Constants::FIRST_WALL_POSITION.y);
	EXPECT_TRUE(std::all_of(wall.cells.begin(), wall.cells.end(),
		[](bool cell) { return cell; }));
}

TEST(WallTemplateTests, HasOneLinePerWallRow)
{
	EXPECT_EQ(Constants::WALL_TEMPLATE.size(), Constants::WALL_ROWS);
}

TEST(WallTemplateTests, EveryLineHasOneCharacterPerWallColumn)
{
	for (size_t row = 0; row < Constants::WALL_TEMPLATE.size(); ++row)
	{
		EXPECT_EQ(Constants::WALL_TEMPLATE[row].size(), Constants::WALL_COLS) << "row " << row;
	}
}

TEST(WallTemplateTests, ContainsOnlyWallAndEmptyCharacters)
{
	for (size_t row = 0; row < Constants::WALL_TEMPLATE.size(); ++row)
	{
		for (const char cell : Constants::WALL_TEMPLATE[row])
		{
			EXPECT_TRUE(cell == '#' || cell == '.') << "row " << row << ": '" << cell << "'";
		}
	}
}