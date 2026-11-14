#pragma once

#include <cstdint>

namespace GameLogic::Utils::Constants
{
	constexpr uint16_t NORMAL_ENEMY_HEALTH{ 100u };
	constexpr uint16_t ARMORED_ENEMY_HEALTH{ 250u };
	constexpr uint16_t SHOOTER_ENEMY_HEALTH{ 150u };
	constexpr uint16_t BOSS_ENEMY_HEALTH{ 1000u };

	constexpr uint16_t NORMAL_ENEMY_SCORE{ 30u };
	constexpr uint16_t ARMORED_ENEMY_SCORE{ 50u };
	constexpr uint16_t SHOOTER_ENEMY_SCORE{ 80u };
	constexpr uint16_t BOSS_ENEMY_SCORE{ 500u };

	constexpr uint8_t PLAYER_INITIAL_LIVES{ 3u };
	constexpr uint8_t PROJECTILE_INITIAL_DAMAGE{ 50u };

	constexpr float INITIAL_ENEMY_DIRECTION{ 1.0f }; // 1: enemies move to right, -1: enemies move to the left
	constexpr float INITIAL_ENEMY_SPEED{ 100.0f };
	constexpr float INITIAL_DROP_DISTANCE{ 25.0f };

	constexpr float SCREEN_HEIGHT{ 720.0f };
	constexpr float SCREEN_WIDTH{ 960.0f };
	constexpr float WIDTH_OFFSET{ 40.0f };

	const Vector2 PLAYER_INITIAL_POSITION{ Vector2(480.0f, 680.0f) };
	const Vector2 BOSS_POSITION{ Vector2(320.0f, 80.0f) };
}