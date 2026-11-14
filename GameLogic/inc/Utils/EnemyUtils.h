#pragma once

#include "Enums/EnemyType.h"

#include "Structs/EnemyState.h"
#include "Structs/GameProgress.h"

#include "Utils/Constants.h"

namespace GameLogic::Utils
{

	constexpr uint16_t GetEnemyHealth(EnemyType type)
	{
		switch (type)
		{
		default:
		case EnemyType::NORMAL:		return Constants::NORMAL_ENEMY_HEALTH;
		case EnemyType::ARMORED:	return Constants::ARMORED_ENEMY_HEALTH;
		case EnemyType::SHOOTER:	return Constants::SHOOTER_ENEMY_HEALTH;
		case EnemyType::BOSS:		return Constants::BOSS_ENEMY_HEALTH;
		}
	}

	bool CheckEdge(const EnemyState& enemy, float direction);

	void UpdateEnemyHorizontalPosition(EnemyState& enemy, const GameProgress& progress, float dt);
	void UpdateEnemyVerticalPosition(EnemyState& enemy, const GameProgress& progress, float dt);
}
