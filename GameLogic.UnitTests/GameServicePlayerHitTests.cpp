#include "pch.h"
#include "GameServiceFixture.h"

class GameServicePlayerHitTests : public GameServiceFixture
{
protected:
	void SetUp() override
	{
		GameServiceFixture::SetUp();
		config.enemyShootCooldown = 0.0f;
		AddEnemy(kTargetX, 500.0f, 150);
	}

	void WaitForHit()
	{
		ASSERT_TRUE(RunUntil([&] { return service->GetPlayerState().livesCount < config.playerLivesCount; }));
	}
};

TEST_F(GameServicePlayerHitTests, EnemyProjectileCostsThePlayerALife)
{
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_HIT)).Times(1);

	WaitForHit();
	Run(5);

	EXPECT_EQ(service->GetPlayerState().livesCount, 2u);
}

TEST_F(GameServicePlayerHitTests, HitPlayerIsHiddenWhileRespawning)
{
	Start();

	WaitForHit();

	EXPECT_FALSE(service->GetPlayerState().isVisible);
}

TEST_F(GameServicePlayerHitTests, PlayerCannotMoveOrShootWhileRespawning)
{
	Start();
	WaitForHit();
	const float xAfterHit = service->GetPlayerState().position.x;
	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(0);

	service->MovePlayer(GameLogic::Direction::LEFT);
	service->Shoot();

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x, xAfterHit);
}

TEST_F(GameServicePlayerHitTests, PlayerReappearsAtSpawnAfterRespawnTime)
{
	Start();
	for (int i = 0; i < 4; ++i) service->MovePlayer(GameLogic::Direction::RIGHT);
	ASSERT_FLOAT_EQ(service->GetPlayerState().position.x, C::PLAYER_SPAWN_POSITION.x + 16.0f);
	WaitForHit();

	service->Update(config.playerRespawnTimer);

	const auto player = service->GetPlayerState();
	EXPECT_TRUE(player.isVisible);
	EXPECT_FLOAT_EQ(player.position.x, C::PLAYER_SPAWN_POSITION.x);
	EXPECT_FLOAT_EQ(player.position.y, C::PLAYER_SPAWN_POSITION.y);
}

TEST_F(GameServicePlayerHitTests, LosingTheLastLifeEndsTheGame)
{
	config.playerLivesCount = 1u;
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::GAME_OVER)).Times(1);

	WaitForHit();
	Run(30);

	EXPECT_EQ(service->GetPlayerState().livesCount, 0u);
}
