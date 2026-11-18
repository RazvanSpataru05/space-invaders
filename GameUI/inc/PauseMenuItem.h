#pragma once

#include <cstdint>

// Options shown on the pause screen, in display order.
enum class PauseMenuItem : uint8_t
{
    RESUME,
    RETRY,
    MAIN_MENU
};
