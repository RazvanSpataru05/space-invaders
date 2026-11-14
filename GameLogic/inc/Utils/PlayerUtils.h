#pragma once

#include "Enums/ProjectileOwner.h"

#include "Structs/ProjectileState.h"

namespace GameLogic::Utils
{
	float ProjectileVelocity(float velocity, GameLogic::ProjectileOwner owner);
	void UpdateProjectilePosition(GameLogic::ProjectileState& projectile, float dt);
}