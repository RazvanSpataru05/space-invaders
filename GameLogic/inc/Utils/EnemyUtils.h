#pragma once

#include "Enums/EnemyType.h"

#include "Structs/EnemyState.h"
#include "Structs/GameProgress.h"
#include "Structs/GameConfig.h"

#include "Utils/Constants.h"

#include <optional>
#include <random>

namespace GameLogic::Utils
{
	constexpr uint16_t GetEnemyHealth(EnemyType type, const GameConfig& config)
	{
		switch (type)
		{
		default:
		case EnemyType::NORMAL:		return config.normalEnemyHealth;
		case EnemyType::ARMORED:	return config.armoredEnemyHealth;
		case EnemyType::SHOOTER:	return config.shooterEnemyHealth;
		case EnemyType::BOSS:		return config.bossEnemyHealth;
		}
	}

	bool CheckEdge(const EnemyState& enemy, float direction);

	void UpdateEnemyHorizontalPosition(EnemyState& enemy, const GameProgress& progress, float dt);
	void UpdateEnemyVerticalPosition(EnemyState& enemy, const GameProgress& progress, float dt);

	std::optional<std::uniform_int_distribution<size_t>> GetUniformEnemyDistribution(size_t enemyCount);

	bool AABBCollision(float xStartObj, float xEndObj, float yEndObj,
		float xProj, float yProj);
}
