#include "pch.h"
#include "GameSession.h"

GameSession::GameSession() :
	m_config{ GameLogic::CreateDefaultConfig() },
	m_generator{ std::random_device{}() },
	m_progress{ GameLogic::GameProgress(m_config) },
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

	CheckCollisions();
	CheckLevelCompletion();
	CheckPlayerDefeat();

	SpawnEnemyProjectile();
	SpawnUFO();

	UpdateEnemies(deltaTime);
	UpdateProjectiles(deltaTime);
	UpdateUFOPosition(deltaTime);

	m_progress.elapsedTime += deltaTime;

	m_progress.playerShootCooldown += deltaTime;
	m_progress.enemyShootCooldown += deltaTime;
	m_progress.ufoSpawnTimer += deltaTime;
}

void GameSession::MovePlayer(GameLogic::Direction direction) noexcept
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	if (direction == GameLogic::Direction::LEFT)
	{
		m_entities.player.position.x = std::max(m_entities.player.position.x - m_config.playerMovementSpeed,
			GameLogic::Utils::Constants::WIDTH_OFFSET);
	}
	else
	{
		m_entities.player.position.x = std::min(m_entities.player.position.x + m_config.playerMovementSpeed,
			GameLogic::Utils::Constants::SCREEN_WIDTH - GameLogic::Utils::Constants::WIDTH_OFFSET
			- GameLogic::Utils::Constants::SHIP_WIDTH);
	}
}

GameLogic::PlayerState GameSession::GetPlayerState() const noexcept
{
	return m_entities.player;
}

GameLogic::Vector2 GameSession::GetBossPosition() const noexcept
{
	return m_entities.enemies[0].position;
}

std::optional<GameLogic::UFOState> GameSession::GetUFOState() const noexcept
{
	return m_entities.ufo;
}

std::span<const GameLogic::EnemyState> GameSession::GetEnemiesState() const noexcept
{
	return m_entities.enemies;
}

std::span<const GameLogic::ProjectileState> GameSession::GetPlayerProjectilesState() const noexcept
{
	return m_entities.playerProjectiles;
}

std::span<const GameLogic::ProjectileState> GameSession::GetEnemyProjectilesState() const noexcept
{
	return m_entities.enemyProjectiles;
}

size_t& GameSession::GetLevel() noexcept
{
	return m_currentLevel;
}

uint32_t GameSession::GetScore() const noexcept
{
	return m_progress.score;
}

void GameSession::Shoot() noexcept
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_progress.playerShootCooldown < m_config.playerShootCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	m_entities.playerProjectiles.emplace_back(
		GameLogic::Vector2(m_entities.player.position.x + GameLogic::Utils::Constants::SHIP_WIDTH * 0.5f, m_entities.player.position.y),
		GameLogic::Utils::ProjectileVelocity(m_config.projectileSpeed, GameLogic::ProjectileOwner::PLAYER),
		GameLogic::ProjectileOwner::PLAYER,
		m_config.playerProjectileDamage
	);

	m_progress.playerShootCooldown = 0.0f;
	++m_progress.projectileCount;
}

void GameSession::Reset() noexcept
{
	m_progress.Reset(m_config);
	m_entities.Reset(m_config.playerLivesCount);

	m_ufoScorePtr = 0;
}

void GameSession::SpawnEnemies() noexcept
{
	for (const auto& enemyConfig : m_config.levels[m_currentLevel].enemies)
	{
		m_entities.enemies.emplace_back(enemyConfig.position, enemyConfig.health, enemyConfig.type);
	}
}

void GameSession::SpawnUFO() noexcept
{
	if (m_progress.ufoSpawnTimer < m_config.ufoSpawnCooldown
		|| m_entities.enemies.size() < m_config.countUFOSpawn
		|| m_entities.ufo) return;

	const float direction = (m_progress.projectileCount % 2) ? 1.0f : -1.0f;
	m_entities.ufo.emplace(GameLogic::UFOState(direction));
	m_progress.ufoSpawnTimer = 0.0f;
}

void GameSession::UpdateEnemies(float deltaTime)
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	UpdateEnemiesHealth();
	UpdateEnemiesPosition(deltaTime);
	UpdateOffscreenEnemies();
}

void GameSession::UpdateProjectiles(float deltaTime)
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	for (auto& projectile : m_entities.playerProjectiles)
	{
		GameLogic::Utils::UpdateProjectilesPosition(projectile, deltaTime);
	} 

	for (auto& projectile : m_entities.enemyProjectiles)
	{
		GameLogic::Utils::UpdateProjectilesPosition(projectile, deltaTime);
	}

	std::erase_if(m_entities.playerProjectiles, [](const GameLogic::ProjectileState& p) {
		return (p.position.y < 0 || p.position.y > GameLogic::Utils::Constants::SCREEN_HEIGHT);
		});

	std::erase_if(m_entities.enemyProjectiles, [](const GameLogic::ProjectileState& p) {
		return (p.position.y < 0 || p.position.y > GameLogic::Utils::Constants::SCREEN_HEIGHT);
		});
}

void GameSession::UpdateScore(GameLogic::EnemyType type)
{
	switch (type)
	{
	case GameLogic::EnemyType::NORMAL:		m_progress.score += m_config.normalEnemyScore;	return;
	case GameLogic::EnemyType::ARMORED:		m_progress.score += m_config.armoredEnemyScore;	return;
	case GameLogic::EnemyType::SHOOTER:		m_progress.score += m_config.shooterEnemyScore;	return;
	case GameLogic::EnemyType::BOSS:		m_progress.score += m_config.bossScore;			return;
	}
}

void GameSession::CheckCollisions()
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	CheckPlayerProjectilesCollisions();
	CheckEnemyProjectilesCollisions();
}

void GameSession::CheckLevelCompletion() noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	if (m_entities.enemies.empty() && m_config.levels[m_currentLevel].hasBoss
		&& !m_progress.bossSpawned)
	{
		std::cout << "Boss has spawned\n";
		m_progress.bossSpawned = true;
		SpawnBoss();
		NotifyObservers({ GameLogic::EventType::BOSS_SPAWNED });
		return;
	}

	if (m_entities.enemies.empty())
	{
		m_gameState = GameLogic::GameState::VICTORY;
		NotifyObservers({ GameLogic::EventType::LEVEL_COMPLETED });
	}
}

void GameSession::CheckPlayerDefeat() noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	if (m_entities.player.livesCount == 0u)
	{
		m_gameState = GameLogic::GameState::GAME_OVER;
		NotifyObservers({ GameLogic::EventType::GAME_OVER });
	}
}

void GameSession::SpawnBoss() noexcept
{
	m_entities.enemies.emplace_back(
		GameLogic::Utils::Constants::BOSS_SPAWN_POSITION,
		m_config.bossEnemyHealth,
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
	auto it = m_entities.enemies.begin();
	while (it != m_entities.enemies.end())
	{
		if (it->health == 0u)
		{
			UpdateScore(it->type);
			it = m_entities.enemies.erase(it);
			continue;
		}
		it++;
	}
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

void GameSession::UpdateOffscreenEnemies()
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	std::erase_if(m_entities.enemies, [](const GameLogic::EnemyState& enemy) {
		return enemy.position.x > GameLogic::Utils::Constants::SCREEN_WIDTH
			|| enemy.position.y > GameLogic::Utils::Constants::SCREEN_HEIGHT;
		});
}

void GameSession::UpdateUFOPosition(float dt)
{
	if (!m_entities.ufo) return;

	m_entities.ufo.value().position.x += (m_config.ufoMovementSpeed * m_entities.ufo.value().direction * dt);

	// Check if UFO is still on screen
	if (m_entities.ufo.value().position.x > GameLogic::Utils::Constants::SCREEN_WIDTH ||
		m_entities.ufo.value().position.x < 0)
	{
		m_entities.ufo = std::nullopt;
	}
}

void GameSession::CheckPlayerProjectilesCollisions()
{
	auto it = m_entities.playerProjectiles.begin();
	while (it != m_entities.playerProjectiles.end())
	{
		const GameLogic::Vector2 projectilePos = it->position;

		for (auto& enemy : m_entities.enemies)
		{
			const float xStart = enemy.position.x;
			const float xEnd = enemy.type == GameLogic::EnemyType::BOSS
				? enemy.position.x + GameLogic::Utils::Constants::BOSS_WIDTH
				: enemy.position.x + GameLogic::Utils::Constants::SHIP_WIDTH;

			const float yStart = enemy.position.y;
			const float yEnd = enemy.type == GameLogic::EnemyType::BOSS
				? yStart + GameLogic::Utils::Constants::BOSS_WIDTH
				: yStart + GameLogic::Utils::Constants::SHIP_WIDTH;

			// Collision between projectile and enemy ship
			if (GameLogic::Utils::AABBCollision(xStart, xEnd, yStart, yEnd, projectilePos.x, projectilePos.y))
			{
				enemy.health = static_cast<int16_t>(std::max(0, enemy.health - m_config.playerProjectileDamage));
				it = m_entities.playerProjectiles.erase(it);
				break;
			}

			// Collision between projectile and UFO
			if (m_entities.ufo.has_value())
			{
				const float xUFOStart = m_entities.ufo.value().position.x;
				const float xUFOEnd = m_entities.ufo.value().position.x + GameLogic::Utils::Constants::UFO_WIDTH;

				const float yUFOStart = m_entities.ufo.value().position.y;
				const float yUFOEnd = m_entities.ufo.value().position.y + GameLogic::Utils::Constants::UFO_WIDTH;

				if (GameLogic::Utils::AABBCollision(xUFOStart, xUFOEnd, yUFOStart, yUFOEnd, projectilePos.x, projectilePos.y))
				{
					const size_t scoreSize = m_entities.ufo.value().scores.size();
					m_progress.score += m_entities.ufo.value().scores[(++m_ufoScorePtr)% scoreSize];
					it = m_entities.playerProjectiles.erase(it);
					break;
				}
			}
		}

		if (it != m_entities.playerProjectiles.end()) it++;
	}
}

void GameSession::CheckEnemyProjectilesCollisions()
{
	auto it = m_entities.enemyProjectiles.begin();
	while (it != m_entities.enemyProjectiles.end())
	{
		const GameLogic::Vector2 projectilePos = it->position;

		const float xStart = m_entities.player.position.x;
		const float xEnd = m_entities.player.position.x + GameLogic::Utils::Constants::SHIP_WIDTH;

		const float yStart = m_entities.player.position.y;
		const float yEnd = m_entities.player.position.y + GameLogic::Utils::Constants::SHIP_WIDTH; // Intended. height is the same as width for the ship

		if (GameLogic::Utils::AABBCollision(xStart, xEnd, yStart, yEnd, projectilePos.x, projectilePos.y))
		{
			m_entities.player.livesCount = std::max(0, m_entities.player.livesCount - m_config.enemyProjectileDamage);
			it = m_entities.enemyProjectiles.erase(it);
			continue;
		}

		if (it != m_entities.enemyProjectiles.end()) it++;
	}
}

void GameSession::SpawnEnemyProjectile()
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_progress.enemyShootCooldown < m_config.enemyShootCooldown
		|| m_entities.enemyProjectiles.size() >= m_config.maxEnemyProjectileCount
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	if (const auto result = GameLogic::Utils::GetUniformEnemyDistribution(m_entities.enemies.size());
		result.has_value())
	{
		auto rd = result.value();
		const size_t index = rd(m_generator);

		m_entities.enemyProjectiles.emplace_back(
			GameLogic::Vector2(m_entities.enemies[index].position.x +
				GameLogic::Utils::Constants::SHIP_WIDTH * 0.5f,
				m_entities.enemies[index].position.y + GameLogic::Utils::Constants::SHIP_WIDTH),
			GameLogic::Utils::ProjectileVelocity(m_config.projectileSpeed, GameLogic::ProjectileOwner::ENEMY),
			GameLogic::ProjectileOwner::ENEMY,
			m_config.enemyProjectileDamage);
	}

	m_progress.enemyShootCooldown = 0.0f;
}
