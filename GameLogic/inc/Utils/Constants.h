#pragma once

#include "API/Utils/GameConstants.h"
#include "API/Structs/Vector2.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace GameLogic::Utils::Constants
{
	inline constexpr float INITIAL_ENEMY_DIRECTION{ 1.0f }; // 1: enemies move to right, -1: enemies move to the left

	inline constexpr int TWO_ROWS_COUNT{ 12 };

	inline constexpr float WIDTH_OFFSET{ 40.0f };

	inline constexpr std::array<std::string_view, 16> WALL_TEMPLATE{
	"....##############....",
	"...################...",
	"..##################..",
	".####################.",
	"######################",
	"######################",
	"######################",
	"######################",
	"######################",
	"######################",
	"######################",
	"######################",
	"#######........#######",
	"######..........######",
	"#####............#####",
	"#####............#####",
	};

	inline constexpr size_t WALL_COUNT{ 4 };
	inline constexpr float WALLS_GAP{ 120.0f };

	inline const Vector2 PLAYER_SPAWN_POSITION{ 480.0f, 660.0f };
	inline const Vector2 UFO_LEFT_SPAWN_POSITION{ 0.0f, 75.0f };
	inline const Vector2 UFO_RIGHT_SPAWN_POSITION{ 960.0f, 75.0f };
	inline const Vector2 FIRST_WALL_POSITION{ 80.0f, 560.0f };

	inline const float UFO_WARNING_TIME{ 3.0f };
	inline const float UFO_LEAVING_DISTANCE{ 150.0f };

	inline const Vector2 BOSS_SPAWN_POSITION{ 405.0f, -125.0f };
	inline const Vector2 BOSS_DESIRED_LOCATION{ 405.0f, 80.0f };
}