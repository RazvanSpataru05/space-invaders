#pragma once

#include "Enums/EnemyType.h"

#include "Structs/Vector2.h"

#include <cstdint>

namespace GameLogic
{
	struct EnemyState
	{
		Vector2 position{};
		uint16_t health{};
		EnemyType type{};
		bool isAlive{};

		EnemyState(Vector2 pos, uint16_t h, EnemyType t) :
			position{ pos }, health{ h }, type{ t }, isAlive{ true }
		{}
	};
}