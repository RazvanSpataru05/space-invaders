#pragma once

#include "Enums/EventType.h"

namespace GameLogic
{
    class IGameEventObserver
    {
    public:
        virtual ~IGameEventObserver() = default;

        virtual void OnGameEvent(EventType event) = 0;
    };
}
    