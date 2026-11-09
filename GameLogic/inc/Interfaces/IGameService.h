#pragma once


#include "Enums/Difficulty.h"
#include "Enums/LevelID.h"
#include "Enums/Direction.h"
#include "Enums/GameState.h"

#include "GameTypes.h"
#include "IGameEventSource.h"

namespace GameLogic
{
    class IGameService : public IGameEventSource
    {
    public:
        ~IGameService() override = default;

        virtual void StartGame(LevelID level, Difficulty difficulty) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void MovePlayer(Direction direction) = 0;
        virtual void Shoot() = 0;

        [[nodiscard]] virtual GameSnapshot GetGameSnapshot() const = 0;
        [[nodiscard]] virtual GameState GetGameState() const = 0;
    };
}
