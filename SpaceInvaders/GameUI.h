#pragma once

// Presentation layer: owns the window, reads input, draws the current screen and
// reacts to game events. Talks to the game logic only through IGameService.
// raylib is used only in GameUI.cpp so this header stays free of raylib names.

#include "Interfaces/IGameEventObserver.h"
#include "Interfaces/IGameService.h"

#include "ScreenType.h"
#include "StarField.h"

class GameUI final : public GameLogic::IGameEventObserver
{
public:
    // Subscribes to the service; the service must outlive this object.
    explicit GameUI(GameLogic::IGameService& gameService);
    // Unsubscribes and closes the window if it was opened.
    ~GameUI() override;

    // Opens the window and sets up raylib. Safe to call more than once.
    void Initialize();
    // Main loop. Returns when the player chooses EXIT or closes the window.
    void Run();

    // IGameEventObserver
    void OnGameEvent(const GameLogic::GameEvent& event) override;
 
private:
    GameUI(const GameUI&) = delete;
    GameUI& operator=(const GameUI&) = delete;

    GameUI(GameUI&&) = delete;
    GameUI& operator=(GameUI&&) = delete;

private:
    void HandleInput();
    void HandleMainMenuInput();
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

    // UI-only state (not in the diagram)
    StarField m_starField;
    int m_selectedMenuItem = 0;
    bool m_shouldExit = false;
    bool m_isInitialized = false;
};
