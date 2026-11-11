#pragma once

// Presentation layer: owns the window, reads input, draws the current screen and
// reacts to game events. Talks to the game logic only through IGameService.
// raylib is used only in GameUI.cpp so this header stays free of raylib names.

#include "Interfaces/IGameEventObserver.h"
#include "Interfaces/IGameService.h"

#include "Structs/GameSnapshot.h"

#include "ScreenType.h"
#include "StarField.h"

class GameUI final : public GameLogic::IGameEventObserver
{
public:
    explicit GameUI(GameLogic::IGameService& gameService);
    ~GameUI() override;


    void Initialize();
    void Run();
    void OnGameEvent(const GameLogic::GameEvent& event) override;
 
private:
    GameUI(const GameUI&) = delete;
    GameUI& operator=(const GameUI&) = delete;

    GameUI(GameUI&&) = delete;
    GameUI& operator=(GameUI&&) = delete;

private:
    void HandleInput();
    void HandleMainMenuInput();
    void HandleLevelSelectionInput();
    void HandlePlaceholderInput();

    void Render();
    void RenderMainMenu();
    void RenderLevelSelection();
    void RenderDifficultySelection();
    void RenderGameplay();
    void RenderGameOver();
    void RenderVictory();

    // From the class diagram
    GameLogic::IGameService& m_gameService;
    ScreenType m_currentScreen = ScreenType::MAIN_MENU;
    GameLogic::GameSnapshot m_gameSnapshot{};

    // UI-only state (not in the diagram)
    StarField m_starField;
    int m_selectedMenuItem = 0;
    int m_selectedLevel = 0; // 0..2 -> LevelId::LEVEL_1..LEVEL_3
    bool m_shouldExit = false;
    bool m_isInitialized = false;
};
