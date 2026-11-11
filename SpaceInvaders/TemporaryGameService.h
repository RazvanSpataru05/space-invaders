#pragma once

// TEMPORARY stand-in for GameSession so the UI can run before GameLogic is implemented.
// When GameSession exists: use it in Main.cpp and delete this file (and remove it from the project).
// Demo behaviour during gameplay: score rises, one life is lost every 2 seconds, and at
// 0 lives GAME_OVER is sent to the observers, exactly like the real session will.

#include "Enums/GameEventType.h"
#include "Interfaces/IGameEventObserver.h"
#include "Interfaces/IGameService.h"
#include "Structs/GameEvent.h"

#include <algorithm>
#include <cstdint>
#include <vector>

class TemporaryGameService final : public GameLogic::IGameService
{
public:
    void Subscribe(GameLogic::IGameEventObserver* observer) override
    {
        if (observer != nullptr && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end())
        {
            m_observers.push_back(observer);
        }
    }

    void Unsubscribe(GameLogic::IGameEventObserver* observer) override
    {
        std::erase(m_observers, observer);
    }

    void StartGame(GameLogic::LevelID level, GameLogic::Difficulty /*difficulty*/) override
    {
        m_snapshot = GameLogic::GameSnapshot{};
        m_snapshot.currentLevel = level;
        m_snapshot.gameState = GameLogic::GameState::PLAYING;
        m_snapshot.player.health = kStartingLives;
        m_snapshot.player.maxHealth = kStartingLives;
        m_snapshot.player.isAlive = true;
        m_lifeTimer = 0.0f;
        m_scoreAccumulator = 0.0f;
    }

    void Update(float deltaTime) override
    {
        if (m_snapshot.gameState != GameLogic::GameState::PLAYING)
        {
            return;
        }

        m_scoreAccumulator += deltaTime * kScorePerSecond;
        m_snapshot.score = static_cast<uint32_t>(m_scoreAccumulator);

        m_lifeTimer += deltaTime;
        if (m_lifeTimer < kSecondsPerLifeLost)
        {
            return;
        }

        m_lifeTimer -= kSecondsPerLifeLost;
        --m_snapshot.player.health;

        if (m_snapshot.player.health <= 0)
        {
            m_snapshot.player.isAlive = false;
            m_snapshot.gameState = GameLogic::GameState::GAME_OVER;
            Notify(GameLogic::GameEventType::GAME_OVER);
        }
    }

    void MovePlayer(GameLogic::Direction /*direction*/) override {}
    void Shoot() override {}

    [[nodiscard]] GameLogic::GameSnapshot GetGameSnapshot() const override
    {
        return m_snapshot;
    }

    [[nodiscard]] GameLogic::GameState GetGameState() const override
    {
        return m_snapshot.gameState;
    }

private:
    static constexpr int kStartingLives = 3;
    static constexpr float kSecondsPerLifeLost = 2.0f;
    static constexpr float kScorePerSecond = 350.0f;

    void Notify(GameLogic::GameEventType type)
    {
        const GameLogic::GameEvent event{ .type = type };
        // Copy: an observer may unsubscribe while being notified.
        const std::vector<GameLogic::IGameEventObserver*> observers = m_observers;
        for (GameLogic::IGameEventObserver* observer : observers)
        {
            observer->OnGameEvent(event);
        }
    }

    std::vector<GameLogic::IGameEventObserver*> m_observers;
    GameLogic::GameSnapshot m_snapshot{};
    float m_lifeTimer = 0.0f;
    float m_scoreAccumulator = 0.0f;
};
