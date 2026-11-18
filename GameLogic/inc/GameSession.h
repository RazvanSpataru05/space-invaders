#pragma once

#include "API/Interfaces/IGameEventObserver.h"
#include "API/Interfaces/IGameService.h"

#include "API/Structs/GameConfig.h"
#include "API/Structs/GameEntities.h"
#include "API/Structs/Vector2.h"

#include "Utils/GameConfigFactory.h"
#include "Utils/PlayerUtils.h"

#include <algorithm>
#include <iostream>
#include <random>

class GameSession final : public GameLogic::IGameService
{
public:
	explicit GameSession();

	void Subscribe(GameLogic::IGameEventObserver* observer)		override;
	void Unsubscribe(GameLogic::IGameEventObserver* observer)	override;

	void StartLevel() noexcept override;

	void Update(float deltaTime) noexcept override;

	void MovePlayer(GameLogic::Direction direction) noexcept override;
	void Shoot() noexcept override;
	void Reset() noexcept override;

	[[nodiscard]] GameLogic::PlayerState						GetPlayerState()			const noexcept override;
	[[nodiscard]] std::optional<GameLogic::UFOState>			GetUFOState()				const noexcept override;
	[[nodiscard]] std::span<const GameLogic::EnemyState>		GetEnemiesState()			const noexcept override;
	[[nodiscard]] std::span<const GameLogic::ProjectileState>	GetPlayerProjectilesState()	const noexcept override;
	[[nodiscard]] std::span<const GameLogic::ProjectileState>	GetEnemyProjectilesState()  const noexcept override;
	[[nodiscard]] size_t&										GetLevel()					noexcept override;
	[[nodiscard]] uint32_t										GetScore()					const noexcept override;
	
	void UpdateEnemies(float deltaTime);
	void UpdateProjectiles(float deltaTime);
	void UpdateScore(GameLogic::EnemyType enemyType);
	
	void NotifyObservers(const GameLogic::GameEvent& event);

private:
	void SpawnEnemies() noexcept;
	void SpawnUFO() noexcept;
	void SpawnBoss() noexcept;

	void CheckCollisions();
	void CheckLevelCompletion() noexcept;
	void CheckPlayerDefeat()	noexcept;

	void GeneratePowerUp();
	void CollectPowerUp(const GameLogic::PowerUpState& powerUp);
	void ApplyPowerUp(GameLogic::PowerUpType type);

	void UpdateEnemiesHealth();
	void UpdateEnemiesPosition(float deltaTime);
	void UpdateOffscreenEnemies();

	void UpdateUFOPosition(float deltaTime);

	void CheckPlayerProjectilesCollisions();
	void CheckEnemyProjectilesCollisions();

	void SpawnEnemyProjectile();

private:
	GameSession(const GameSession&) = delete;
	GameSession& operator=(const GameSession&) = delete;

	GameSession(GameSession&&) = delete;
	GameSession& operator=(GameSession&) = delete;

private:
	GameLogic::GameEntities m_entities;
	GameLogic::GameProgress m_progress;

	GameLogic::GameConfig m_config;
	GameLogic::GameState m_gameState;

	std::vector<GameLogic::IGameEventObserver*> m_observers;

	size_t m_currentLevel{};
	size_t m_ufoScorePtr{};
	std::mt19937 m_generator;
	std::uniform_int_distribution<size_t> m_dist;
};

