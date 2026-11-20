#pragma once

#include "API/Structs/Vector2.h"

#include <cstdint>

namespace GameLogic
{
	struct BossState
	{
		Vector2 position{};
		float direction{};
		int16_t health{};
		bool isVisible{};

		BossState(Vector2 p, int16_t h) :
			health{ h }, position{ p }, direction{ 1.0f } {}
		int16_t maxHealth{};	// health at spawn; lets the UI show a fraction

		BossState(int16_t h) :
			health{ h }, maxHealth{ h }, direction{ 1.0f }
		{
			position = Utils::Constants::BOSS_SPAWN_POSITION;
		}

		void ChangeDirection()
		{
			direction *= -1.0f;
		}
	};
}