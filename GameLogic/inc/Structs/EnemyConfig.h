#pragma once

#include "Enums/EnemyType.h"

#include "Structs/Vector2.h"

namespace GameLogic
{
	struct EnemyConfig
	{
		Vector2 position{};
		uint16_t health{};
		EnemyType type{};
	};
}