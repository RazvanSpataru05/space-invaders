#pragma once

#include <cstdint>

// Volume envelope state of the UFO siren loop.
enum class UfoSirenPhase : uint8_t
{
    SILENT,
    APPROACHING,    // builds up during the warning window before the spawn
    ACTIVE,         // UFO on screen, full volume
    LEAVING,        // fades with the distance left to the exit edge
    FADING_OUT,     // approach cancelled, short fade to silence
};
