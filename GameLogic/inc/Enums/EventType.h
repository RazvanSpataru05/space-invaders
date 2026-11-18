#pragma once

#include <cstdint>

namespace GameLogic
{
    enum class EventType : uint8_t
    {
        LEVEL_COMPLETED,
        GAME_OVER,
        BOSS_SPAWNED,

        PLAYER_SHOT,
        PLAYER_HIT,
        ENEMY_HIT,
        ENEMY_DESTROYED,

        UFO_APPROACHING,         // spawn conditions hold, UFO arrives in UFO_WARNING_TIME seconds
        UFO_APPROACH_CANCELLED,  // conditions stopped holding before the UFO spawned
        UFO_SPAWNED,
        UFO_LEAVING,             // UFO is within UFO_LEAVING_DISTANCE of the edge it exits through
        UFO_DESPAWNED
    };
}