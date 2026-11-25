#pragma once

#include "GameService.h"

#include <gmock/gmock.h>

#include <algorithm>
#include <functional>
#include <memory>

namespace C = GameLogic::Utils::Constants;
using GameLogic::EventType;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;

class MockObserver : public GameLogic::IGameEventObserver
{
public:
	MOCK_METHOD(void, OnGameEvent, (GameLogic::EventType event), (override));
};

class GameServiceFixture : public ::testing::Test
{
protected:
	static constexpr float kFrame = 0.1f;

	static constexpr float kTargetX = 480.0f;
	static constexpr float kTargetY = 300.0f;

	GameLogic::GameConfig config;
	::testing::NiceMock<MockObserver> observer;
	std::unique_ptr<GameService> service;

	void SetUp() override
	{
		config.startCooldown = 0.0f;

		config.playerLivesCount = 3u;
		config.playerMovementSpeed = 4.0f;
		config.playerShootCooldown = 0.2f;
		config.playerRespawnTimer = 2.0f;
		config.projectileSpeed = 400.0f;
		config.playerProjectileDamage = 50u;
		config.enemyProjectileDamage = 1u;

		config.enemyMovementSpeed = 0.0f;
		config.enemyDropDistance = 0.0f;
		config.enemyShootCooldown = 1000.0f;
		config.maxEnemyProjectileCount = 1u;

		config.normalEnemyHealth = 150u;
		config.armoredEnemyHealth = 300u;
		config.shooterEnemyHealth = 200u;
		config.normalEnemyScore = 30u;
		config.armoredEnemyScore = 50u;
		config.shooterEnemyScore = 80u;

		config.ufoSpawnCooldown = 1000.0f;
		config.countUFOSpawn = 100u;
		config.ufoMovementSpeed = 0.0f;

		config.powerUpChance = 0.0f;
		config.powerUpDuration = 10.0f;
		config.powerUpDropDistance = 400.0f;
		config.powerUpBonusScore = 1000u;

		config.bossEnemyHealth = 50u;
		config.bossScore = 3000u;
		config.bossMovementSpeed = 0.0f;
		config.bossDropDistance = 2050.0f;

		EXPECT_CALL(observer, OnGameEvent(_)).Times(AnyNumber());
	}

	void TearDown() override
	{
		if (service) service->Unsubscribe(&observer);
	}

	void AddEnemy(float x, float y, int16_t health,
		GameLogic::EnemyType type = GameLogic::EnemyType::NORMAL, size_t level = 0)
	{
		config.levels[level].enemies.push_back(
			{ GameLogic::Vector2{ x, y }, static_cast<uint16_t>(health), type });
	}

	void AddTargetAndBystander(int16_t targetHealth = 50,
		GameLogic::EnemyType targetType = GameLogic::EnemyType::NORMAL)
	{
		AddEnemy(kTargetX, kTargetY, targetHealth, targetType);
		AddEnemy(100.0f, 100.0f, 150);
	}

	void Start(uint32_t seed = 1u)
	{
		service = std::make_unique<GameService>(config, seed);
		service->Subscribe(&observer);
		service->StartLevel();
	}

	void Run(int frames)
	{
		for (int i = 0; i < frames; ++i) service->Update(kFrame);
	}

	bool RunUntil(const std::function<bool()>& condition, int maxFrames = 300)
	{
		for (int i = 0; i < maxFrames; ++i)
		{
			if (condition()) return true;
			service->Update(kFrame);
		}
		return condition();
	}

	void ShootNow()
	{
		service->Update(config.playerShootCooldown);
		service->Shoot();
	}

	size_t EnemyCount() const { return service->GetEnemiesState().size(); }
};
