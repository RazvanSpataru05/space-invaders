#pragma once

#include "API/Structs/Vector2.h"

#include "Utils/Constants.h"

struct Bounds
{
	float x{}, y{}, width{}, height{};

	Bounds(float x, float y, float w, float h) :
		x{ x }, y{ y }, width{ w }, height{ h } {
	}

	bool Overlaps(const Bounds& other) const noexcept
	{
		return x < other.x + other.width
			&& x + width > other.x
			&& y < other.y + other.height
			&& y + height > other.y;
	}

	bool Overlaps(GameLogic::Vector2 p) const noexcept
	{
		return x < p.x + GameLogic::Utils::Constants::PROJECTILE_WIDTH
			&& x + width > p.x
			&& y < p.y + GameLogic::Utils::Constants::PROJECTILE_HEIGHT
			&& y + height > p.y;
	}
};

inline Bounds GetBounds(GameLogic::Vector2 position, float size)
{
	return { position.x, position.y, size, size };
}

