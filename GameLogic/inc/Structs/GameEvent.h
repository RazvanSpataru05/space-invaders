#pragma once

#include "Enums/GameEventType.h"

namespace GameLogic
{
	struct GameEvent
	{
		GameEventType type{};
		int entityId{ -1 }; // -1 when the event is not tied to a specific entity
		int value{};
	};
}