#include "pch.h"
#include "GameSession.h"

GameSession::GameSession() :
	m_gameConfig{ GameLogic::CreateDefaultConfig() },
	m_gameState{ GameLogic::GameState::PLAYING }
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

void GameSession::StartLevel(GameLogic::LevelID level)
{
	m_progress.Reset(level);
	m_entities.Reset();
	
	SpawnEnemies();
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

void GameSession::Shoot()
{

}

void GameSession::SpawnEnemies() noexcept
{
	size_t level = static_cast<size_t>(m_progress.currentLevel);

	for (const auto& enemyConfig : m_gameConfig.levels[level].enemies)
	{
		m_entities.enemies.emplace_back(enemyConfig.position, enemyConfig.health, enemyConfig.type);
	}
}

void GameSession::SpawnBoss() noexcept
{
	if (m_progress.currentLevel != GameLogic::LevelID::LEVEL_3) return;

	m_entities.enemies.emplace_back(
		GameLogic::Utils::Constants::BOSS_POSITION,
		GameLogic::Utils::Constants::BOSS_ENEMY_HEALTH,
		GameLogic::EnemyType::BOSS);
}
