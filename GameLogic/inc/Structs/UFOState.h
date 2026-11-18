#pragma once

#include "API/Structs/Vector2.h"

#include "Utils/Constants.h"

#include <array>
#include <cstdint>

namespace GameLogic
{
	struct UFOState
	{
		const std::array <uint16_t, 16> scores
		{
			100, 50, 50, 100,
			150, 100, 100, 50,
			300, 100, 100, 100,
			50, 150, 100, 50
		};
		Vector2 position{};
		float direction{};

		UFOState(float d)
		{
			direction = d;
			if (direction > 0) position = Utils::Constants::UFO_LEFT_SPAWN_POSITION;
			else               position = Utils::Constants::UFO_RIGHT_SPAWN_POSITION;
		}
	};
}