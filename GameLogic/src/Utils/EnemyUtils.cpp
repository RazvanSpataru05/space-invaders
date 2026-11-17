#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace GameLogic::Utils
{
	bool CheckEdge(const EnemyState& enemy, float direction)
	{
		return (direction < 0.0f && enemy.position.x < Constants::WIDTH_OFFSET) ||
				(direction > 0.0f && (enemy.position.x + Constants::SHIP_WIDTH) > (Constants::SCREEN_WIDTH - Constants::WIDTH_OFFSET));
	}

	void UpdateEnemyHorizontalPosition(EnemyState& enemy, const GameProgress& progress, float dt)
	{
		enemy.position.x += (progress.baseEnemySpeed * dt) * progress.enemyDirection;
	}

	void UpdateEnemyVerticalPosition(EnemyState& enemy, const GameProgress& progress, float dt)
	{
		enemy.position.y += (progress.dropDistance * dt);
	}

	std::optional<std::uniform_int_distribution<size_t>> GetUniformEnemyDistribution(size_t enemyCount)
	{
		if (enemyCount == 0) return std::nullopt;

		return std::uniform_int_distribution<size_t>(0, enemyCount - 1);
	}

	bool AABBCollision(float xStartObj, float xEndObj, float yStartObj,
		float yEndObj, float xProj, float yProj)
	{
		return xStartObj < xProj + Constants::PROJECTILE_WIDTH
				&& xEndObj > xProj
				&& yStartObj < yProj + Constants::PROJECTILE_HEIGHT
				&& yEndObj > yProj;
	}
}