#pragma once

#include <cstdint>

namespace GameLogic::Utils::Constants
{
	constexpr uint16_t NORMAL_ENEMY_HEALTH{ 100u };
	constexpr uint16_t ARMORED_ENEMY_HEALTH{ 250u };
	constexpr uint16_t SHOOTER_ENEMY_HEALTH{ 150u };
	constexpr uint16_t BOSS_ENEMY_HEALTH{ 1000u };

	constexpr uint8_t PLAYER_INITIAL_LIVES{ 3u };

	const Vector2 PLAYER_INITIAL_POSITION{ Vector2(480.0f, 680.0f) };
	const Vector2 BOSS_POSITION{ Vector2(320.0f, 80.0f) };
}