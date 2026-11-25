#include "pch.h"
#include "Utils/EnemyUtils.h"

namespace Components = GameLogic::Utils::Constants;
namespace Utils = GameLogic::Utils;

class GetEnemyHealthTests : public ::testing::Test
{
protected:
	GameLogic::GameConfig config;

	void SetUp() override
	{
		config.normalEnemyHealth = 100;
		config.armoredEnemyHealth = 200;
		config.shooterEnemyHealth = 300;
	}
};

TEST_F(GetEnemyHealthTests, ReturnsNormalHealthForNormalEnemy)
{
	EXPECT_EQ(Utils::GetEnemyHealth(GameLogic::EnemyType::NORMAL, config), 100u);
}

TEST_F(GetEnemyHealthTests, ReturnsArmoredHealthForArmoredEnemy)
{
	EXPECT_EQ(Utils::GetEnemyHealth(GameLogic::EnemyType::ARMORED, config), 200u);
}

TEST_F(GetEnemyHealthTests, ReturnsShooterHealthForShooterEnemy)
{
	EXPECT_EQ(Utils::GetEnemyHealth(GameLogic::EnemyType::SHOOTER, config), 300u);
}

TEST_F(GetEnemyHealthTests, FallsBackToNormalHealthForUnknownType)
{
	const auto unknownType = static_cast<GameLogic::EnemyType>(99);

	EXPECT_EQ(Utils::GetEnemyHealth(unknownType, config), 100u);
}
