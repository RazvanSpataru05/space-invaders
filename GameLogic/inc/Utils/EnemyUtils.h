#pragma once

#include "API/Structs/GameProgress.h"
#include "API/Structs/GameConfig.h"

#include "Enums/EnemyType.h"

#include "Structs/EnemyState.h"
#include "Structs/Bounds.h"

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
		}
	}

	bool CheckEdge(const EnemyState& enemy, float direction);

	void UpdateEnemyHorizontalPosition(EnemyState& enemy, const GameProgress& progress, size_t remainingEnemies, size_t totalEnemies, float dt);
	void UpdateEnemyVerticalPosition(EnemyState& enemy, const GameProgress& progress, float dt);

	std::optional<std::uniform_int_distribution<size_t>> GetUniformEnemyDistribution(int max);

	bool AABBCollision(float xStartObj, float xEndObj, float yStartObj, float yEndObj,
		float xProj, float yProj);

	bool IsValidIndex(size_t row, size_t col, size_t rowCount, size_t colCount);
}
