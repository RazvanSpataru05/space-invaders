#pragma once

namespace GameLogic
{
	struct Vector2
	{
		float x{};
		float y{};

		Vector2() = default;
		Vector2(float x, float y) :
			x{ x }, y{ y } {}
	};
}


