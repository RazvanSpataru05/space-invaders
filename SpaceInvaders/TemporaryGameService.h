#pragma once

// TEMPORARY stand-in for GameSession so the UI can run before GameLogic is implemented.
// When GameSession exists: use it in Main.cpp and delete this file (and remove it from the project).

#include "Interfaces/IGameEventObserver.h"
#include "Interfaces/IGameService.h"

#include <algorithm>
#include <vector>

class TemporaryGameService final : public GameLogic::IGameService
{
public:

    void Subscribe(GameLogic::IGameEventObserver* observer) override
    {
        if (observer && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end())
        {
            m_observers.push_back(observer);
        }
    }

    void Unsubscribe(GameLogic::IGameEventObserver* observer) override
    {
        std::erase(m_observers, observer);
    }

    void StartGame(GameLogic::LevelID level) override
    {
    }

    void Update(float deltaTime) override {}
    void MovePlayer(GameLogic::Direction direction) override {}
    void Shoot() override {}

private:
    std::vector<GameLogic::IGameEventObserver*> m_observers;
};
