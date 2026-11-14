#pragma once

#include "Enums/ProjectileOwner.h"

#include "Structs/Vector2.h"

namespace GameLogic
{
	struct ProjectileState
	{
		Vector2 position{};
		float velocity{};
		ProjectileOwner owner{};
		uint8_t damage{};

		ProjectileState(Vector2 pos, float v, ProjectileOwner o, uint8_t d) :
			position{ pos }, velocity{ v }, owner{ o }, damage{ d } {
		}
	};
}