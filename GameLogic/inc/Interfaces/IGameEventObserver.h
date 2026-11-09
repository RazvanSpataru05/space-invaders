#pragma once

// Observer side of the Observer pattern. Implemented by GameUI.

#include "Structs/GameEvent.h"

namespace GameLogic
{
    class IGameEventObserver
    {
    public:
        virtual ~IGameEventObserver() = default;

        virtual void OnGameEvent(const GameEvent& event) = 0;
    };
}
