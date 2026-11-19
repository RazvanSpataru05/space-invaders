#pragma once

#include "Enums/PowerUpType.h"

#include "API/Structs/Vector2.h"

namespace GameLogic
{
	struct PowerUpPickup
	{
		Vector2 position{};
		PowerUpType type{};

		PowerUpPickup(Vector2 p, PowerUpType t) :
			position{ p }, type{ t } {}
	};
}
