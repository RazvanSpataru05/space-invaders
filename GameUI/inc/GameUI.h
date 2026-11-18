#pragma once

#include "Enums/EnemyType.h"
#include "Enums/EventType.h"

#include "API/Interfaces/IGameEventObserver.h"
#include "API/Interfaces/IGameService.h"

#include "API/Structs/GameProgress.h"

#include "Structs/GameEvent.h"
#include "Structs/PlayerState.h"
#include "Structs/EnemyState.h"

#include "MainMenuItem.h"
#include "PauseMenuItem.h"
#include "VerdictMenuItem.h"

#include "ScreenType.h"
#include "StarField.h"

#include <raylib.h>

#include <algorithm>
#include <array>
#include <span>
#include <string>
#include <cstdint>
#include <iostream>

class GameUI final : public GameLogic::IGameEventObserver
{
public:
    explicit GameUI(std::unique_ptr<GameLogic::IGameService> gameService);

    ~GameUI() override;

    void Initialize();
    void Run();
    void OnGameEvent(const GameLogic::GameEvent& event) override;

private:
    void HandleInput();
    void HandleMainMenuInput();
    void HandleLevelSelectionInput();
    void HandleVerdictInput();
    void HandleGameplayInput();
    void HandlePauseInput();

    void Render();
    void RenderMainMenu() const;
    void RenderLevelSelection() const;
    void RenderGameplay();
    void RenderPause();
    void RenderGameOver() const;
    void RenderVictory() const;

    void ShowVerdict(ScreenType verdictScreen);
    void StartSelectedGame();

private:
    GameUI(const GameUI&) = delete;
    GameUI& operator=(const GameUI&) = delete;

    GameUI(GameUI&&) = delete;
    GameUI& operator=(GameUI&&) = delete;

private:

    std::unique_ptr<GameLogic::IGameService> m_gameService;
    ScreenType m_currentScreen = ScreenType::MAIN_MENU;

    StarField m_starField;
    size_t m_selectedMenuItem{};
    size_t m_selectedVerdictOption{};
    size_t m_selectedPauseOption{};
    bool m_shouldExit{};
    bool m_isInitialized{};
};
