#include "pch.h"
#include "GameService.h"

GameService::GameService() :
	m_config{ GameLogic::CreateDefaultConfig() },
	m_generator{ std::random_device{}() },
	m_progress{ GameLogic::GameProgress(m_config) },
	m_gameState{ GameLogic::GameState::PLAYING }
{
}

GameService::GameService(GameLogic::GameConfig config, uint32_t seed) :
	m_config{ std::move(config) },
	m_progress{ m_config },
	m_gameState{ GameLogic::GameState::PLAYING },
	m_generator{ seed }
{
}

void GameService::Subscribe(GameLogic::IGameEventObserver* observer)
{
	if (observer && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end())
	{
		m_observers.emplace_back(observer);
	}
}

void GameService::Unsubscribe(GameLogic::IGameEventObserver* observer)
{
	std::erase(m_observers, observer);
}

void GameService::StartLevel() noexcept
{
	Reset();

	SpawnEnemies();
	m_gameState = GameLogic::GameState::PLAYING;
}

void GameService::Update(float deltaTime) noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	if (m_progress.isPlayerRespawning)
	{
		m_progress.playerRespawnTimer += deltaTime;
		if (m_progress.playerRespawnTimer >= m_config.playerRespawnTimer)
		{
			m_progress.isPlayerRespawning = false;
			m_entities.player.isVisible = true;
			m_entities.player.position = GameLogic::Utils::Constants::PLAYER_SPAWN_POSITION;
		}
		return;
	}

	if (m_entities.boss && m_entities.boss->position.y < GameLogic::Utils::Constants::BOSS_DESIRED_LOCATION.y)
	{
		m_entities.boss->position.y += (m_config.bossDropDistance * deltaTime);
		if (m_entities.boss->position.y >= GameLogic::Utils::Constants::BOSS_DESIRED_LOCATION.y)
		{
			m_entities.boss->isVisible = true;
		}
		return;
	}

	CheckCollisions();
	CheckLevelCompletion();
	CheckPlayerDefeat();

	SpawnEnemyProjectile();
	SpawnUFO();

	UpdateEnemies(deltaTime);
	UpdateProjectiles(deltaTime);
	UpdateUFOPosition(deltaTime);
	UpdatePowerUpPosition(deltaTime);
	UpdateActivePowerUpsDuration(deltaTime);
	UpdateBoss(deltaTime);

	m_progress.elapsedTime += deltaTime;

	m_progress.playerShootCooldown += deltaTime;
	m_progress.enemyShootCooldown += deltaTime;
	m_progress.ufoSpawnTimer += deltaTime;
}

void GameService::MovePlayer(GameLogic::Direction direction) noexcept
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING
		|| m_progress.isPlayerRespawning
		|| (m_entities.boss && !m_entities.boss->isVisible)) return;

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

GameLogic::PlayerState GameService::GetPlayerState() const noexcept
{
	return m_entities.player;
}

const std::optional<GameLogic::UFOState>& GameService::GetUFOState() const noexcept
{
	return m_entities.ufo;
}

const std::optional<GameLogic::BossState>& GameService::GetBossState() const noexcept
{
	return m_entities.boss;
}

std::span<const GameLogic::EnemyState> GameService::GetEnemiesState() const noexcept
{
	return m_entities.enemies;
}

std::span<const GameLogic::ProjectileState> GameService::GetPlayerProjectilesState() const noexcept
{
	return m_entities.playerProjectiles;
}

std::span<const GameLogic::ProjectileState> GameService::GetEnemyProjectilesState() const noexcept
{
	return m_entities.enemyProjectiles;
}

std::span<const GameLogic::PowerUpPickup> GameService::GetPowerUpPickups() const noexcept
{
	return m_entities.powerUpPickups;
}

std::span<const GameLogic::WallState> GameService::GetWallsState() const noexcept
{
	return m_entities.wallsState;
}

size_t& GameService::GetLevel() noexcept
{
	return m_currentLevel;
}

uint32_t GameService::GetScore() const noexcept
{
	return m_progress.score;
}

void GameService::Shoot() noexcept
{
	using namespace GameLogic;

	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_progress.playerShootCooldown < m_config.playerShootCooldown / m_progress.fireRateMultiplier
		|| m_gameState != GameState::PLAYING
		|| m_progress.isPlayerRespawning
		|| (m_entities.boss && !m_entities.boss->isVisible)) return;

	const float x = m_entities.player.position.x;
	const float y = m_entities.player.position.y;

	m_entities.playerProjectiles.emplace_back(
		Vector2(x + Utils::Constants::SHIP_WIDTH * 0.5f, y),
		-m_config.projectileSpeed,
		m_config.playerProjectileDamage
	);

	if (m_progress.multiShot)
	{
		m_entities.playerProjectiles.emplace_back(
			Vector2(x + Utils::Constants::SHIP_WIDTH * 0.33f, y + Utils::Constants::SHIP_WIDTH * 0.33f),
			-m_config.projectileSpeed,
			m_config.playerProjectileDamage
		);

		m_entities.playerProjectiles.emplace_back(
			Vector2(x + Utils::Constants::SHIP_WIDTH * 0.66f, y + Utils::Constants::SHIP_WIDTH * 0.33f),
			-m_config.projectileSpeed,
			m_config.playerProjectileDamage
		);
	}

	m_progress.playerShootCooldown = 0.0f;
	++m_progress.projectileCount;

	NotifyObservers(EventType::PLAYER_SHOT);
}

void GameService::Reset() noexcept
{
	m_progress.Reset(m_config);
	m_entities.Reset(m_config.playerLivesCount);
	InitializeWalls();

	m_ufoScorePtr = 0;
}

void GameService::SpawnEnemies() noexcept
{
	for (const auto& enemyConfig : m_config.levels[m_currentLevel].enemies)
	{
		m_entities.enemies.emplace_back(enemyConfig.position, enemyConfig.health, enemyConfig.type);
	}
}

void GameService::SpawnUFO() noexcept
{
	const bool canSpawn = m_entities.enemies.size() >= m_config.countUFOSpawn && !m_entities.ufo;

	if (!canSpawn)
	{
		// Enemies dropped below countUFOSpawn during the warning window: the UFO will not come
		if (m_progress.ufoApproaching)
		{
			m_progress.ufoApproaching = false;
			NotifyObservers(GameLogic::EventType::UFO_APPROACH_CANCELLED);
		}
		return;
	}

	if (!m_progress.ufoApproaching
		&& m_progress.ufoSpawnTimer >= m_config.ufoSpawnCooldown - GameLogic::Utils::Constants::UFO_WARNING_TIME)
	{
		m_progress.ufoApproaching = true;
		NotifyObservers(GameLogic::EventType::UFO_APPROACHING);
	}

	if (m_progress.ufoSpawnTimer < m_config.ufoSpawnCooldown) return;

	const float direction = (m_progress.projectileCount % 2) ? 1.0f : -1.0f;
	const GameLogic::Vector2 position = direction > 0
		? GameLogic::Utils::Constants::UFO_LEFT_SPAWN_POSITION
		: GameLogic::Utils::Constants::UFO_RIGHT_SPAWN_POSITION;
	m_entities.ufo.emplace(GameLogic::UFOState(direction, position));
	m_progress.ufoSpawnTimer = 0.0f;
	m_progress.ufoApproaching = false;
	m_progress.ufoLeaving = false;

	NotifyObservers(GameLogic::EventType::UFO_SPAWNED);
}

void GameService::InitializeWalls()
{
	using namespace GameLogic;

	Vector2 wallPos = Utils::Constants::FIRST_WALL_POSITION;

	for (auto& wall : m_entities.wallsState)
	{
		wall.position = wallPos;
		for (size_t row = 0; row < Utils::Constants::WALL_ROWS; ++row)
		{
			for (size_t col = 0; col < Utils::Constants::WALL_COLS; ++col)
			{
				wall.cells[row * Utils::Constants::WALL_COLS + col] = Utils::Constants::WALL_TEMPLATE[row][col] == '#';
			}
		}
		wallPos.x += (Utils::Constants::WALL_COLS * Utils::Constants::CELL_SIZE) + Utils::Constants::WALLS_GAP;
	}
}

void GameService::DestroyCells(GameLogic::WallState& wall, size_t row, size_t col)
{
	using namespace GameLogic;
	using namespace Utils::Constants;

	wall.cells[row * WALL_COLS + col] = false;

	if (Utils::IsValidIndex(row + 1, col, WALL_ROWS, WALL_COLS)) wall.cells[(row + 1) * WALL_COLS + col] = false;
	if (Utils::IsValidIndex(row, col - 1, WALL_ROWS, WALL_COLS)) wall.cells[row * WALL_COLS + (col - 1)] = false;
	if (Utils::IsValidIndex(row, col + 1, WALL_ROWS, WALL_COLS)) wall.cells[row * WALL_COLS + (col + 1)] = false;
}

void GameService::UpdateEnemies(float deltaTime)
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	UpdateEnemiesHealth();
	UpdateEnemiesPosition(deltaTime);
	UpdateOffscreenEnemies();
}

void GameService::UpdateProjectiles(float deltaTime)
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

void GameService::UpdateActivePowerUpsDuration(float deltaTime)
{
	m_progress.fireRateMultiplier = 1.0f;
	m_progress.multiShot = false;

	if (m_entities.activePowerUps.empty()) return;

	for (auto& powerUp : m_entities.activePowerUps)
	{
		powerUp.duration -= deltaTime;
		switch (powerUp.type)
		{
		case GameLogic::PowerUpType::RAPID_FIRE: m_progress.fireRateMultiplier = 2.0f; break;
		case GameLogic::PowerUpType::MULTI_SHOT: m_progress.multiShot = true; break;
		}
 	}

	std::erase_if(m_entities.activePowerUps, [](const GameLogic::ActivePowerUp p)
		{

		return p.duration <= 0.0f;
		});
}

void GameService::UpdateBoss(float deltaTime)
{
	if (!m_entities.boss || !m_entities.boss->isVisible) return;

	UpdateBossPosition(deltaTime);
	SpawnBossProjectile();

	if (m_entities.boss->health == 0u)
	{
		m_entities.boss = std::nullopt;
		m_progress.score += m_config.bossScore;
		m_progress.bossDefeated = true;
		NotifyObservers(GameLogic::EventType::ENEMY_DESTROYED);
	}
}

void GameService::UpdateBossPosition(float deltaTime)
{
	if (m_entities.boss->position.x < 0
		|| m_entities.boss->position.x > GameLogic::Utils::Constants::SCREEN_WIDTH - GameLogic::Utils::Constants::BOSS_WIDTH)
	{
		m_entities.boss->ChangeDirection();
		m_entities.boss->position.y += m_config.bossDropDistance;
	}
	
	const float speed = m_config.bossMovementSpeed * std::min(3.0f, (static_cast<float>(m_config.bossEnemyHealth) / static_cast<float>(m_entities.boss->health)));
	m_entities.boss->position.x += (speed * m_entities.boss->direction * deltaTime);
}

void GameService::UpdateScore(GameLogic::EnemyType type)
{
	switch (type)
	{
	case GameLogic::EnemyType::NORMAL:		m_progress.score += m_config.normalEnemyScore;	return;
	case GameLogic::EnemyType::ARMORED:		m_progress.score += m_config.armoredEnemyScore;	return;
	case GameLogic::EnemyType::SHOOTER:		m_progress.score += m_config.shooterEnemyScore;	return;
	}
}

void GameService::CheckCollisions()
{
	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_gameState != GameLogic::GameState::PLAYING) return;

	CheckPlayerProjectilesCollisions();
	CheckEnemyProjectilesCollisions();
	CheckWallEnemyCollisions();
	CheckPlayerEnemyCollisions();
	CheckPlayerPowerUpCollisions();
}

void GameService::CheckLevelCompletion() noexcept
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
		NotifyObservers(GameLogic::EventType::LEVEL_COMPLETED);
	}
}

void GameService::CheckPlayerDefeat() noexcept
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	if (m_entities.player.livesCount == 0u)
	{
		m_gameState = GameLogic::GameState::GAME_OVER;
		NotifyObservers(GameLogic::EventType::GAME_OVER);
	}
}

void GameService::SpawnBoss() noexcept
{
	m_entities.boss.emplace(GameLogic::BossState(GameLogic::Utils::Constants::BOSS_SPAWN_POSITION, m_config.bossEnemyHealth));
	m_entities.enemyProjectiles.clear();
	m_entities.playerProjectiles.clear();
}

void GameService::SpawnPowerUpPickup(GameLogic::Vector2 position) noexcept
{
	using namespace GameLogic;
	std::uniform_real_distribution<float> realDist(0, 1.0f);

	if (realDist(m_generator) >= m_config.powerUpChance) return;

	std::uniform_int_distribution<int> dist(0, Utils::Constants::POWER_UP_TYPES - 1);

	const PowerUpType type = static_cast<PowerUpType>(dist(m_generator));

	m_entities.powerUpPickups.emplace_back(position, type);
}

void GameService::ActivatePowerUp(const GameLogic::PowerUpType type)
{
	switch (type)
	{
	case GameLogic::PowerUpType::BONUS_SCORE: m_progress.score += m_config.powerUpBonusScore; break;
	case GameLogic::PowerUpType::EXTRA_LIFE:  m_entities.player.livesCount = std::min(static_cast<int>(m_config.playerLivesCount), m_entities.player.livesCount + 1); break;
	case GameLogic::PowerUpType::MULTI_SHOT:
	case GameLogic::PowerUpType::RAPID_FIRE: m_entities.activePowerUps.emplace_back(m_config.powerUpDuration, type); break;
	}
}

void GameService::NotifyObservers(GameLogic::EventType event)
{
	for (const auto& observer : m_observers)
	{
		observer->OnGameEvent(event);
	}
}

void GameService::UpdateEnemiesHealth()
{
	auto it = m_entities.enemies.begin();
	while (it != m_entities.enemies.end())
	{
		if (it->health == 0u)
		{
			UpdateScore(it->type);
			SpawnPowerUpPickup(it->position);
			NotifyObservers(GameLogic::EventType::ENEMY_DESTROYED);
			it = m_entities.enemies.erase(it);
			continue;
		}
		it++;
	}
}

void GameService::UpdateEnemiesPosition(float deltaTime)
{
	bool hitEdge = false;

	for (auto& enemy : m_entities.enemies)
	{
		GameLogic::Utils::UpdateEnemyHorizontalPosition(enemy, m_progress, m_entities.enemies.size(), m_config.levels[m_currentLevel].enemies.size(), deltaTime);

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

void GameService::UpdateOffscreenEnemies()
{
	if (m_gameState != GameLogic::GameState::PLAYING) return;

	std::erase_if(m_entities.enemies, [](const GameLogic::EnemyState& enemy) {
		return enemy.position.x > GameLogic::Utils::Constants::SCREEN_WIDTH
			|| enemy.position.y > GameLogic::Utils::Constants::SCREEN_HEIGHT;
		});
}

void GameService::OnPlayerHit()
{
	m_entities.player.livesCount = std::max(0, m_entities.player.livesCount - m_config.enemyProjectileDamage);
	NotifyObservers(GameLogic::EventType::PLAYER_HIT);

	if (m_entities.player.livesCount != 0)
	{
		m_progress.isPlayerRespawning = true;
		m_entities.player.isVisible = false;
		m_progress.playerRespawnTimer = 0.0f;
	}
}

void GameService::UpdateUFOPosition(float dt)
{
	if (!m_entities.ufo) return;

	m_entities.ufo->position.x += (m_config.ufoMovementSpeed * m_entities.ufo->direction * dt);

	const float x = m_entities.ufo->position.x;
	const float distanceToExit = m_entities.ufo->direction > 0
		? GameLogic::Utils::Constants::SCREEN_WIDTH - x
		: x;

	if (!m_progress.ufoLeaving && distanceToExit <= GameLogic::Utils::Constants::UFO_LEAVING_DISTANCE)
	{
		m_progress.ufoLeaving = true;
		NotifyObservers(GameLogic::EventType::UFO_LEAVING);
	}

	// Check if UFO is still on screen
	if (x > GameLogic::Utils::Constants::SCREEN_WIDTH || x < 0)
	{
		m_entities.ufo = std::nullopt;
		m_progress.ufoLeaving = false;
		NotifyObservers(GameLogic::EventType::UFO_DESPAWNED);
	}
}

void GameService::UpdatePowerUpPosition(float deltaTime)
{
	for (auto& powerUp : m_entities.powerUpPickups)
	{
		powerUp.position.y += (m_config.powerUpDropDistance * deltaTime);
	}
}

void GameService::CheckPlayerProjectilesCollisions()
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
						NotifyObservers(GameLogic::EventType::ENEMY_HIT);
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

				if (m_entities.boss->health > 0)
				{
					NotifyObservers(GameLogic::EventType::ENEMY_HIT);
				}
				return true;
			}

			return false;
		});
}

void GameService::CheckEnemyProjectilesCollisions()
{
	using namespace GameLogic::Utils;

	const auto playerBounds = GetBounds(m_entities.player.position, Constants::SHIP_WIDTH);

	std::erase_if(m_entities.enemyProjectiles, [&](const GameLogic::ProjectileState& projectile)
		{
			if (playerBounds.Overlaps(projectile.position))
			{
				OnPlayerHit();
				return true;
			}

			for (auto& wall : m_entities.wallsState)
			{
				for (size_t row = 0; row < Constants::WALL_ROWS; ++row)
				{
					for (size_t col = 0; col < Constants::WALL_COLS; ++col)
					{
						if (wall.cells[row * Constants::WALL_COLS + col])
						{
							const float x = wall.position.x + col * Constants::CELL_SIZE;
							const float y = wall.position.y + row * Constants::CELL_SIZE;
							const Bounds bounds = GetBounds(GameLogic::Vector2(x, y), Constants::CELL_SIZE);

							if (bounds.Overlaps(projectile.position))
							{
								DestroyCells(wall, row, col);
								return true;
							}
						}
					}
				}
			}
			return false;
		});
}

void GameService::CheckPlayerPowerUpCollisions()
{
	const Bounds playerBounds = GetBounds(m_entities.player.position, GameLogic::Utils::Constants::SHIP_WIDTH);

	std::erase_if(m_entities.powerUpPickups, [&](const GameLogic::PowerUpPickup& powerUp)
		{
			const Bounds powerUpBounds = GetBounds(powerUp.position, GameLogic::Utils::Constants::POWER_UP_WIDTH);
			if (!playerBounds.Overlaps(powerUpBounds)) return false;

			ActivatePowerUp(powerUp.type);
			return true;
		});
}

void GameService::CheckWallEnemyCollisions()
{
	using namespace GameLogic;

	for (const auto& enemy : m_entities.enemies)
	{
		const Bounds enemyBounds = GetBounds(enemy.position, Utils::Constants::SHIP_WIDTH);
		for (auto& wall : m_entities.wallsState)
		{
			for (size_t row = 0; row < Utils::Constants::WALL_ROWS; ++row)
			{
				for (size_t col = 0; col < Utils::Constants::WALL_COLS; ++col)
				{
					if (wall.cells[row * Utils::Constants::WALL_COLS + col])
					{
						const float x = wall.position.x + col * Utils::Constants::CELL_SIZE;
						const float y = wall.position.y + row * Utils::Constants::CELL_SIZE;
						const Bounds bounds = GetBounds(Vector2(x, y), Utils::Constants::CELL_SIZE);

						if (enemyBounds.Overlaps(bounds))
						{
							wall.cells[row * Utils::Constants::WALL_COLS + col] = false;
						}
					}
				}
			}
		}
	}
}

void GameService::CheckPlayerEnemyCollisions()
{
	using namespace GameLogic;

	const Bounds playerBounds = GetBounds(m_entities.player.position, Utils::Constants::SHIP_WIDTH);

	for (const auto& enemy : m_entities.enemies)
	{
		const Bounds enemyBounds = GetBounds(enemy.position, Utils::Constants::SHIP_WIDTH);
		if (playerBounds.Overlaps(enemyBounds))
		{
			OnPlayerHit();
			break;
		}
	}
}

void GameService::SpawnEnemyProjectile() noexcept
{
	using namespace GameLogic;

	if (m_progress.elapsedTime < m_config.startCooldown
		|| m_progress.enemyShootCooldown < m_config.enemyShootCooldown
		|| m_entities.enemyProjectiles.size() >= m_config.maxEnemyProjectileCount
		|| m_entities.enemies.empty()
		|| m_gameState != GameState::PLAYING) return;

	if (const auto result = Utils::GetUniformEnemyDistribution(static_cast<int>(m_entities.enemies.size()));
		result.has_value())
	{
		auto rd = result.value();
		const size_t index = rd(m_generator);
		
		const float x = m_entities.enemies[index].position.x;
		const float y = m_entities.enemies[index].position.y;

		m_entities.enemyProjectiles.emplace_back(
			Vector2(x + Utils::Constants::SHIP_WIDTH * 0.5f, y + Utils::Constants::SHIP_WIDTH),
			m_config.projectileSpeed,
			m_config.enemyProjectileDamage);

		if (m_entities.enemies[index].type == EnemyType::SHOOTER)
		{
			m_entities.enemyProjectiles.emplace_back(
				Vector2(x, y + Utils::Constants::SHIP_WIDTH),
				m_config.projectileSpeed,
				m_config.enemyProjectileDamage);

			m_entities.enemyProjectiles.emplace_back(
				Vector2(x + Utils::Constants::SHIP_WIDTH, y + Utils::Constants::SHIP_WIDTH),
				m_config.projectileSpeed,
				m_config.enemyProjectileDamage);
		}
	}

	m_progress.enemyShootCooldown = 0.0f;
}

void GameService::SpawnBossProjectile() noexcept
{
	using namespace GameLogic;

	if (m_progress.enemyShootCooldown < m_config.enemyShootCooldown) return;

	const GameLogic::Vector2 position = m_entities.boss->position;

	m_entities.enemyProjectiles.emplace_back(
		Vector2(position.x + Utils::Constants::BOSS_WIDTH * 0.5f, position.y + Utils::Constants::BOSS_WIDTH),
		m_config.projectileSpeed,
		m_config.enemyProjectileDamage);

	if (m_entities.boss->health < static_cast<int16_t>(m_config.bossEnemyHealth * 0.5f))
	{
		m_entities.enemyProjectiles.emplace_back(
			Vector2(position.x, position.y + Utils::Constants::BOSS_WIDTH),
			m_config.projectileSpeed,
			m_config.enemyProjectileDamage);

		m_entities.enemyProjectiles.emplace_back(
			Vector2(position.x + Utils::Constants::BOSS_WIDTH, position.y + Utils::Constants::BOSS_WIDTH),
			m_config.projectileSpeed,
			m_config.enemyProjectileDamage);
	}

	m_progress.enemyShootCooldown = 0.0f;
}
