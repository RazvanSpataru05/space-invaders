#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace GameLogic::Utils
{
	bool CheckEdge(const EnemyState& enemy, float direction)
	{
		return (direction < 0.0f && enemy.position.x < Constants::WIDTH_OFFSET) ||
				(direction > 0.0f && enemy.position.x > (Constants::SCREEN_WIDTH - Constants::WIDTH_OFFSET));
	}

	void UpdateEnemyHorizontalPosition(EnemyState& enemy, GameProgress& progress, float dt)
	{
		enemy.position.x += (progress.baseEnemySpeed * dt) * progress.enemyDirection;
	}

	void UpdateEnemyVerticalPosition(EnemyState& enemy, GameProgress& progress, float dt)
	{
		enemy.position.y += (progress.dropDistance * dt);
	}
}