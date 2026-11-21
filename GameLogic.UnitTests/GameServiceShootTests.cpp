#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceShootTests : public GameServiceFixture
{};

TEST_F(GameServiceShootTests, ShootCreatesProjectileFromTheMiddleOfTheShip)
{
	AddTargetAndBystander();
	Start();

	ShootNow();

	const auto projectiles = service->GetPlayerProjectilesState();
	ASSERT_EQ(projectiles.size(), 1u);
	EXPECT_FLOAT_EQ(projectiles[0].position.x, C::PLAYER_SPAWN_POSITION.x + C::SHIP_WIDTH * 0.5f);
	EXPECT_FLOAT_EQ(projectiles[0].position.y, C::PLAYER_SPAWN_POSITION.y);
	EXPECT_FLOAT_EQ(projectiles[0].velocity, -400.0f);
	EXPECT_EQ(projectiles[0].damage, 50u);
}

TEST_F(GameServiceShootTests, CannotShootAgainBeforeCooldown)
{
	AddTargetAndBystander();
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(1);

	ShootNow();
	service->Shoot();

	EXPECT_EQ(service->GetPlayerProjectilesState().size(), 1u);
}

TEST_F(GameServiceShootTests, CanShootAgainAfterCooldown)
{
	AddTargetAndBystander();
	Start();

	ShootNow();
	ShootNow();

	EXPECT_EQ(service->GetPlayerProjectilesState().size(), 2u);
}

TEST_F(GameServiceShootTests, CannotShootDuringStartCooldown)
{
	config.startCooldown = 2.0f;
	AddTargetAndBystander();
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(0);

	ShootNow();

	EXPECT_TRUE(service->GetPlayerProjectilesState().empty());
}

TEST_F(GameServiceShootTests, PlayerProjectileMovesUpEachFrame)
{
	AddTargetAndBystander();
	Start();
	ShootNow();

	service->Update(kFrame);

	ASSERT_EQ(service->GetPlayerProjectilesState().size(), 1u);
	EXPECT_FLOAT_EQ(service->GetPlayerProjectilesState()[0].position.y, C::PLAYER_SPAWN_POSITION.y - 40.0f);
}

TEST_F(GameServiceShootTests, PlayerProjectileIsRemovedAfterLeavingTheScreen)
{
	AddEnemy(100.0f, 100.0f, 150);
	Start();
	ShootNow();

	Run(20);

	EXPECT_TRUE(service->GetPlayerProjectilesState().empty());
}
