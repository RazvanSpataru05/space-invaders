#pragma once

#include "Enums/ProjectileOwner.h"

#include "Structs/Vector2.h"


namespace GameLogic
{
    struct ProjectileState
    {
        Vector2 position{};
        Vector2 velocity{};
        ProjectileOwner owner{};
        int damage{};
    };
}