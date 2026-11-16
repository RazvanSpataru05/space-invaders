#pragma once

#include "Enums/LevelID.h"

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

        GameProgress()
        {
            enemyDirection = Utils::Constants::INITIAL_ENEMY_DIRECTION;
            baseEnemySpeed = Utils::Constants::INITIAL_ENEMY_SPEED;
            dropDistance   = Utils::Constants::INITIAL_DROP_DISTANCE;
        }

        void ChangeDirection()
        {
            enemyDirection *= -1;
        }

        void Reset()
        {
            enemyDirection = Utils::Constants::INITIAL_ENEMY_DIRECTION;
            baseEnemySpeed = Utils::Constants::INITIAL_ENEMY_SPEED;
            dropDistance   = Utils::Constants::INITIAL_DROP_DISTANCE;

            score = 0u;
            elapsedTime = 0.0f;
            shootCoodldown = 0.0f;

        }
    };
}