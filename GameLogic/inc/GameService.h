#pragma once

#include "API/Interfaces/IGameEventObserver.h"
#include "API/Interfaces/IGameService.h"

#include "API/Structs/GameConfig.h"
#include "API/Structs/GameEntities.h"
#include "API/Structs/Vector2.h"

#include "Enums/EventType.h"

#include "Utils/GameConfigFactory.h"
#include "Utils/PlayerUtils.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <cstdint>


class GameService final : public GameLogic::IGameService
{
public:
	explicit GameService();
	GameService(GameLogic::GameConfig config, uint32_t seed);

	void Subscribe(GameLogic::IGameEventObserver* observer)		override;
	void Unsubscribe(GameLogic::IGameEventObserver* observer)	override;

	void StartLevel() noexcept override;

	void Update(float deltaTime) noexcept override;

	void MovePlayer(GameLogic::Direction direction) noexcept override;
	void Shoot() noexcept override;
	void Reset() noexcept override;

	[[nodiscard]] GameLogic::PlayerState						GetPlayerState()			const noexcept override;
	[[nodiscard]] const std::optional<GameLogic::BossState>&	GetBossState()				const noexcept override;
	[[nodiscard]] const std::optional<GameLogic::UFOState>&		GetUFOState()				const noexcept override;
	[[nodiscard]] std::span<const GameLogic::EnemyState>		GetEnemiesState()			const noexcept override;
	[[nodiscard]] std::span<const GameLogic::ProjectileState>	GetPlayerProjectilesState()	const noexcept override;
	[[nodiscard]] std::span<const GameLogic::ProjectileState>	GetEnemyProjectilesState()  const noexcept override;
	[[nodiscard]] std::span<const GameLogic::PowerUpPickup>		GetPowerUpPickups()			const noexcept override;
	[[nodiscard]] std::span<const GameLogic::WallState>			GetWallsState()				const noexcept override;
	[[nodiscard]] size_t&										GetLevel()					noexcept override;
	[[nodiscard]] uint32_t										GetScore()					const noexcept override;

private:
	void InitializeWalls();
	void DestroyCells(GameLogic::WallState& wall, size_t row, size_t col);

	void UpdateEnemies(float deltaTime);
	void UpdateProjectiles(float deltaTime);
	void UpdateActivePowerUpsDuration(float deltaTime);
	void UpdateScore(GameLogic::EnemyType enemyType);

	void UpdateBoss(float deltaTime);
	void UpdateBossPosition(float deltaTime);

	void NotifyObservers(GameLogic::EventType event);

	void SpawnEnemies() noexcept;
	void SpawnUFO() noexcept;
	void SpawnBoss() noexcept;
	void SpawnPowerUpPickup(GameLogic::Vector2 position) noexcept;

	void CheckCollisions();
	void CheckLevelCompletion() noexcept;
	void CheckPlayerDefeat()	noexcept;

	void ActivatePowerUp(GameLogic::PowerUpType type);

	void UpdateEnemiesHealth();
	void UpdateEnemiesPosition(float deltaTime);
	void UpdateOffscreenEnemies();

	void OnPlayerHit();

	void UpdateUFOPosition(float deltaTime);

	void UpdatePowerUpPosition(float deltaTime);

	void CheckPlayerProjectilesCollisions();
	void CheckEnemyProjectilesCollisions();
	void CheckPlayerPowerUpCollisions();
	void CheckWallEnemyCollisions();
	void CheckPlayerEnemyCollisions();

	void SpawnEnemyProjectile() noexcept;
	void SpawnBossProjectile() noexcept;

private:
	GameService(const GameService&) = delete;
	GameService& operator=(const GameService&) = delete;

	GameService(GameService&&) = delete;
	GameService& operator=(GameService&) = delete;

private:
	GameLogic::GameConfig m_config;

	GameLogic::GameEntities m_entities;
	GameLogic::GameProgress m_progress;

	GameLogic::GameState m_gameState;

	std::vector<GameLogic::IGameEventObserver*> m_observers;

	size_t m_currentLevel{};
	size_t m_ufoScorePtr{};
	std::mt19937 m_generator;
	std::uniform_int_distribution<size_t> m_dist;
};

