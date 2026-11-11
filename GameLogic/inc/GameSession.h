#pragma once

#include "Enums/LevelID.h"
#include "Enums/GameState.h"

#include "Interfaces/IGameEventObserver.h"
#include "Interfaces/IGameService.h"

#include "Structs/EnemyState.h"
#include "Structs/ProjectileState.h"
#include "Structs/GameConfig.h"
#include "Structs/PowerUpState.h"
#include "Structs/PlayerState.h"
#include "Structs/Vector2.h"

#include <vector>
#include <algorithm>

class GameSession final : public GameLogic::IGameService
{
public:
	explicit GameSession(GameLogic::LevelID level);

	void Subscribe(GameLogic::IGameEventObserver* observer)		override;
	void Unsubscribe(GameLogic::IGameEventObserver* observer)	override;

	void SpawnEnemies() noexcept;
	
	void UpdateEnemies(float deltaTime);
	void UpdateProjectile(float deltaTime);

	void CheckCollissions();
	void CheckLevelCompletion() const noexcept;
	void CheckPlayerDefeat() const noexcept;

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
	GameLogic::PlayerState m_player;
	std::vector<GameLogic::EnemyState> m_enemies;
	std::vector<GameLogic::ProjectileState> m_projectiles;
	std::vector<GameLogic::PowerUpState> m_powerUps;

	std::vector<GameLogic::IGameEventObserver*> m_observers;
	GameLogic::GameConfig m_gameConfig;
	GameLogic::GameState m_gameState;
	GameLogic::LevelID m_currentLevel;

	uint32_t m_score{};
	float m_elapsedTime{};
};

