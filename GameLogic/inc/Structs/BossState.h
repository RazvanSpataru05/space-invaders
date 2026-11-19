#pragma once

#include "API/Structs/Vector2.h"

#include "Utils/Constants.h"

#include <cstdint>

namespace GameLogic
{
	struct BossState
	{
		Vector2 position{};
		float direction{};
		int16_t health{};

		BossState(int16_t h) :
			health{ h }, direction{ 1.0f }
		{
			position = Utils::Constants::BOSS_SPAWN_POSITION;
		}

		void ChangeDirection()
		{
			direction *= -1.0f;
		}
	};
}