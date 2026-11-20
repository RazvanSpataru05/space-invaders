#pragma once

#include "Structs/ProjectileState.h"

namespace GameLogic::Utils
{
	void UpdateProjectilesPosition(GameLogic::ProjectileState& projectile, float dt);
}