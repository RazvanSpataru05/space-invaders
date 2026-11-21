#include "pch.h"
#include "GameServiceFixture.h"

class GameServicePowerUpTests : public GameServiceFixture
{
protected:
	void SetUp() override
	{
		GameServiceFixture::SetUp();
		config.powerUpChance = 1.0f;
		AddTargetAndBystander(50);
	}

	void DestroyTarget(GameService& game)
	{
		game.Update(config.playerShootCooldown);
		game.Shoot();
		for (int i = 0; i < 100 && game.GetEnemiesState().size() > 1u; ++i) game.Update(kFrame);
	}

	uint32_t FindSeedDropping(GameLogic::PowerUpType wanted)
	{
		for (uint32_t seed = 0; seed < 1000; ++seed)
		{
			GameService probe{ config, seed };
			probe.StartLevel();
			DestroyTarget(probe);
			const auto pickups = probe.GetPowerUpPickups();
			if (!pickups.empty() && pickups[0].type == wanted) return seed;
		}
		ADD_FAILURE() << "no seed below 1000 drops power-up " << static_cast<int>(wanted);
		return 0;
	}

	void StartAndCollect(GameLogic::PowerUpType type)
	{
		Start(FindSeedDropping(type));
		DestroyTarget(*service);
		ASSERT_EQ(service->GetPowerUpPickups().size(), 1u);
		ASSERT_TRUE(RunUntil([&] { return service->GetPowerUpPickups().empty(); }));
	}
};

TEST_F(GameServicePowerUpTests, DestroyedEnemyDropsPowerUpWhereItWas)
{
	Start();

	DestroyTarget(*service);

	const auto pickups = service->GetPowerUpPickups();
	ASSERT_EQ(pickups.size(), 1u);
	EXPECT_FLOAT_EQ(pickups[0].position.x, kTargetX);
	EXPECT_GT(pickups[0].position.y, kTargetY);
}

TEST_F(GameServicePowerUpTests, NoPowerUpWhenChanceIsZero)
{
	config.powerUpChance = 0.0f;
	Start();

	DestroyTarget(*service);

	EXPECT_TRUE(service->GetPowerUpPickups().empty());
}

TEST_F(GameServicePowerUpTests, PowerUpFallsAtDropSpeed)
{
	Start();
	DestroyTarget(*service);
	const float y = service->GetPowerUpPickups()[0].position.y;

	service->Update(kFrame);

	EXPECT_FLOAT_EQ(service->GetPowerUpPickups()[0].position.y, y + 40.0f);
}

TEST_F(GameServicePowerUpTests, BonusScorePowerUpAddsBonusScore)
{
	StartAndCollect(GameLogic::PowerUpType::BONUS_SCORE);

	EXPECT_EQ(service->GetScore(), 30u + 1000u);
}

TEST_F(GameServicePowerUpTests, MultiShotPowerUpFiresThreeProjectiles)
{
	StartAndCollect(GameLogic::PowerUpType::MULTI_SHOT);

	ShootNow();

	EXPECT_EQ(service->GetPlayerProjectilesState().size(), 3u);
}

TEST_F(GameServicePowerUpTests, RapidFirePowerUpHalvesTheShootCooldown)
{
	StartAndCollect(GameLogic::PowerUpType::RAPID_FIRE);

	service->Update(config.playerShootCooldown);
	service->Shoot();
	service->Update(config.playerShootCooldown / 2.0f);
	service->Shoot();

	EXPECT_EQ(service->GetPlayerProjectilesState().size(), 2u);
}

TEST_F(GameServicePowerUpTests, ExtraLifeDoesNotGoAboveStartingLives)
{
	StartAndCollect(GameLogic::PowerUpType::EXTRA_LIFE);

	EXPECT_EQ(service->GetPlayerState().livesCount, config.playerLivesCount);
}
