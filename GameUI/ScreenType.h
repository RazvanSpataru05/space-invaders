#pragma once

#include <cstdint>

// Screens the UI can show (ClassDiagram.md, section 4).
// Belongs to the UI project only - it is not part of the GameLogic API.

enum class ScreenType : uint8_t
{
    MAIN_MENU,
    LEVEL_SELECTION,
    DIFFICULTY_SELECTION,
    GAMEPLAY,
    GAME_OVER,
    VICTORY
};
