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

#include <algorithm>

class GameSession final : public GameLogic::IGameService
{
public:
	explicit GameSession();

	void Subscribe(GameLogic::IGameEventObserver* observer)		override;
	void Unsubscribe(GameLogic::IGameEventObserver* observer)	override;

	void StartLevel(GameLogic::LevelID level) override;

	void Update(float deltaTime) override;

	void MovePlayer(GameLogic::Direction direction) override;
	void Shoot() override;

	void SpawnEnemies() noexcept;
	
	void UpdateEnemies(float deltaTime);
	void UpdateProjectile(float deltaTime);

	void CheckCollissions();
	void CheckLevelCompletion() const noexcept;
	void CheckPlayerDefeat()	const noexcept;

	void SpawnBoss() noexcept;

	void GeneratePowerUp(GameLogic::EnemyType enemyType);
	void CollectPowerUp(const GameLogic::PowerUpState& powerUp);
	void ApplyPowerUp(GameLogic::PowerUpType type);
	
	void NotifyObservers(const GameLogic::GameEvent& event);
	void CalculateEnemyScore(GameLogic::EnemyType enemyType);

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
};

