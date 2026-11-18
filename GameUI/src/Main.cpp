#include "GameUI.h"
#include "GameServiceFactory.h"

int main()
{
	GameUI gameUI = GameUI(GameServiceFactory::CreateGameService());

	gameUI.Initialize();
	gameUI.Run();
}