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

void GameSession::StartLevel() noexcept
{
	Reset();

	SpawnEnemies();
	m_gameState = GameLogic::GameState::PLAYING;
}

void GameSession::Update(float deltaTime) noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	CheckCollissions();
	CheckLevelCompletion();
	CheckPlayerDefeat();

	UpdateEnemies(deltaTime);
	UpdateProjectile(deltaTime);

	std::cout << "Enemies left: " << m_entities.enemies.size() << std::endl;

	m_progress.elapsedTime += deltaTime;
	m_progress.shootCoodldown += deltaTime;
}

void GameSession::MovePlayer(GameLogic::Direction direction) noexcept
{
	if (m_progress.elapsedTime < GameLogic::Utils::Constants::START_COOLDOWN_SECONDS
		|| m_gameState != GameLogic::GameState::PLAYING) return;

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

size_t& GameSession::GetLevel() noexcept
{
	return m_currentLevel;
}

size_t GameSession::GetLevel() const noexcept
{
	return m_currentLevel;
}

void GameSession::Shoot() noexcept
{
	if (m_progress.elapsedTime < GameLogic::Utils::Constants::START_COOLDOWN_SECONDS
		|| m_progress.shootCoodldown < GameLogic::Utils::Constants::SHOOT_COOLDOWN_SECONDS
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	m_entities.projectiles.emplace_back(
		GameLogic::Vector2(m_entities.player.position.x + GameLogic::Utils::Constants::SHIP_WIDTH * 0.5f, m_entities.player.position.y),
		GameLogic::Utils::ProjectileVelocity(m_gameConfig.projectileSpeed, GameLogic::ProjectileOwner::PLAYER),
		GameLogic::ProjectileOwner::PLAYER,
		GameLogic::Utils::Constants::PROJECTILE_INITIAL_DAMAGE
	);
	m_progress.shootCoodldown = 0.0f;
	std::cout << "projectile count: " << m_entities.projectiles.size() << '\n';
}

void GameSession::Reset() noexcept
{
	m_progress.Reset();
	m_entities.Reset();
}

void GameSession::SpawnEnemies() noexcept
{
	for (const auto& enemyConfig : m_gameConfig.levels[m_currentLevel].enemies)
	{
		m_entities.enemies.emplace_back(enemyConfig.position, enemyConfig.health, enemyConfig.type);
	}
}

void GameSession::UpdateEnemies(float deltaTime)
{
	if (m_progress.elapsedTime < GameLogic::Utils::Constants::START_COOLDOWN_SECONDS
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	UpdateEnemiesHealth();
	UpdateEnemiesPosition(deltaTime);
}

void GameSession::UpdateProjectile(float deltaTime)
{
	if (m_progress.elapsedTime < GameLogic::Utils::Constants::START_COOLDOWN_SECONDS
		|| m_gameState != GameLogic::GameState::PLAYING) return;

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
	if (m_progress.elapsedTime < GameLogic::Utils::Constants::START_COOLDOWN_SECONDS
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	auto it = m_entities.projectiles.begin();
	while (it != m_entities.projectiles.end())
	{
		const GameLogic::Vector2 projectilePos = it->position;

		for (auto& enemy : m_entities.enemies)
		{
			const float xStart = enemy.position.x;
			const float yStart = enemy.position.y;

			const float xEnd = enemy.position.x + GameLogic::Utils::Constants::SHIP_WIDTH;
			const float yEnd = enemy.position.y + GameLogic::Utils::Constants::SHIP_WIDTH;

			if (xStart < projectilePos.x + GameLogic::Utils::Constants::PROJECTILE_WIDTH
				&& xEnd > projectilePos.x
				&& yStart < projectilePos.y + GameLogic::Utils::Constants::PROJECTILE_HEIGHT
				&& yEnd > projectilePos.y)
			{
				std::cout << "hit\n";
				enemy.health = static_cast<int16_t>(std::max(0, enemy.health - GameLogic::Utils::Constants::PROJECTILE_INITIAL_DAMAGE));
				it = m_entities.projectiles.erase(it);
			}
		}

		if (it != m_entities.projectiles.end()) it++;
	}
}

void GameSession::CheckLevelCompletion() noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	if (m_entities.enemies.empty())
	{
		m_gameState = GameLogic::GameState::VICTORY;
		NotifyObservers({ GameLogic::GameEventType::LEVEL_COMPLETED });
	}
}

void GameSession::CheckPlayerDefeat() noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	if (m_entities.player.livesCount == 0u)
	{
		m_gameState = GameLogic::GameState::GAME_OVER;
		NotifyObservers({ GameLogic::GameEventType::GAME_OVER });
	}
}

void GameSession::SpawnBoss() noexcept
{
	if (m_currentLevel != 2) return;

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
	for (const auto& observer : m_observers)
	{
		observer->OnGameEvent(event);
	}
}

void GameSession::UpdateEnemiesHealth()
{
	std::erase_if(m_entities.enemies, [](const GameLogic::EnemyState& e)
		{
			return e.health == 0u;
		});
}

void GameSession::UpdateEnemiesPosition(float deltaTime)
{
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
