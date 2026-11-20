#pragma once

#include "API/Structs/Vector2.h"

#include "API/Utils/GameConstants.h"

#include <cstdint>
#include <array>

namespace GameLogic
{
	struct WallState
	{
		Vector2 position{};
		std::array<bool, Utils::Constants::WALL_ROWS * Utils::Constants::WALL_COLS> cells{};

		WallState() = default;
		WallState(Vector2 p) : position{ p }
		{
			cells.fill(true);
		}
	};
}