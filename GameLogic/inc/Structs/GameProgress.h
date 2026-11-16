#pragma once

#include "Enums/LevelID.h"
#include "Enums/EnemyType.h"

#include "Structs/GameConfig.h"

#include "Utils/Constants.h"

#include <cstdint>

namespace GameLogic
{
    struct GameProgress
    {
        uint32_t score{};
        float elapsedTime{};
        float shootCoodldown{};

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
            shootCoodldown = 0.0f;

        }
    };
}