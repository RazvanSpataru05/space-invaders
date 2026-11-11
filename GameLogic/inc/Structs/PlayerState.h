#pragma once

#include "Structs/Vector2.h"

namespace GameLogic
{
	struct PlayerState
	{
		Vector2 position{};
		uint8_t livesCount{};
		bool isAlive{};

		PlayerState() = default;
		PlayerState(Vector2 pos, uint8_t lives) :
			position{ pos }, livesCount{ lives } {}
	};
}