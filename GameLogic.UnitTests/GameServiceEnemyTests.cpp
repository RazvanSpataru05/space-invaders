#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceEnemyTests : public GameServiceFixture
{};

TEST_F(GameServiceEnemyTests, HitEnemyLosesHealthAndObserverIsNotified)
{
	AddTargetAndBystander(150);
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::ENEMY_HIT)).Times(1);
	EXPECT_CALL(observer, OnGameEvent(EventType::ENEMY_DESTROYED)).Times(0);

	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return service->GetPlayerProjectilesState().empty(); }));

	EXPECT_EQ(service->GetEnemiesState()[0].health, 100);
}

TEST_F(GameServiceEnemyTests, DestroyedEnemyIsRemovedAndObserverIsNotified)
{
	AddTargetAndBystander(50);
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::ENEMY_DESTROYED)).Times(1);
	EXPECT_CALL(observer, OnGameEvent(EventType::ENEMY_HIT)).Times(0);

	ShootNow();

	ASSERT_TRUE(RunUntil([&] { return EnemyCount() == 1u; }));
	EXPECT_FLOAT_EQ(service->GetEnemiesState()[0].position.x, 100.0f);
}

TEST_F(GameServiceEnemyTests, DestroyingNormalEnemyAddsNormalScore)
{
	AddTargetAndBystander(50, GameLogic::EnemyType::NORMAL);
	Start();

	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return EnemyCount() == 1u; }));

	EXPECT_EQ(service->GetScore(), 30u);
}

TEST_F(GameServiceEnemyTests, DestroyingArmoredEnemyAddsArmoredScore)
{
	AddTargetAndBystander(50, GameLogic::EnemyType::ARMORED);
	Start();

	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return EnemyCount() == 1u; }));

	EXPECT_EQ(service->GetScore(), 50u);
}

TEST_F(GameServiceEnemyTests, DestroyingShooterEnemyAddsShooterScore)
{
	AddTargetAndBystander(50, GameLogic::EnemyType::SHOOTER);
	Start();

	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return EnemyCount() == 1u; }));

	EXPECT_EQ(service->GetScore(), 80u);
}

TEST_F(GameServiceEnemyTests, DestroyingLastEnemyCompletesTheLevel)
{
	AddEnemy(kTargetX, kTargetY, 50);
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::LEVEL_COMPLETED)).Times(1);

	ShootNow();
	Run(30);
}

TEST_F(GameServiceEnemyTests, NothingHappensAfterTheLevelIsCompleted)
{
	AddEnemy(kTargetX, kTargetY, 50);
	Start();
	ShootNow();
	Run(30);
	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(0);

	ShootNow();
	service->MovePlayer(GameLogic::Direction::RIGHT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x, C::PLAYER_SPAWN_POSITION.x);
}

TEST_F(GameServiceEnemyTests, EnemiesTurnAroundAndDropAtTheRightEdge)
{
	config.enemyMovementSpeed = 600.0f;
	config.enemyDropDistance = 100.0f;
	AddEnemy(870.0f, 100.0f, 150);
	Start();

	service->Update(kFrame);

	const auto enemy = service->GetEnemiesState()[0];
	EXPECT_FLOAT_EQ(enemy.position.x, 930.0f);
	EXPECT_FLOAT_EQ(enemy.position.y, 110.0f);

	service->Update(kFrame);

	EXPECT_FLOAT_EQ(service->GetEnemiesState()[0].position.x, 870.0f);
}

TEST_F(GameServiceEnemyTests, NormalEnemyFiresOneProjectileBelowItsCentre)
{
	config.enemyShootCooldown = 0.0f;
	config.maxEnemyProjectileCount = 3u;
	AddEnemy(100.0f, 100.0f, 150, GameLogic::EnemyType::NORMAL);
	Start();

	service->Update(kFrame);

	const auto projectiles = service->GetEnemyProjectilesState();
	ASSERT_EQ(projectiles.size(), 1u);
	EXPECT_FLOAT_EQ(projectiles[0].position.x, 100.0f + C::SHIP_WIDTH * 0.5f);
	EXPECT_FLOAT_EQ(projectiles[0].velocity, 400.0f);
	EXPECT_EQ(projectiles[0].damage, 1u);
}

TEST_F(GameServiceEnemyTests, ShooterEnemyFiresThreeProjectiles)
{
	config.enemyShootCooldown = 0.0f;
	config.maxEnemyProjectileCount = 3u;
	AddEnemy(100.0f, 100.0f, 150, GameLogic::EnemyType::SHOOTER);
	Start();

	service->Update(kFrame);

	const auto projectiles = service->GetEnemyProjectilesState();
	ASSERT_EQ(projectiles.size(), 3u);
	EXPECT_FLOAT_EQ(projectiles[0].position.x, 100.0f + C::SHIP_WIDTH * 0.5f);
	EXPECT_FLOAT_EQ(projectiles[1].position.x, 100.0f);
	EXPECT_FLOAT_EQ(projectiles[2].position.x, 100.0f + C::SHIP_WIDTH);
}

TEST_F(GameServiceEnemyTests, EnemiesDoNotFireMoreThanTheProjectileLimit)
{
	config.enemyShootCooldown = 0.0f;
	config.maxEnemyProjectileCount = 2u;
	AddEnemy(100.0f, 100.0f, 150);
	Start();

	Run(5);

	EXPECT_EQ(service->GetEnemyProjectilesState().size(), 2u);
}

TEST_F(GameServiceEnemyTests, TouchingAnEnemyCostsALife)
{
	AddEnemy(C::PLAYER_SPAWN_POSITION.x, C::PLAYER_SPAWN_POSITION.y, 150);
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_HIT)).Times(1);

	service->Update(kFrame);

	EXPECT_EQ(service->GetPlayerState().livesCount, 2u);
}
