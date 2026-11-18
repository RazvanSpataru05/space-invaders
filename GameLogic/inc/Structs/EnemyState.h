#pragma once

#include "Enums/EnemyType.h"

#include "API/Structs/Vector2.h"

#include <cstdint>

namespace GameLogic
{
	struct EnemyState
	{
		Vector2 position{};
		int16_t health{};
		EnemyType type{};

		EnemyState(Vector2 pos, int16_t h, EnemyType t) :
			position{ pos }, health{ h }, type{ t }
		{}
	};
}