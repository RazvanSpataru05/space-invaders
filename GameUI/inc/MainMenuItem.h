#pragma once

#include <cstdint>

enum class MainMenuItem : uint8_t
{
    PLAY,
    GAME_VERDICTS, // TEMPORARY: previews the Game Over / Victory screens until GameSession exists
    EXIT
};
