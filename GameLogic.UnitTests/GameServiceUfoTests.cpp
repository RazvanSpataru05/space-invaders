#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceUfoTests : public GameServiceFixture
{
protected:
	void SetUp() override
	{
		GameServiceFixture::SetUp();
		config.countUFOSpawn = 1u;
		config.ufoSpawnCooldown = 5.0f;
		AddEnemy(100.0f, 100.0f, 150);
	}
};

TEST_F(GameServiceUfoTests, UfoIsAnnouncedAndThenSpawned)
{
	Start();
	{
		InSequence order;
		EXPECT_CALL(observer, OnGameEvent(EventType::UFO_APPROACHING)).Times(1);
		EXPECT_CALL(observer, OnGameEvent(EventType::UFO_SPAWNED)).Times(1);
	}

	Run(60);

	EXPECT_TRUE(service->GetUFOState().has_value());
}

TEST_F(GameServiceUfoTests, NoUfoBeforeTheCooldown)
{
	Start();

	Run(45);

	EXPECT_FALSE(service->GetUFOState().has_value());
}

TEST_F(GameServiceUfoTests, UfoComesFromTheRightWhenShotCountIsEven)
{
	Start();

	Run(60);

	ASSERT_TRUE(service->GetUFOState().has_value());
	EXPECT_FLOAT_EQ(service->GetUFOState()->position.x, C::UFO_RIGHT_SPAWN_POSITION.x);
	EXPECT_FLOAT_EQ(service->GetUFOState()->direction, -1.0f);
}

TEST_F(GameServiceUfoTests, UfoComesFromTheLeftWhenShotCountIsOdd)
{
	Start();
	ShootNow();

	Run(60);

	ASSERT_TRUE(service->GetUFOState().has_value());
	EXPECT_FLOAT_EQ(service->GetUFOState()->position.x, C::UFO_LEFT_SPAWN_POSITION.x);
	EXPECT_FLOAT_EQ(service->GetUFOState()->direction, 1.0f);
}

TEST_F(GameServiceUfoTests, UfoWarnsBeforeLeavingAndDisappearsOffScreen)
{
	config.ufoMovementSpeed = 2000.0f;
	Start();
	{
		InSequence order;
		EXPECT_CALL(observer, OnGameEvent(EventType::UFO_SPAWNED)).Times(1);
		EXPECT_CALL(observer, OnGameEvent(EventType::UFO_LEAVING)).Times(1);
		EXPECT_CALL(observer, OnGameEvent(EventType::UFO_DESPAWNED)).Times(1);
	}

	ASSERT_TRUE(RunUntil([&] { return service->GetUFOState().has_value(); }));
	ASSERT_TRUE(RunUntil([&] { return !service->GetUFOState().has_value(); }));
}

TEST_F(GameServiceUfoTests, ApproachIsCancelledWhenTooFewEnemiesRemain)
{
	config.levels[0].enemies.clear();
	config.countUFOSpawn = 2u;
	AddTargetAndBystander(50);
	Start();
	EXPECT_CALL(observer, OnGameEvent(EventType::UFO_APPROACH_CANCELLED)).Times(1);
	EXPECT_CALL(observer, OnGameEvent(EventType::UFO_SPAWNED)).Times(0);

	Run(25);
	ShootNow();
	ASSERT_TRUE(RunUntil([&] { return EnemyCount() == 1u; }));
	Run(40);
}

TEST_F(GameServiceUfoTests, HittingTheUfoAddsAScoreFromItsTable)
{
	config.ufoMovementSpeed = 100.0f;
	Start();
	ASSERT_TRUE(RunUntil([&] { return service->GetUFOState().has_value(); }));

	const bool hit = RunUntil([&] {
		service->Shoot();
		return service->GetScore() > 0u;
		}, 200);

	ASSERT_TRUE(hit);
	const auto& table = service->GetUFOState()->scores;
	EXPECT_NE(std::find(table.begin(), table.end(), service->GetScore()), table.end());
}
