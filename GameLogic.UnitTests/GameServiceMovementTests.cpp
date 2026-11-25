#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceMovementTests : public GameServiceFixture
{};

TEST_F(GameServiceMovementTests, MovePlayerLeftMovesBySpeed)
{
	AddTargetAndBystander();
	Start();

	service->MovePlayer(GameLogic::Direction::LEFT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x, C::PLAYER_SPAWN_POSITION.x - 4.0f);
}

TEST_F(GameServiceMovementTests, MovePlayerRightMovesBySpeed)
{
	AddTargetAndBystander();
	Start();

	service->MovePlayer(GameLogic::Direction::RIGHT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x, C::PLAYER_SPAWN_POSITION.x + 4.0f);
}

TEST_F(GameServiceMovementTests, MovePlayerDoesNotChangeHeight)
{
	AddTargetAndBystander();
	Start();

	service->MovePlayer(GameLogic::Direction::RIGHT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.y, C::PLAYER_SPAWN_POSITION.y);
}

TEST_F(GameServiceMovementTests, PlayerStopsAtLeftLimit)
{
	AddTargetAndBystander();
	Start();

	for (int i = 0; i < 500; ++i) service->MovePlayer(GameLogic::Direction::LEFT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x, C::WIDTH_OFFSET);
}

TEST_F(GameServiceMovementTests, PlayerStopsAtRightLimit)
{
	AddTargetAndBystander();
	Start();

	for (int i = 0; i < 500; ++i) service->MovePlayer(GameLogic::Direction::RIGHT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x,
		C::SCREEN_WIDTH - C::WIDTH_OFFSET - C::SHIP_WIDTH);
}

TEST_F(GameServiceMovementTests, PlayerCannotMoveDuringStartCooldown)
{
	config.startCooldown = 2.0f;
	AddTargetAndBystander();
	Start();

	service->MovePlayer(GameLogic::Direction::RIGHT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x, C::PLAYER_SPAWN_POSITION.x);
}

TEST_F(GameServiceMovementTests, PlayerCanMoveAfterStartCooldown)
{
	config.startCooldown = 2.0f;
	AddTargetAndBystander();
	Start();

	service->Update(2.0f);
	service->MovePlayer(GameLogic::Direction::RIGHT);

	EXPECT_FLOAT_EQ(service->GetPlayerState().position.x, C::PLAYER_SPAWN_POSITION.x + 4.0f);
}
