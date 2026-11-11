#include "pch.h"
#include "GameSession.h"

GameSession::GameSession()
{

}

void GameSession::Subscribe(GameLogic::IGameEventObserver* observer)
{
	if (observer && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end())
	{
		m_observers.emplace_back(observer);
	}
}

void GameSession::Unsubscribe(GameLogic::IGameEventObserver* observer)
{
	std::erase(m_observers, observer);
}

void GameSession::SpawnBoss() noexcept
{
	if (m_progress.currentLevel != GameLogic::LevelID::LEVEL_3) return;


}
