#pragma once

#include "API/Structs/Vector2.h"

#include <cstdint>

namespace GameLogic::Utils::Constants
{
	constexpr float INITIAL_ENEMY_DIRECTION{ 1.0f }; // 1: enemies move to right, -1: enemies move to the left

	constexpr float PROJECTILE_WIDTH{ 2.0f };
	constexpr float PROJECTILE_HEIGHT{ 10.0f };

	constexpr float SHIP_WIDTH{ 40.0f };
	constexpr float UFO_WIDTH{ 50.0f };

	constexpr float SCREEN_HEIGHT{ 720.0f };
	constexpr float SCREEN_WIDTH{ 960.0f };
	constexpr float WIDTH_OFFSET{ 40.0f };

	const Vector2 PLAYER_SPAWN_POSITION{ Vector2(480.0f, 615.0f) };
	const Vector2 UFO_LEFT_SPAWN_POSITION{ Vector2(0.0f, 75.0f) };
	const Vector2 UFO_RIGHT_SPAWN_POSITION{ Vector2(960.0f, 75.0f) };

	const Vector2 BOSS_SPAWN_POSITION{ Vector2(320.0f, 80.0f) };
}