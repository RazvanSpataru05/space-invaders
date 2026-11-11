#include "GameUI.h"

#include <array>
#include <cstdint>

#include <raylib.h>

namespace
{
	// Window
	constexpr int kScreenWidth = 960;
	constexpr int kScreenHeight = 720;
	constexpr int kTargetFps = 60;
	constexpr const char* kWindowTitle = "Space Invaders";
	constexpr int kStarCount = 140;

	// Theme: black background, terminal green
	constexpr Color kBackgroundColor{ 0, 0, 0, 255 };
	constexpr Color kPrimaryGreen{ 34, 197, 34, 255 };
	constexpr Color kDimGreen{ 20, 110, 20, 255 };

	constexpr int kTitleFontSize = 60;
	constexpr int kMenuFontSize = 30;
	constexpr int kHintFontSize = 20;
	// raylib's default font is a 10 px bitmap: multiples of 10 keep it pixel-sharp.

	// Main menu
	enum class MainMenuItem
	{
		PLAY = 0,
		EXIT = 1
	};

	constexpr std::array<const char*, 2> kMainMenuLabels{ "PLAY", "EXIT" };
	constexpr int kMainMenuItemCount = static_cast<int>(kMainMenuLabels.size());

	// Input helpers
	bool IsUpPressed()
	{
		return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
	}

	bool IsDownPressed()
	{
		return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
	}

	bool IsConfirmPressed()
	{
		return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE);
	}

	bool IsBackPressed()
	{
		return IsKeyPressed(KEY_ESCAPE);
	}

	// Drawing helpers
	void DrawTextCentered(const char* text, int y, int fontSize, Color color, int xOffset = 0)
	{
		const int textWidth = MeasureText(text, fontSize);
		DrawText(text, (GetScreenWidth() - textWidth) / 2 + xOffset, y, fontSize, color);
	}

	// Width of the widest main menu label, so the selection bar never changes size.
	int WidestMainMenuLabel()
	{
		int widest = 0;
		for (const char* label : kMainMenuLabels)
		{
			const int width = MeasureText(label, kMenuFontSize);
			widest = width > widest ? width : widest;
		}
		return widest;
	}

	// Temporary screen for everything that is not implemented yet.
	void DrawPlaceholderScreen(const char* screenName)
	{
		const int centerY = GetScreenHeight() / 2;
		DrawTextCentered(screenName, centerY - 60, kMenuFontSize, kPrimaryGreen);
		DrawTextCentered("COMING SOON", centerY, kHintFontSize, kDimGreen);
		DrawTextCentered("PRESS ESC TO RETURN", centerY + 40, kHintFontSize, kDimGreen);
	}
}

GameUI::GameUI(GameLogic::IGameService& gameService)
	: m_gameService{ gameService }
{
	m_gameService.Subscribe(this);
}

GameUI::~GameUI()
{
	m_gameService.Unsubscribe(this);

	if (m_isInitialized && IsWindowReady())
	{
		CloseWindow();
	}
}

void GameUI::Initialize()
{
	if (m_isInitialized) return;

	InitWindow(kScreenWidth, kScreenHeight, kWindowTitle);
	SetExitKey(KEY_NULL); // Esc is handled per screen, not as "close window"
	SetTargetFPS(kTargetFps);

	m_starField.Initialize(kScreenWidth, kScreenHeight, kStarCount);
	m_currentScreen = ScreenType::MAIN_MENU;
	m_selectedMenuItem = 0;
	m_shouldExit = false;
	m_isInitialized = true;
}

void GameUI::Run()
{
	Initialize();

	while (!m_shouldExit && !WindowShouldClose())
	{
		const float deltaTime = GetFrameTime();

		HandleInput();
		m_starField.Update(deltaTime);

		if (m_currentScreen == ScreenType::GAMEPLAY)
		{
			m_gameService.Update(deltaTime);
			m_gameSnapshot = m_gameService.GetGameSnapshot();
		}

		Render();
	}
}

void GameUI::OnGameEvent(const GameLogic::GameEvent& event)
{
	// Only screen transitions are handled here; score, health and entities are
	// read from the snapshot that Run() refreshes every gameplay frame.
	switch (event.type)
	{
	case GameLogic::GameEventType::GAME_OVER:
		m_currentScreen = ScreenType::GAME_OVER;
		break;
	case GameLogic::GameEventType::GAME_WON:
		m_currentScreen = ScreenType::VICTORY;
		break;
	default:
		break;
	}
}

// ---------------------------------------------------------------- input

void GameUI::HandleInput()
{
	switch (m_currentScreen)
	{
	case ScreenType::MAIN_MENU:
		HandleMainMenuInput();
		break;
	case ScreenType::LEVEL_SELECTION:
	case ScreenType::DIFFICULTY_SELECTION:
	case ScreenType::GAMEPLAY:
	case ScreenType::GAME_OVER:
	case ScreenType::VICTORY:
		HandlePlaceholderInput();
		break;
	}
}

void GameUI::HandleMainMenuInput()
{
	if (IsUpPressed())
	{
		m_selectedMenuItem = (m_selectedMenuItem + kMainMenuItemCount - 1) % kMainMenuItemCount;
	}
	if (IsDownPressed())
	{
		m_selectedMenuItem = (m_selectedMenuItem + 1) % kMainMenuItemCount;
	}

	if (IsBackPressed())
	{
		m_shouldExit = true;
		return;
	}

	if (IsConfirmPressed())
	{
		switch (static_cast<MainMenuItem>(m_selectedMenuItem))
		{
		case MainMenuItem::PLAY:
			m_currentScreen = ScreenType::LEVEL_SELECTION;
			break;
		case MainMenuItem::EXIT:
			m_shouldExit = true;
			break;
		}
	}
}

void GameUI::HandlePlaceholderInput()
{
	if (IsBackPressed())
	{
		m_currentScreen = ScreenType::MAIN_MENU;
	}
}

// ---------------------------------------------------------------- rendering

void GameUI::Render()
{
	BeginDrawing();
	ClearBackground(kBackgroundColor);
	m_starField.Draw(); // shared background for every screen

	switch (m_currentScreen)
	{
	case ScreenType::MAIN_MENU:
		RenderMainMenu();
		break;
	case ScreenType::LEVEL_SELECTION:
		RenderLevelSelection();
		break;
	case ScreenType::DIFFICULTY_SELECTION:
		RenderDifficultySelection();
		break;
	case ScreenType::GAMEPLAY:
		RenderGameplay();
		break;
	case ScreenType::GAME_OVER:
		RenderGameOver();
		break;
	case ScreenType::VICTORY:
		RenderVictory();
		break;
	}

	EndDrawing();
}

void GameUI::RenderMainMenu()
{
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();

	// Title with a dim "drop shadow" and an underline
	constexpr const char* title = "SPACE INVADERS";
	constexpr int titleY = 150;
	constexpr int shadowOffset = 4;
	DrawTextCentered(title, titleY + shadowOffset, kTitleFontSize, kDimGreen, shadowOffset);
	DrawTextCentered(title, titleY, kTitleFontSize, kPrimaryGreen);

	const int titleWidth = MeasureText(title, kTitleFontSize);
	DrawRectangle((screenWidth - titleWidth) / 2, titleY + kTitleFontSize + 20, titleWidth, 4, kDimGreen);

	// Menu items: the selected one sits on a solid green bar with black text
	constexpr int firstItemY = 380;
	constexpr int itemSpacing = 64;
	constexpr int barPaddingX = 48;
	constexpr int barPaddingY = 10;
	const int barWidth = WidestMainMenuLabel() + 2 * barPaddingX;
	const int barHeight = kMenuFontSize + 2 * barPaddingY;

	for (int i = 0; i < kMainMenuItemCount; ++i)
	{
		const char* label = kMainMenuLabels[static_cast<std::size_t>(i)];
		const int y = firstItemY + i * itemSpacing;

		if (i == m_selectedMenuItem)
		{
			DrawRectangle((screenWidth - barWidth) / 2, y - barPaddingY, barWidth, barHeight, kPrimaryGreen);
			DrawTextCentered(label, y, kMenuFontSize, kBackgroundColor);
		}
		else
		{
			DrawTextCentered(label, y, kMenuFontSize, kPrimaryGreen);
		}
	}

	// Controls hint
	DrawTextCentered("UP/DOWN  SELECT     ENTER  CONFIRM     ESC  QUIT",
		screenHeight - 60, kHintFontSize, kDimGreen);
}

void GameUI::RenderLevelSelection()
{
	DrawPlaceholderScreen("LEVEL SELECTION");
}

void GameUI::RenderDifficultySelection()
{
	DrawPlaceholderScreen("DIFFICULTY SELECTION");
}

void GameUI::RenderGameplay()
{
	DrawPlaceholderScreen("GAMEPLAY");
}

void GameUI::RenderGameOver()
{
	DrawPlaceholderScreen("GAME OVER");
}

void GameUI::RenderVictory()
{
	DrawPlaceholderScreen("VICTORY");
}
