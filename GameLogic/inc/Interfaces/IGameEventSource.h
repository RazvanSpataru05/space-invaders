#pragma once

namespace GameLogic
{
    class IGameEventObserver;

    class IGameEventSource
    {
    public:
        virtual ~IGameEventSource() = default;

        virtual void Subscribe(IGameEventObserver* observer) = 0;
        virtual void Unsubscribe(IGameEventObserver* observer) = 0;
    };
}
