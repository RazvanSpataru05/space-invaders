#include "pch.h"
#include "Utils/PlayerUtils.h"

namespace GameLogic::Utils
{
	float ProjectileVelocity(float velocity, GameLogic::ProjectileOwner owner)
	{
		return owner == GameLogic::ProjectileOwner::PLAYER ? -velocity : velocity;
	}

	void UpdateProjectilesPosition(GameLogic::ProjectileState& projectile, float dt)
	{
		projectile.position.y += (projectile.velocity * dt);
	}
}