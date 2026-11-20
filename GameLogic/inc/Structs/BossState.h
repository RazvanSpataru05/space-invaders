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
		int16_t maxHealth{};	// health at spawn; lets the UI show a fraction
		bool isVisible{};

		BossState(Vector2 p, int16_t h) :
			position{ p }, direction{ 1.0f }, health{ h }, maxHealth{ h } {}

		void ChangeDirection()
		{
			direction *= -1.0f;
		}
	};
}