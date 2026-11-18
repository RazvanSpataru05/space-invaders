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

		UFOState()
		{
			position = GameLogic::Utils::Constants::UFO_SPAWN_POSITION;
		}
	};
}