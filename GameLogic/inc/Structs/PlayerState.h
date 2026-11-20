#pragma once

#include "API/Structs/Vector2.h"

namespace GameLogic
{
	struct PlayerState
	{
		Vector2 position{};
		uint8_t livesCount{};
		bool isVisible{};

		PlayerState() = default;
		PlayerState(Vector2 pos, uint8_t lives) :
			position{ pos }, livesCount{ lives } {}

		void Reset(Vector2 p, uint8_t lives) 
		{
			livesCount = lives;
			position = p;
			isVisible = true;
		}
	};
}