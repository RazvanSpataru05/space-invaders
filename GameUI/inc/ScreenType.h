#pragma once

#include <cstdint>

enum class ScreenType : uint8_t
{
    MAIN_MENU,
    LEVEL_SELECTION,
    GAMEPLAY,
    PAUSED,
    GAME_OVER,
    VICTORY,
};
