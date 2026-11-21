#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceWallTests : public GameServiceFixture
{};

TEST_F(GameServiceWallTests, EnemyProjectileDestroysTheWallCellItHitsAndItsNeighbours)
{
	config.enemyShootCooldown = 0.0f;
	AddEnemy(110.0f, 460.0f, 150);
	Start();

	Run(3);

	const auto& cells = service->GetWallsState()[0].cells;
	const auto cell = [&](size_t row, size_t col) { return cells[row * C::WALL_COLS + col]; };
	EXPECT_FALSE(cell(4, 10));
	EXPECT_FALSE(cell(5, 10));
	EXPECT_FALSE(cell(4, 9));
	EXPECT_FALSE(cell(4, 11));
	EXPECT_TRUE(cell(3, 10));

	size_t templateGaps = 0;
	for (const auto line : C::WALL_TEMPLATE) templateGaps += std::count(line.begin(), line.end(), '.');
	EXPECT_EQ(static_cast<size_t>(std::count(cells.begin(), cells.end(), false)), templateGaps + 4u);

	EXPECT_EQ(service->GetPlayerState().livesCount, 3u);
}

TEST_F(GameServiceWallTests, StartingTheLevelAgainRepairsTheWalls)
{
	config.enemyShootCooldown = 0.0f;
	AddEnemy(110.0f, 460.0f, 150);
	Start();
	Run(3);

	service->StartLevel();

	EXPECT_TRUE(service->GetWallsState()[0].cells[4 * C::WALL_COLS + 10]);
}

TEST_F(GameServiceWallTests, EnemyTouchingAWallDestroysTheCellsItCovers)
{
	AddEnemy(100.0f, 560.0f, 150);
	Start();

	service->Update(kFrame);

	const auto& cells = service->GetWallsState()[0].cells;
	for (size_t row = 0; row < 8; ++row)
	{
		for (size_t col = 4; col < 12; ++col)
		{
			EXPECT_FALSE(cells[row * C::WALL_COLS + col]) << "row " << row << " col " << col;
		}
	}
	EXPECT_TRUE(cells[8 * C::WALL_COLS + 4]);
	EXPECT_TRUE(cells[4 * C::WALL_COLS + 12]);
}
