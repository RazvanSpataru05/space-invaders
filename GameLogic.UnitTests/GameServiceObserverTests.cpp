#include "pch.h"
#include "GameServiceFixture.h"

class GameServiceObserverTests : public GameServiceFixture
{};

TEST_F(GameServiceObserverTests, SubscribedObserverIsNotified)
{
	AddTargetAndBystander();
	Start();

	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(1);

	ShootNow();
}

TEST_F(GameServiceObserverTests, UnsubscribedObserverIsNotNotified)
{
	AddTargetAndBystander();
	Start();
	service->Unsubscribe(&observer);

	EXPECT_CALL(observer, OnGameEvent(_)).Times(0);

	ShootNow();
}

TEST_F(GameServiceObserverTests, SubscribingTwiceDeliversEachEventOnce)
{
	AddTargetAndBystander();
	Start();
	service->Subscribe(&observer);

	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(1);

	ShootNow();
}

TEST_F(GameServiceObserverTests, AllSubscribedObserversAreNotified)
{
	AddTargetAndBystander();
	Start();
	::testing::NiceMock<MockObserver> second;
	service->Subscribe(&second);

	EXPECT_CALL(observer, OnGameEvent(EventType::PLAYER_SHOT)).Times(1);
	EXPECT_CALL(second, OnGameEvent(EventType::PLAYER_SHOT)).Times(1);

	ShootNow();
	service->Unsubscribe(&second);
}

TEST_F(GameServiceObserverTests, SubscribingNullptrIsIgnored)
{
	AddTargetAndBystander();
	Start();
	service->Subscribe(nullptr);

	ShootNow();

	EXPECT_EQ(service->GetPlayerProjectilesState().size(), 1u);
}
