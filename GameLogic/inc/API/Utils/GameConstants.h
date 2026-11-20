#pragma once

#include <cstddef>
#include <cstdint>

namespace GameLogic::Utils::Constants
{
	// World
	inline constexpr float SCREEN_WIDTH{ 960.0f };
	inline constexpr float SCREEN_HEIGHT{ 720.0f };

	// Entity sizes
	inline constexpr float SHIP_WIDTH{ 40.0f };
	inline constexpr float POWER_UP_WIDTH{ 30.0f };
	inline constexpr float UFO_WIDTH{ 50.0f };
	inline constexpr float BOSS_WIDTH{ 150.0f };
	inline constexpr float PROJECTILE_WIDTH{ 2.0f };
	inline constexpr float PROJECTILE_HEIGHT{ 10.0f };

	// Walls
	inline constexpr float CELL_SIZE{ 5.0f };
	inline constexpr size_t WALL_ROWS{ 16 };
	inline constexpr size_t WALL_COLS{ 22 };

	// Gameplay
	inline constexpr int MAX_LIVES{ 3 };
	inline constexpr size_t LEVEL_COUNT{ 3 };
	inline constexpr size_t ENEMY_TYPES{ 3 };
	inline constexpr uint8_t POWER_UP_TYPES{ 4u };
}