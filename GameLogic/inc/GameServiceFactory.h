#pragma once

#include "API/Interfaces/IGameService.h"

#include "GameService.h"

#include <memory>

class GameServiceFactory
{
public:
	static std::unique_ptr<GameLogic::IGameService> CreateGameService() noexcept;


private:

};

