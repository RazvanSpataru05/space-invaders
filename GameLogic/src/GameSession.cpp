#include "pch.h"
#include "GameSession.h"

GameSession::GameSession() :
	m_gameConfig{ GameLogic::CreateDefaultConfig() }
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

void GameSession::StartGame(GameLogic::LevelID level)
{
	m_progress.currentLevel = level;

	SpawnEnemies();
	SpawnBoss();
}

void GameSession::Update(float deltaTime)
{

}

void GameSession::MovePlayer(GameLogic::Direction direction)
{
	if (direction == GameLogic::Direction::LEFT)
	{
		m_entities.player.position.x = std::max(m_entities.player.position.x - 10.0f, GameLogic::kLeftBound);
	}
	else
	{
		m_entities.player.position.x = std::min(m_entities.player.position.x + 10.0f, GameLogic::kRightBound);
	}
}

void GameSession::SpawnBoss() noexcept
{
	if (m_progress.currentLevel != GameLogic::LevelID::LEVEL_3) return;


}
