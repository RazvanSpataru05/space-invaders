#pragma once

#include "Interfaces/IGameService.h"

#include "GameSession.h"

#include <memory>

class GameServiceFactory
{
public:
	static std::unique_ptr<GameLogic::IGameService> CreateGameService() noexcept;


private:

};

