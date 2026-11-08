#include "GameUI.h"
#include "TemporaryGameService.h"

int main()
{
    // TODO: replace with GameLogic::GameSession once it is implemented.
    TemporaryGameService gameService;

    // Declared after the service, so it is destroyed first and unsubscribes while the service still exists.
    GameUI gameUI(gameService);

    gameUI.Initialize();
    gameUI.Run();

    return 0;
}
