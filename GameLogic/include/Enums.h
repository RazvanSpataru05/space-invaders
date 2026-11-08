#pragma once

// GameLogic public API - enums (see ClassDiagram.md, section 4).
// Minimal version written alongside GameUI; the GameLogic owner may extend it.
// All enums are scoped (enum class) so their values cannot collide with
// raylib / Windows names such as GAME_OVER or LEFT.

namespace GameLogic
{
    enum class Difficulty
    {
        EASY,
        MEDIUM,
        HARD
    };

    enum class LevelId
    {
        LEVEL_1,
        LEVEL_2,
        LEVEL_3
    };

    enum class GameState
    {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER,
        VICTORY
    };

    enum class EnemyType
    {
        NORMAL,
        ARMORED,
        SHOOTER,
        BOSS
    };

    enum class PowerUpType
    {
        EXTRA_LIFE,
        BONUS_SCORE,
        RAPID_FIRE,
        MULTI_SHOT
    };

    enum class ProjectileOwner
    {
        PLAYER,
        ENEMY
    };

    enum class Direction
    {
        LEFT,
        RIGHT
    };

    enum class GameEventType
    {
        SCORE_CHANGED,
        PLAYER_HEALTH_CHANGED,
        PLAYER_DESTROYED,
        ENEMY_DESTROYED,
        POWER_UP_COLLECTED,
        LEVEL_COMPLETED,
        BOSS_SPAWNED,
        GAME_OVER,
        GAME_WON
    };
}
