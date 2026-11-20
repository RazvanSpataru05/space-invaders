#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace GameLogic::Utils
{
	bool CheckEdge(const EnemyState& enemy, float direction)
	{
		return (direction < 0.0f && enemy.position.x < Constants::WIDTH_OFFSET) ||
				(direction > 0.0f && (enemy.position.x + Constants::SHIP_WIDTH) > (Constants::SCREEN_WIDTH - Constants::WIDTH_OFFSET));
	}

	void UpdateEnemyHorizontalPosition(EnemyState& enemy, const GameProgress& progress, size_t remainingEnemies, size_t totalEnemies, float dt)
	{
		const float speed = progress.baseEnemySpeed * (static_cast<float>(totalEnemies) / static_cast<float>(remainingEnemies));
		enemy.position.x += (speed * dt) * progress.enemyDirection;
	}

	void UpdateEnemyVerticalPosition(EnemyState& enemy, const GameProgress& progress, float dt)
	{
		enemy.position.y += (progress.dropDistance * dt);
	}

	std::optional<std::uniform_int_distribution<size_t>> GetUniformEnemyDistribution(int max)
	{
		if (max == 0) return std::nullopt;

		return std::uniform_int_distribution<size_t>(std::max(0, max - Constants::TWO_ROWS_COUNT), max - 1);
	}

	bool AABBCollision(float xStartObj, float xEndObj, float yStartObj,
		float yEndObj, float xProj, float yProj)
	{
		return xStartObj < xProj + Constants::PROJECTILE_WIDTH
				&& xEndObj > xProj
				&& yStartObj < yProj + Constants::PROJECTILE_HEIGHT
				&& yEndObj > yProj;
	}

	bool IsValidIndex(size_t row, size_t col, size_t rowCount, size_t colCount)
	{
		return row >= 0
			&& row < rowCount
			&& col >= 0
			&& col < colCount;
	}
}