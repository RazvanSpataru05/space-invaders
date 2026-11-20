#pragma once

#include "Enums/EnemyType.h"

#include "API/Structs/GameConfig.h"

#include "Utils/Constants.h"

namespace GameLogic
{
    struct GameProgress
    {
        uint32_t score{};
        uint32_t projectileCount{};
        float elapsedTime{};
        float playerShootCooldown{};
        float enemyShootCooldown{};
        float ufoSpawnTimer{};
        float playerRespawnTimer{};

        float enemyDirection{};
        float baseEnemySpeed{};
        float dropDistance{};

        float fireRateMultiplier{};

        bool bossDefeated{};
        bool ufoApproaching{};
        bool ufoLeaving{};
        bool multiShot{};
        bool isPlayerRespawning{};

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

            projectileCount = 0u;
            score = 0u;
            elapsedTime = 0.0f;
            playerShootCooldown = 0.0f;
            enemyShootCooldown = 0.0f;
            ufoSpawnTimer = 0.0f;
            playerRespawnTimer = 0.0f;
            fireRateMultiplier = 1.0f;
            bossDefeated = false;
            ufoApproaching = false;
            ufoLeaving = false;
            multiShot = false;
            isPlayerRespawning = false;
        }
    };
}