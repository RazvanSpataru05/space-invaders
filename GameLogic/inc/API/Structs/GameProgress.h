#pragma once

#include "Enums/EnemyType.h"

#include "API/Structs/GameConfig.h"

#include "Utils/Constants.h"

namespace GameLogic
{
    struct GameProgress
    {
        uint32_t score{};
        float elapsedTime{};
        float playerShootCooldown{};
        float enemyShootCooldown{};

        float enemyDirection{};
        float baseEnemySpeed{};
        float dropDistance{};

        GameProgress(const GameConfig& config)
        {
            enemyDirection = Utils::Constants::INITIAL_ENEMY_DIRECTION;
            baseEnemySpeed = config.enemyMovementSpeed;
            dropDistance = config.enemyDropDistance;
        }

        void ChangeDirection()
        {
            enemyDirection *= -1;
        }

        void Reset(const GameConfig& config)
        {
            enemyDirection = Utils::Constants::INITIAL_ENEMY_DIRECTION;
            baseEnemySpeed = config.enemyMovementSpeed;
            dropDistance = config.enemyDropDistance;

            score = 0u;
            elapsedTime = 0.0f;
            playerShootCooldown = 0.0f;
            enemyShootCooldown = 0.0f;

        }
    };
}