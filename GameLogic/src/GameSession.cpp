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
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	UpdateEnemies(deltaTime);
	UpdateProjectile(deltaTime);
	m_progress.elapsedTime += deltaTime;

	//std::cout << "Delta time: " << m_progress.elapsedTime << '\n';

}

void GameSession::MovePlayer(GameLogic::Direction direction)
{
	if (m_progress.elapsedTime < 2.0f || m_gameState != GameLogic::GameState::PLAYING) return;

	if (direction == GameLogic::Direction::LEFT)
	{
		m_entities.player.position.x = std::max(m_entities.player.position.x - GameLogic::Utils::Constants::SHIP_SPEED,
												GameLogic::Utils::Constants::WIDTH_OFFSET);
	}
	else
	{
		m_entities.player.position.x = std::min(m_entities.player.position.x + GameLogic::Utils::Constants::SHIP_SPEED,
			GameLogic::Utils::Constants::SCREEN_WIDTH - GameLogic::Utils::Constants::WIDTH_OFFSET - GameLogic::Utils::Constants::SHIP_WIDTH);
	}
	std::cout << "updated position: " << "(" << m_entities.player.position.x << ", " << m_entities.player.position.y << ")\n";
}

GameLogic::GameProgress GameSession::GetGameProgress() const noexcept
{
	return m_progress;
}

GameLogic::PlayerState GameSession::GetPlayerState() const noexcept
{
	return m_entities.player;
}

std::span<const GameLogic::EnemyState> GameSession::GetEnemiesState() const noexcept
{
	return m_entities.enemies;
}

std::span<const GameLogic::ProjectileState> GameSession::GetProjectilesState() const noexcept
{
	return m_entities.projectiles;
}

void GameSession::Shoot()
{
	if (m_progress.elapsedTime < 2.0f || m_gameState != GameLogic::GameState::PLAYING) return;

	m_entities.projectiles.emplace_back(
		GameLogic::Vector2(m_entities.player.position.x + GameLogic::Utils::Constants::SHIP_WIDTH * 0.5f, m_entities.player.position.y),
		GameLogic::Utils::ProjectileVelocity(m_gameConfig.projectileSpeed, GameLogic::ProjectileOwner::PLAYER),
		GameLogic::ProjectileOwner::PLAYER,
		GameLogic::Utils::Constants::PROJECTILE_INITIAL_DAMAGE
	);
	std::cout << "projectile count: " << m_entities.projectiles.size() << '\n';
}

void GameSession::SpawnEnemies() noexcept
{
	size_t level = static_cast<size_t>(m_progress.currentLevel);

	for (const auto& enemyConfig : m_gameConfig.levels[level].enemies)
	{
		m_entities.enemies.emplace_back(enemyConfig.position, enemyConfig.health, enemyConfig.type);
	}
}

void GameSession::UpdateEnemies(float deltaTime)
{
	if (m_progress.elapsedTime < 2.0f) return;

	std::erase_if(m_entities.enemies, [](const GameLogic::EnemyState& e)
		{
			return e.health == 0u;
		});

	bool hitEdge = false;

	for (auto& enemy : m_entities.enemies)
	{
		GameLogic::Utils::UpdateEnemyHorizontalPosition(enemy, m_progress, deltaTime);

		if (GameLogic::Utils::CheckEdge(enemy, m_progress.enemyDirection))
		{
			hitEdge = true;
		}
	}

	if (hitEdge)
	{
		m_progress.ChangeDirection();

		for (auto& enemy : m_entities.enemies)
		{
			GameLogic::Utils::UpdateEnemyVerticalPosition(enemy, m_progress, deltaTime);
		}
	}
}

void GameSession::UpdateProjectile(float deltaTime)
{
	for (auto& projectile : m_entities.projectiles)
	{
		GameLogic::Utils::UpdateProjectilePosition(projectile, deltaTime);
	}

	std::erase_if(m_entities.projectiles, [](const GameLogic::ProjectileState& p) {
				return (p.position.y < 0 || p.position.y > GameLogic::Utils::Constants::SCREEN_HEIGHT);
		});
}

void GameSession::UpdateScore(GameLogic::EnemyType enemyType)
{
	switch (enemyType)
	{
	case GameLogic::EnemyType::NORMAL:		m_progress.score += GameLogic::Utils::Constants::NORMAL_ENEMY_SCORE;	return;
	case GameLogic::EnemyType::ARMORED:		m_progress.score += GameLogic::Utils::Constants::ARMORED_ENEMY_SCORE;	return;
	case GameLogic::EnemyType::SHOOTER:		m_progress.score += GameLogic::Utils::Constants::SHOOTER_ENEMY_SCORE;	return;
	case GameLogic::EnemyType::BOSS:		m_progress.score += GameLogic::Utils::Constants::BOSS_ENEMY_SCORE;		return;
	}
}

void GameSession::CheckCollissions()
{
}

void GameSession::CheckLevelCompletion() const noexcept
{
}

void GameSession::CheckPlayerDefeat() const noexcept
{
}

void GameSession::SpawnBoss() noexcept
{
	if (m_progress.currentLevel != GameLogic::LevelID::LEVEL_3) return;

	m_entities.enemies.emplace_back(
		GameLogic::Utils::Constants::BOSS_POSITION,
		GameLogic::Utils::Constants::BOSS_ENEMY_HEALTH,
		GameLogic::EnemyType::BOSS);
}

void GameSession::GeneratePowerUp()
{

}

void GameSession::CollectPowerUp(const GameLogic::PowerUpState& powerUp)
{
}

void GameSession::ApplyPowerUp(GameLogic::PowerUpType type)
{
}

void GameSession::NotifyObservers(const GameLogic::GameEvent& event)
{
	
}
