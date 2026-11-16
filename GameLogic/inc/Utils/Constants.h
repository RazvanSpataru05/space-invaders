#pragma once

#include "Structs/Vector2.h"

#include <cstdint>

namespace GameLogic::Utils::Constants
{
	constexpr float INITIAL_ENEMY_DIRECTION{ 1.0f }; // 1: enemies move to right, -1: enemies move to the left

	constexpr float PROJECTILE_WIDTH{ 2.0f };
	constexpr float PROJECTILE_HEIGHT{ 10.0f };
	constexpr float SHIP_WIDTH{ 40.0f };

	constexpr float SCREEN_HEIGHT{ 720.0f };
	constexpr float SCREEN_WIDTH{ 960.0f };
	constexpr float WIDTH_OFFSET{ 40.0f };

	const Vector2 PLAYER_INITIAL_POSITION{ Vector2(480.0f, 615.0f) };
	const Vector2 BOSS_POSITION{ Vector2(320.0f, 80.0f) };
}