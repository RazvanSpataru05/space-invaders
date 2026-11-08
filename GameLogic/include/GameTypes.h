#pragma once

// GameLogic public API - data structures shared with the UI
// (see ClassDiagram.md, section 3).
// Minimal version written alongside GameUI; the GameLogic owner may extend it.
// LevelConfig and GameConfig are internal to GameSession and are not declared here.

#include <vector>

#include "Enums.h"

namespace GameLogic
{
    // Own Vector2 inside the GameLogic namespace, so it never collides with
    // raylib's global ::Vector2. The UI converts between the two when drawing.
    struct Vector2
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct PlayerState
    {
        Vector2 position{};
        int health = 0;
        int maxHealth = 0;
        bool isAlive = false;
    };

    struct EnemyState
    {
        int id = 0;
        Vector2 position{};
        int health = 0;
        int maxHealth = 0;
        EnemyType type = EnemyType::NORMAL;
        bool isAlive = false;
    };

    struct ProjectileState
    {
        Vector2 position{};
        Vector2 velocity{};
        ProjectileOwner owner = ProjectileOwner::PLAYER;
        int damage = 0;
    };

    struct PowerUpState
    {
        Vector2 position{};
        PowerUpType type = PowerUpType::EXTRA_LIFE;
        bool isActive = false;
        float remainingDuration = 0.0f;
    };

    // Read-only picture of the current game state, used by the UI for drawing.
    struct GameSnapshot
    {
        PlayerState player{};
        std::vector<EnemyState> enemies;
        std::vector<ProjectileState> projectiles;
        std::vector<PowerUpState> powerUps;
        int score = 0;
        LevelId currentLevel = LevelId::LEVEL_1;
        GameState gameState = GameState::MENU;
    };

    // Payload delivered to observers (Observer pattern).
    struct GameEvent
    {
        GameEventType type = GameEventType::SCORE_CHANGED;
        int entityId = -1; // -1 when the event is not tied to a specific entity
        int value = 0;
    };
}
