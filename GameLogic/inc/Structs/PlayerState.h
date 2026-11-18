#pragma once

#include "API/Structs/Vector2.h"

#include "Utils/Constants.h"

namespace GameLogic
{
	struct PlayerState
	{
		Vector2 position{};
		uint8_t livesCount{};
		bool isAlive{};

		PlayerState() = default;
		PlayerState(Vector2 pos, uint8_t lives) :
			position{ pos }, livesCount{ lives } {}

		void Reset(uint8_t lives)
		{
			livesCount = lives;
			position = Utils::Constants::PLAYER_SPAWN_POSITION;
		}
	};
}