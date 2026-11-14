#include "GameUI.h"
#include "GameSession.h"

int main()
{
    GameSession gameSession;

    GameUI gameUI(gameSession);

    gameUI.Initialize();
    gameUI.Run();
}