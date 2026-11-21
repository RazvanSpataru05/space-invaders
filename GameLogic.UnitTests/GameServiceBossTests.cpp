#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceBossTests : public GameServiceFixture
{
protected:
	void SetUp() override
	{
		GameServiceFixture::SetUp();
		config.levels[0].hasBoss = true;
		AddEnemy(kTargetX, kTargetY, 50);
	}

	void DestroyLastEnemy()
	{
		ShootNow();
		ASSERT_TRUE(RunUntil([&] { return service->GetBossState().has_value(); }));
	}
};

TEST_F(GameServiceBossTests, BossAppearsInsteadOfCompletingTheLevel)
{
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::LEVEL_COMPLETED)).Times(0);

	DestroyLastEnemy();

	const auto& boss = service->GetBossState();
	EXPECT_FLOAT_EQ(boss->position.x, C::BOSS_SPAWN_POSITION.x);
	EXPECT_EQ(boss->health, 50);
	EXPECT_FALSE(boss->isVisible);
}

TEST_F(GameServiceBossTests, BossBecomesVisibleWhenItReachesItsPlace)
{
	Start();
	DestroyLastEnemy();

	service->Update(kFrame);

	EXPECT_TRUE(service->GetBossState()->isVisible);
	EXPECT_FLOAT_EQ(service->GetBossState()->position.y, C::BOSS_DESIRED_LOCATION.y);
}

TEST_F(GameServiceBossTests, PlayerCannotShootWhileBossIsArriving)
{
	Start();
	DestroyLastEnemy();
	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(0);

	service->Shoot();

	EXPECT_TRUE(service->GetPlayerProjectilesState().empty());
}

TEST_F(GameServiceBossTests, DefeatingTheBossAddsScoreAndCompletesTheLevel)
{
	Start();
	DestroyLastEnemy();
	service->Update(kFrame);
	EXPECT_CALL(observer, OnGameEvent(EventType::LEVEL_COMPLETED)).Times(1);

	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return !service->GetBossState().has_value(); }));
	Run(5);

	EXPECT_EQ(service->GetScore(), 30u + 3000u);
}

TEST_F(GameServiceBossTests, BossMovingPastTheRightEdgeTurnsAndDrops)
{
	config.bossMovementSpeed = 4500.0f;
	Start();
	DestroyLastEnemy();

	ASSERT_TRUE(RunUntil([&] { return service->GetBossState()->direction < 0.0f; }));

	const auto& boss = service->GetBossState();
	EXPECT_FLOAT_EQ(boss->position.x, 405.0f);
	EXPECT_FLOAT_EQ(boss->position.y, C::BOSS_DESIRED_LOCATION.y + config.bossDropDistance);
}

class GameServiceBossFightTests : public GameServiceBossTests
{
protected:
	void SetUp() override
	{
		GameServiceBossTests::SetUp();
		config.enemyShootCooldown = 0.0f;
		config.maxEnemyProjectileCount = 0u;
		config.bossEnemyHealth = 100u;
		config.playerProjectileDamage = 60u;
	}

	void StartFight()
	{
		Start();
		DestroyLastEnemy();
		service->Update(kFrame);
		for (int i = 0; i < 15; ++i) service->MovePlayer(GameLogic::Direction::LEFT);
		ASSERT_FLOAT_EQ(service->GetPlayerState().position.x, 420.0f);
	}
};

TEST_F(GameServiceBossFightTests, BossFiresOneProjectileFromItsCentre)
{
	StartFight();
	const size_t before = service->GetEnemyProjectilesState().size();

	service->Update(kFrame);

	const auto projectiles = service->GetEnemyProjectilesState();
	ASSERT_EQ(projectiles.size(), before + 1u);
	EXPECT_FLOAT_EQ(projectiles.back().position.x, C::BOSS_DESIRED_LOCATION.x + C::BOSS_WIDTH * 0.5f);
}

TEST_F(GameServiceBossFightTests, BossDoesNotFireBeforeItIsVisible)
{
	Start();

	DestroyLastEnemy();

	ASSERT_FALSE(service->GetBossState()->isVisible);
	EXPECT_TRUE(service->GetEnemyProjectilesState().empty());
}

TEST_F(GameServiceBossFightTests, HitBossLosesHealthAndObserverIsNotified)
{
	StartFight();
	EXPECT_CALL(observer, OnGameEvent(EventType::ENEMY_HIT)).Times(1);

	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return service->GetBossState()->health < 100; }));

	EXPECT_EQ(service->GetBossState()->health, 40);
	EXPECT_EQ(service->GetBossState()->maxHealth, 100);
}

TEST_F(GameServiceBossFightTests, BossFiresThreeProjectilesBelowHalfHealth)
{
	StartFight();
	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return service->GetBossState()->health < 100; }));

	service->Update(kFrame);

	const auto projectiles = service->GetEnemyProjectilesState();
	ASSERT_GE(projectiles.size(), 3u);
	const size_t n = projectiles.size();
	const float bossX = C::BOSS_DESIRED_LOCATION.x;
	EXPECT_FLOAT_EQ(projectiles[n - 3].position.x, bossX + C::BOSS_WIDTH * 0.5f);
	EXPECT_FLOAT_EQ(projectiles[n - 2].position.x, bossX);
	EXPECT_FLOAT_EQ(projectiles[n - 1].position.x, bossX + C::BOSS_WIDTH);
}
