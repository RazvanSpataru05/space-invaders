#pragma once

#include <cstdint>

// One-shot sound effects. The UFO siren is a looping stream handled separately by GameAudio.
enum class SoundCue : uint8_t
{
    MENU_MOVE,
    MENU_CONFIRM,
    PLAYER_SHOT,
    PLAYER_HIT,
    ENEMY_HIT,
    ENEMY_DESTROYED,
};
