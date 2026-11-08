#pragma once

// Facade through which the UI talks to the game logic. Implemented by GameSession.
// Subscribe / Unsubscribe are inherited from IGameEventSource, so GameSession only
// needs to derive from IGameService (deriving from both would create a diamond).
// Minimal version written alongside GameUI; the GameLogic owner may extend it.

#include "Enums.h"
#include "GameTypes.h"
#include "IGameEventSource.h"

namespace GameLogic
{
    class IGameService : public IGameEventSource
    {
    public:
        ~IGameService() override = default;

        virtual void StartGame(LevelId level, Difficulty difficulty) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void MovePlayer(Direction direction) = 0;
        virtual void Shoot() = 0;

        [[nodiscard]] virtual GameSnapshot GetGameSnapshot() const = 0;
        [[nodiscard]] virtual GameState GetGameState() const = 0;
    };
}
