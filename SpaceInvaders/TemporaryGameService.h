#pragma once

// TEMPORARY stand-in for GameSession so the UI can run before GameLogic is implemented.
// When GameSession exists: use it in Main.cpp and delete this file (and remove it from the project).

#include <algorithm>
#include <vector>

#include "IGameEventObserver.h"
#include "IGameService.h"

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

    void StartGame(GameLogic::LevelId level, GameLogic::Difficulty /*difficulty*/) override
    {
        m_snapshot = {};
        m_snapshot.currentLevel = level;
        m_snapshot.gameState = GameLogic::GameState::PLAYING;
    }

    void Update(float /*deltaTime*/) override {}
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
    std::vector<GameLogic::IGameEventObserver*> m_observers;
    GameLogic::GameSnapshot m_snapshot{};
};
