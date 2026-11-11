#pragma once

#include "Enums/EnemyType.h"

#include "Structs/Vector2.h"

namespace GameLogic
{
	struct EnemyConfig
	{
		Vector2 position{};
		uint8_t health{};
		EnemyType type{};
	};
}