#include "pch.h"
#include "GameSession.h"

GameSession::GameSession(GameLogic::LevelID level) :
	m_currentLevel{ level },
	m_player{ GameLogic::PlayerState(GameLogic::Vector2(0.f, 0.f), 3u) }
{

}

void GameSession::SpawnBoss() noexcept
{
	if (m_currentLevel != GameLogic::LevelID::LEVEL_3) return;


}
