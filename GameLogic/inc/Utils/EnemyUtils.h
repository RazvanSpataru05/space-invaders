#pragma once

#include "Enums/EnemyType.h"

#include "Utils/Constants.h"

namespace GameLogic::Utils
{
	constexpr uint16_t GetEnemyHealth(EnemyType type)
	{
		switch (type)
		{
		default:
		case EnemyType::NORMAL:		return GameLogic::Utils::Constants::NORMAL_ENEMY_HEALTH;
		case EnemyType::ARMORED:	return GameLogic::Utils::Constants::ARMORED_ENEMY_HEALTH;
		case EnemyType::SHOOTER:	return GameLogic::Utils::Constants::SHOOTER_ENEMY_HEALTH;
		case EnemyType::BOSS:		return GameLogic::Utils::Constants::BOSS_ENEMY_HEALTH;
		}
	}
}
