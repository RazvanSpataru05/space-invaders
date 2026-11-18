#include "pch.h"
#include "GameServiceFactory.h"

std::unique_ptr<GameLogic::IGameService> GameServiceFactory::CreateGameService() noexcept
{
	return std::make_unique<GameSession>();
}

