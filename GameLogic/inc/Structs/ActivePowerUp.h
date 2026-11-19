#pragma once

#include "Enums/PowerUpType.h"

namespace GameLogic
{
	struct ActivePowerUp
	{
		float duration{};
		PowerUpType type{};

		ActivePowerUp(float d, PowerUpType t) :
			duration{ d }, type{ t } {}
	};
}