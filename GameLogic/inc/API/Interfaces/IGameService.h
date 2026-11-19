#pragma once

#include "API/Enums/Direction.h"
#include "API/Enums/GameState.h"

#include "API/Interfaces/IGameEventSource.h"

#include "API/Structs/GameProgress.h"
#include "Structs/PlayerState.h"
#include "Structs/EnemyState.h"
#include "Structs/ProjectileState.h"
#include "Structs/UFOState.h"
#include "Structs/BossState.h"
#include "Structs/PowerUpPickup.h"

#include <span>
#include <optional>

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

        [[nodiscard]] virtual PlayerState GetPlayerState()                                    const noexcept = 0;
        [[nodiscard]] virtual const std::optional<BossState>& GetBossState()                  const noexcept = 0;
        [[nodiscard]] virtual const std::optional<UFOState>& GetUFOState()                    const noexcept = 0;
        [[nodiscard]] virtual std::span<const EnemyState> GetEnemiesState()                   const noexcept = 0;
        [[nodiscard]] virtual std::span<const ProjectileState> GetPlayerProjectilesState()    const noexcept = 0;
        [[nodiscard]] virtual std::span<const ProjectileState> GetEnemyProjectilesState()     const noexcept = 0;
        [[nodiscard]] virtual std::span<const PowerUpPickup> GetPowerUpPickups()              const noexcept = 0;
        [[nodiscard]] virtual size_t& GetLevel()                                              noexcept = 0;
        [[nodiscard]] virtual uint32_t GetScore()                                             const noexcept = 0;
    };
}
