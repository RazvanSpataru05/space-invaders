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
	UpdatePowerUpPosition(deltaTime);
	UpdateBoss(deltaTime);

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

const std::optional<GameLogic::UFOState>& GameSession::GetUFOState() const noexcept
{
	return m_entities.ufo;
}

const std::optional<GameLogic::BossState>& GameSession::GetBossState() const noexcept
{
	return m_entities.boss;
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

std::span<const GameLogic::PowerUpPickup> GameSession::GetPowerUpPickups() const noexcept
{
	return m_entities.powerUpPickups;
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

	NotifyObservers({ GameLogic::EventType::PLAYER_SHOT });
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
	const bool canSpawn = m_entities.enemies.size() >= m_config.countUFOSpawn && !m_entities.ufo;

	if (!canSpawn)
	{
		// Enemies dropped below countUFOSpawn during the warning window: the UFO will not come
		if (m_progress.ufoApproaching)
		{
			m_progress.ufoApproaching = false;
			NotifyObservers({ GameLogic::EventType::UFO_APPROACH_CANCELLED });
		}
		return;
	}

	if (!m_progress.ufoApproaching
		&& m_progress.ufoSpawnTimer >= m_config.ufoSpawnCooldown - GameLogic::Utils::Constants::UFO_WARNING_TIME)
	{
		m_progress.ufoApproaching = true;
		NotifyObservers({ GameLogic::EventType::UFO_APPROACHING });
	}

	if (m_progress.ufoSpawnTimer < m_config.ufoSpawnCooldown) return;

	const float direction = (m_progress.projectileCount % 2) ? 1.0f : -1.0f;
	m_entities.ufo.emplace(GameLogic::UFOState(direction));
	m_progress.ufoSpawnTimer = 0.0f;
	m_progress.ufoApproaching = false;
	m_progress.ufoLeaving = false;

	NotifyObservers({ GameLogic::EventType::UFO_SPAWNED });
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

void GameSession::UpdateBoss(float deltaTime)
{
	if (!m_entities.boss) return;

	UpdateBossPosition(deltaTime);
	SpawnBossProjectile();

	if (m_entities.boss->health == 0u)
	{
		m_entities.boss = std::nullopt;
		m_progress.score += m_config.bossScore;
		m_progress.bossDefeated = true;
		NotifyObservers({ GameLogic::EventType::ENEMY_DESTROYED });
	}
}

void GameSession::UpdateBossPosition(float deltaTime)
{
	if (m_entities.boss.value().position.x < 0
		|| m_entities.boss.value().position.x > GameLogic::Utils::Constants::SCREEN_WIDTH - GameLogic::Utils::Constants::BOSS_WIDTH)
	{
		m_entities.boss.value().ChangeDirection();
		m_entities.boss.value().position.y += m_config.bossDropDistance;
	}
	
	const float direction = m_entities.boss.value().direction;
	m_entities.boss.value().position.x += (m_config.enemyMovementSpeed * direction * deltaTime);
}

void GameSession::UpdateScore(GameLogic::EnemyType type)
{
	switch (type)
	{
	case GameLogic::EnemyType::NORMAL:		m_progress.score += m_config.normalEnemyScore;	return;
	case GameLogic::EnemyType::ARMORED:		m_progress.score += m_config.armoredEnemyScore;	return;
	case GameLogic::EnemyType::SHOOTER:		m_progress.score += m_config.shooterEnemyScore;	return;
	}
}

void GameSession::CheckCollisions()
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	CheckPlayerProjectilesCollisions();
	CheckEnemyProjectilesCollisions();
	CheckPlayerPowerUpCollisions();
}

void GameSession::CheckLevelCompletion() noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	if (m_entities.enemies.empty() && m_config.levels[m_currentLevel].hasBoss
		&& !m_entities.boss && !m_progress.bossDefeated)
	{
		SpawnBoss();
		return;
	}

	if (m_entities.enemies.empty() && !m_entities.boss)
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
	m_entities.boss.emplace(GameLogic::BossState(m_config.bossEnemyHealth));
}

void GameSession::SpawnPowerUpPickup(GameLogic::Vector2 position)
{
	using namespace GameLogic;
	std::uniform_real_distribution<float> realDist(0, 1.0f);
	if (realDist(m_generator) >= m_config.powerUpChance) return;

	std::uniform_int_distribution<int> dist(0, Utils::Constants::POWER_UP_TYPES - 1);

	const PowerUpType type = static_cast<PowerUpType>(dist(m_generator));

	m_entities.powerUpPickups.emplace_back(position, type);
}

void GameSession::CollectPowerUp(const GameLogic::PowerUpPickup& powerUp)
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
			SpawnPowerUpPickup(it->position);
			NotifyObservers({ GameLogic::EventType::ENEMY_DESTROYED });
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

	const float x = m_entities.ufo.value().position.x;
	const float distanceToExit = m_entities.ufo.value().direction > 0
		? GameLogic::Utils::Constants::SCREEN_WIDTH - x
		: x;

	if (!m_progress.ufoLeaving && distanceToExit <= GameLogic::Utils::Constants::UFO_LEAVING_DISTANCE)
	{
		m_progress.ufoLeaving = true;
		NotifyObservers({ GameLogic::EventType::UFO_LEAVING });
	}

	// Check if UFO is still on screen
	if (x > GameLogic::Utils::Constants::SCREEN_WIDTH || x < 0)
	{
		m_entities.ufo = std::nullopt;
		m_progress.ufoLeaving = false;
		NotifyObservers({ GameLogic::EventType::UFO_DESPAWNED });
	}
}

void GameSession::UpdatePowerUpPosition(float deltaTime)
{
	for (auto& powerUp : m_entities.powerUpPickups)
	{
		powerUp.position.y += (m_config.powerUpDropDistance * deltaTime);
	}
}

void GameSession::CheckPlayerProjectilesCollisions()
{
	using namespace GameLogic::Utils;

	std::erase_if(m_entities.playerProjectiles, [this](const GameLogic::ProjectileState& projectile)
		{
			for (auto& enemy : m_entities.enemies)
			{
				if (GetBounds(enemy.position, Constants::SHIP_WIDTH).Overlaps(projectile.position))
				{
					enemy.health = static_cast<int16_t>(std::max(0, enemy.health - m_config.playerProjectileDamage));
					if (enemy.health > 0)
					{
						NotifyObservers({ GameLogic::EventType::ENEMY_HIT });
					}
					return true;
				}
			}

			if (m_entities.ufo
				&& GetBounds(m_entities.ufo->position, Constants::UFO_WIDTH).Overlaps(projectile.position))
			{
				const size_t scoreSize = m_entities.ufo->scores.size();
				m_progress.score += m_entities.ufo->scores[(++m_ufoScorePtr) % scoreSize];
				return true;
			}

			if (m_entities.boss
				&& GetBounds(m_entities.boss->position, Constants::BOSS_WIDTH).Overlaps(projectile.position))
			{
				m_entities.boss->health = static_cast<int16_t>(std::max(0, m_entities.boss->health - m_config.playerProjectileDamage));
				std::cout << "health: " << m_entities.boss->health << '\n';
				if (m_entities.boss->health > 0)
				{
					NotifyObservers({ GameLogic::EventType::ENEMY_HIT });
				}
				return true;
			}

			return false;
		});
}

void GameSession::CheckEnemyProjectilesCollisions()
{
	const auto playerBounds = GetBounds(m_entities.player.position, GameLogic::Utils::Constants::SHIP_WIDTH);

	std::erase_if(m_entities.enemyProjectiles, [&](const GameLogic::ProjectileState& projectile)
		{
			if (!playerBounds.Overlaps(projectile.position)) return false;

			m_entities.player.livesCount = std::max(0, m_entities.player.livesCount - m_config.enemyProjectileDamage);
			NotifyObservers({ GameLogic::EventType::PLAYER_HIT });
			return true;
		});
}

void GameSession::CheckPlayerPowerUpCollisions()
{
	const auto playerBounds = GetBounds(m_entities.player.position, GameLogic::Utils::Constants::SHIP_WIDTH);

	std::erase_if(m_entities.powerUpPickups, [&](const GameLogic::PowerUpPickup& powerUp)
		{
			const auto powerUpBounds = GetBounds(powerUp.position, GameLogic::Utils::Constants::POWER_UP_WIDTH);
			if (!playerBounds.Overlaps(powerUpBounds)) return false;

			// TODO: Activate powerup
			return true;
		});
}

void GameSession::SpawnEnemyProjectile()
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_progress.enemyShootCooldown < m_config.enemyShootCooldown
		|| m_entities.enemyProjectiles.size() >= m_config.maxEnemyProjectileCount
		|| m_entities.enemies.empty()
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	if (const auto result = GameLogic::Utils::GetUniformEnemyDistribution(m_entities.enemies.size());
		result.has_value())
	{
		auto rd = result.value();
		const size_t index = rd(m_generator);

		switch (m_entities.enemies[index].type)
		{
		case GameLogic::EnemyType::NORMAL:
		case GameLogic::EnemyType::ARMORED:
		case GameLogic::EnemyType::SHOOTER: SpawnNormalEnemyProjectile(index); break;
		}
	}

	m_progress.enemyShootCooldown = 0.0f;
}

void GameSession::SpawnNormalEnemyProjectile(size_t index)
{
	m_entities.enemyProjectiles.emplace_back(
		GameLogic::Vector2(m_entities.enemies[index].position.x +
			GameLogic::Utils::Constants::SHIP_WIDTH * 0.5f,
			m_entities.enemies[index].position.y + GameLogic::Utils::Constants::SHIP_WIDTH),
		GameLogic::Utils::ProjectileVelocity(m_config.projectileSpeed, GameLogic::ProjectileOwner::ENEMY),
		GameLogic::ProjectileOwner::ENEMY,
		m_config.enemyProjectileDamage);
}

void GameSession::SpawnBossProjectile()
{
	if (m_progress.enemyShootCooldown < m_config.enemyShootCooldown) return;

	const GameLogic::Vector2 position = m_entities.boss.value().position;

	m_entities.enemyProjectiles.emplace_back(
		GameLogic::Vector2(position.x + GameLogic::Utils::Constants::BOSS_WIDTH * 0.5f,
			position.y + GameLogic::Utils::Constants::BOSS_WIDTH),
		GameLogic::Utils::ProjectileVelocity(m_config.projectileSpeed, GameLogic::ProjectileOwner::ENEMY),
		GameLogic::ProjectileOwner::ENEMY,
		m_config.enemyProjectileDamage);

	if (m_entities.boss.value().health < static_cast<int16_t>(m_config.bossEnemyHealth * 0.5f))
	{
		m_entities.enemyProjectiles.emplace_back(
			GameLogic::Vector2(position.x, position.y + GameLogic::Utils::Constants::BOSS_WIDTH),
			GameLogic::Utils::ProjectileVelocity(m_config.projectileSpeed, GameLogic::ProjectileOwner::ENEMY),
			GameLogic::ProjectileOwner::ENEMY,
			m_config.enemyProjectileDamage);

		m_entities.enemyProjectiles.emplace_back(
			GameLogic::Vector2(position.x + GameLogic::Utils::Constants::BOSS_WIDTH,
				position.y + GameLogic::Utils::Constants::BOSS_WIDTH),
			GameLogic::Utils::ProjectileVelocity(m_config.projectileSpeed, GameLogic::ProjectileOwner::ENEMY),
			GameLogic::ProjectileOwner::ENEMY,
			m_config.enemyProjectileDamage);
	}

	m_progress.enemyShootCooldown = 0.0f;
}
