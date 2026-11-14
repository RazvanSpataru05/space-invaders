#pragma once

// Presentation layer: owns the window, reads input, draws the current screen and
// reacts to game events. Talks to the game logic only through IGameService.
// raylib is used only in GameUI.cpp so this header stays free of raylib names.

#include "Interfaces/IGameEventObserver.h"
#include "Interfaces/IGameService.h"

#include "Structs/GameProgress.h"
#include "Structs/PlayerState.h"

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
    void HandleVerdictInput();
    void HandleVerdictPreviewInput();
    void HandleGameplayInput();

    void Render();
    void RenderMainMenu() const;
    void RenderLevelSelection() const;
    void RenderGameplay();
    void RenderGameOver() const;
    void RenderVictory() const;
    void RenderVerdictPreview() const;

    void ShowVerdict(ScreenType verdictScreen);
    void StartSelectedGame();

private:

    GameLogic::IGameService& m_gameService;

    ScreenType m_currentScreen = ScreenType::MAIN_MENU;
    GameLogic::GameProgress m_gameProgress{};
    GameLogic::PlayerState m_playerState{};

    StarField m_starField;
    int m_selectedMenuItem = 0;
    int m_selectedLevel = 0;
    int m_selectedVerdictOption = 0;
    int m_selectedVerdictPreview = 0;
    bool m_shouldExit = false;
    bool m_isInitialized = false;
};
