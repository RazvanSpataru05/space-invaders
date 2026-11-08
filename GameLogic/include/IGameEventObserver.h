#pragma once

// Observer side of the Observer pattern. Implemented by GameUI.

#include "GameTypes.h"

namespace GameLogic
{
    class IGameEventObserver
    {
    public:
        virtual ~IGameEventObserver() = default;

        virtual void OnGameEvent(const GameEvent& event) = 0;
    };
}
