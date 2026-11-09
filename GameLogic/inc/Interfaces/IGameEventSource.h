#pragma once

// Subject side of the Observer pattern.
// Observers are non-owning pointers: an observer must Unsubscribe before it is destroyed.

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
