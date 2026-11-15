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

        LevelID currentLevel{};

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

        void Reset(LevelID level)
        {
            enemyDirection = Utils::Constants::INITIAL_ENEMY_DIRECTION;
            baseEnemySpeed = Utils::Constants::INITIAL_ENEMY_SPEED;
            dropDistance   = Utils::Constants::INITIAL_DROP_DISTANCE;

            currentLevel = level;
            score = 0u;
            elapsedTime = 0.0f;
            shootCoodldown = 0.0f;

        }
    };
}