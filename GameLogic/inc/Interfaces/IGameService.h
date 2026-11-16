#pragma once

#include "Enums/LevelID.h"
#include "Enums/Direction.h"
#include "Enums/GameState.h"

#include "Interfaces/IGameEventSource.h"

#include "Structs/GameProgress.h"
#include "Structs/PlayerState.h"
#include "Structs/EnemyState.h"
#include "Structs/ProjectileState.h"

#include <span>

namespace GameLogic
{
    class IGameService : public IGameEventSource
    {
    public:
        ~IGameService() override = default;

        virtual void StartLevel() noexcept = 0;
        virtual void Update(float deltaTime) noexcept = 0;
        virtual void MovePlayer(Direction direction) noexcept = 0;
        virtual void Shoot() noexcept = 0;
        virtual void Reset() noexcept = 0;

        [[nodiscard]] virtual GameProgress GetGameProgress()                            const noexcept = 0;
        [[nodiscard]] virtual PlayerState GetPlayerState()                              const noexcept = 0;
        [[nodiscard]] virtual std::span<const EnemyState> GetEnemiesState()             const noexcept = 0;
        [[nodiscard]] virtual std::span<const ProjectileState> GetProjectilesState()    const noexcept = 0;

        [[nodiscard]] virtual size_t  GetLevel()                                        const noexcept = 0;
        [[nodiscard]] virtual size_t& GetLevel()                                        noexcept = 0;
    };
}
