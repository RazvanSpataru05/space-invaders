#include "pch.h"
#include "Utils/PlayerUtils.h"

namespace GameLogic::Utils
{
	void UpdateProjectilesPosition(GameLogic::ProjectileState& projectile, float dt)
	{
		projectile.position.y += (projectile.velocity * dt);
	}
}