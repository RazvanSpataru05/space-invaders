#include "pch.h"
#include "GameSession.h"

GameSession::GameSession(std::unique_ptr<GameLogic::IDifficultyStrategy> difficultyStrategy,
	GameLogic::LevelID level) :
	m_difficultyStrategy{ std::move(difficultyStrategy) },
	m_currentLevel{ level },
	m_player{ GameLogic::PlayerState(GameLogic::Vector2(0.f, 0.f), m_difficultyStrategy->GetPlayerLivesCount()) }
{

}

void GameSession::SpawnBoss() noexcept
{
	if (m_currentLevel != GameLogic::LevelID::LEVEL_3) return;


}
