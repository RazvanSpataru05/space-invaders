#pragma once

#include "Enums/LevelID.h"
#include "Enums/Direction.h"
#include "Enums/GameState.h"

#include "Interfaces/IGameEventSource.h"

#include "Structs/GameProgress.h"

namespace GameLogic
{
    class IGameService : public IGameEventSource
    {
    public:
        ~IGameService() override = default;

        virtual void StartLevel(LevelID level) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void MovePlayer(Direction direction) = 0;
        virtual void Shoot() = 0;

        // Copy of the score and current level, so the UI never reads live state.
        [[nodiscard]] virtual GameProgress GetGameProgress() const = 0;

    };
}
