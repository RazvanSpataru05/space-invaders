#pragma once

#include "Enums/LevelID.h"
#include "Enums/GameState.h"

#include "Interfaces/IGameEventObserver.h"
#include "Interfaces/IGameService.h"

#include "Structs/GameConfig.h"
#include "Structs/GameEntities.h"
#include "Structs/GameProgress.h"
#include "Structs/Vector2.h"

#include "Utils/GameConfigFactory.h"
#include "Utils/PlayerUtils.h"

#include <algorithm>
#include <iostream>

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

	[[nodiscard]] GameLogic::GameProgress						GetGameProgress()		const noexcept override;
	[[nodiscard]] GameLogic::PlayerState						GetPlayerState()		const noexcept override;
	[[nodiscard]] std::span<const GameLogic::EnemyState>		GetEnemiesState()		const noexcept override;
	[[nodiscard]] std::span<const GameLogic::ProjectileState>	GetProjectilesState()	const noexcept override;
	[[nodiscard]] size_t&										GetLevel()				noexcept override;
	[[nodsicard]] size_t										GetLevel()				const noexcept override;

	void SpawnEnemies() noexcept;
	
	void UpdateEnemies(float deltaTime);
	void UpdateProjectile(float deltaTime);
	void UpdateScore(GameLogic::EnemyType enemyType);

	void CheckCollissions();
	void CheckLevelCompletion() noexcept;
	void CheckPlayerDefeat()	noexcept;

	void SpawnBoss() noexcept;

	void GeneratePowerUp();
	void CollectPowerUp(const GameLogic::PowerUpState& powerUp);
	void ApplyPowerUp(GameLogic::PowerUpType type);
	
	void NotifyObservers(const GameLogic::GameEvent& event);

private:
	void UpdateEnemiesHealth();
	void UpdateEnemiesPosition(float deltaTime);

private:
	GameSession(const GameSession&) = delete;
	GameSession& operator=(const GameSession&) = delete;

	GameSession(GameSession&&) = delete;
	GameSession& operator=(GameSession&) = delete;

private:
	GameLogic::GameEntities m_entities;
	GameLogic::GameProgress m_progress;

	GameLogic::GameConfig m_gameConfig;
	GameLogic::GameState m_gameState;

	std::vector<GameLogic::IGameEventObserver*> m_observers;

	size_t m_currentLevel{};
};

