#pragma once

#include <cstdint>

// Entries of the main menu, in display order.
enum class MainMenuItem : uint8_t
{
    PLAY,
    GAME_VERDICTS, // TEMPORARY: previews the Game Over / Victory screens until GameSession exists
    EXIT
};
