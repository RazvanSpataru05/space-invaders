#pragma once

#include "API/Structs/Vector2.h"

#include <cstdint>

namespace GameLogic
{
	struct ProjectileState
	{
		Vector2 position{};
		float velocity{};
		uint8_t damage{};

		ProjectileState(Vector2 pos, float v, uint8_t d) :
			position{ pos }, velocity{ v }, damage{ d } {
		}
	};
}