#include "GameUI.h"

namespace
{
	// Window
	constexpr uint16_t kScreenWidth = 960;
	constexpr uint16_t kScreenHeight = 720;

	constexpr uint8_t kTargetFps = 60;
	constexpr const char* kWindowTitle = "Space Invaders";
	constexpr uint8_t kStarCount = 140;

	// Theme: black background, terminal green
	constexpr Color kBackgroundColor{ 0, 0, 0, 255 };
	constexpr Color kPrimaryGreen{ 34, 197, 34, 255 };
	constexpr Color kDimGreen{ 20, 110, 20, 255 };

	constexpr uint8_t kTitleFontSize = 60;
	constexpr uint8_t kScreenTitleFontSize = 40;
	constexpr uint8_t kMenuFontSize = 30;
	constexpr uint8_t kHintFontSize = 20;

	// Main menu
	constexpr std::array<MainMenuItem, 2> kMainMenuItems{
		MainMenuItem::PLAY,
		MainMenuItem::EXIT };

	constexpr std::array<const char*, 2> kMainMenuLabels{ "PLAY", "EXIT" };

	// Level selection
	constexpr std::array<const char*, 3> kLevelLabels{ "1", "2", "3" };

	constexpr uint8_t kLevelLabelFontSize = 60;

	const char* VerdictLabel(VerdictMenuItem item)
	{
		switch (item)
		{
		case VerdictMenuItem::NEXT_LEVEL:	return "NEXT LEVEL";
		case VerdictMenuItem::RETRY:		return "RETRY";
		case VerdictMenuItem::MAIN_MENU:	return "MAIN MENU";
		}
		return "?";
	}

	// Pause menu
	constexpr std::array<PauseMenuItem, 3> kPauseMenuItems{
		PauseMenuItem::RESUME,
		PauseMenuItem::RETRY,
		PauseMenuItem::MAIN_MENU };

	constexpr std::array<const char*, 3> kPauseMenuLabels{ "RESUME", "RETRY", "MAIN MENU" };

	std::vector<VerdictMenuItem> GetVerdictItems(bool isVictory, size_t level)
	{
		std::vector<VerdictMenuItem> items;

		if (isVictory && level < 2)
		{
			items.emplace_back(VerdictMenuItem::NEXT_LEVEL);
		}

		items.emplace_back(VerdictMenuItem::RETRY);
		items.emplace_back(VerdictMenuItem::MAIN_MENU);
		return items;
	}

	std::vector<const char*> GetVerdictLabels(const std::vector<VerdictMenuItem>& items)
	{
		std::vector<const char*> labels;
		labels.reserve(items.size());

		for (VerdictMenuItem item : items)
		{
			labels.emplace_back(VerdictLabel(item));
		}
		
		return labels;
	}

	bool IsUpPressed()
	{
		return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
	}

	bool IsDownPressed()
	{
		return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
	}

	bool IsLeftPressed()
	{
		return IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
	}

	bool IsSpacePressed()
	{
		return IsKeyPressed(KEY_SPACE);
	}

	bool IsLeftHold()
	{
		return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
	}

	bool IsRightHold()
	{
		return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
	}

	bool IsRightPressed()
	{
		return IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);
	}

	bool IsConfirmPressed()
	{
		return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
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

	// Screen heading: text with a dim "drop shadow" and an underline.
	void DrawScreenTitle(const char* title, int y, int fontSize)
	{
		constexpr int shadowOffset = 4;
		DrawTextCentered(title, y + shadowOffset, fontSize, kDimGreen, shadowOffset);
		DrawTextCentered(title, y, fontSize, kPrimaryGreen);

		const int titleWidth = MeasureText(title, fontSize);
		DrawRectangle((GetScreenWidth() - titleWidth) / 2, y + fontSize + 20, titleWidth, 4, kDimGreen);
	}

	// Vertical list of labels; the selected one sits on a solid green bar with black text.
	// The bar is sized to the widest label so it never changes size while navigating.
	void DrawMenuList(std::span<const char* const> labels, size_t selectedIndex, int firstItemY)
	{
		constexpr int itemSpacing = 64;
		constexpr int barPaddingX = 48;
		constexpr int barPaddingY = 10;

		int widestLabel = 0;
		for (const char* label : labels)
		{
			const int width = MeasureText(label, kMenuFontSize);
			widestLabel = width > widestLabel ? width : widestLabel;
		}

		const int barWidth = widestLabel + 2 * barPaddingX;
		const int barHeight = kMenuFontSize + 2 * barPaddingY;

		for (size_t i = 0; i < labels.size(); ++i)
		{
			const int y = firstItemY + static_cast<int>(i) * itemSpacing;

			if (static_cast<int>(i) == selectedIndex)
			{
				DrawRectangle((GetScreenWidth() - barWidth) / 2, y - barPaddingY, barWidth, barHeight, kPrimaryGreen);
				DrawTextCentered(labels[i], y, kMenuFontSize, kBackgroundColor);
			}
			else
			{
				DrawTextCentered(labels[i], y, kMenuFontSize, kPrimaryGreen);
			}
		}
	}

	// Moves a list cursor with wrap-around.
	size_t StepSelection(size_t current, int step, size_t count)
	{
		return (current + step + count) % count;
	}

	const char* LevelLabel(size_t level)
	{
		for (size_t i = 0; i < kLevelLabels.size(); ++i)
		{
			if (i == level)
			{
				return kLevelLabels[i];
			}
		}
		return "?";
	}

	// Shared layout of the Game Over and Victory screens.
	void DrawVerdictScreen(const char* title, const char* subtitle, const char* details, uint32_t score,
		std::span<const char* const> options, size_t selectedOption)
	{
		DrawScreenTitle(title, 120, kTitleFontSize);
		DrawTextCentered(subtitle, 230, kHintFontSize, kPrimaryGreen);
		DrawTextCentered(details, 260, kHintFontSize, kDimGreen);

		DrawTextCentered("FINAL SCORE", 320, kHintFontSize, kDimGreen);
		DrawTextCentered(TextFormat("%06u", score), 350, kTitleFontSize, kPrimaryGreen);

		DrawMenuList(options, selectedOption, 470);

		DrawTextCentered("UP/DOWN  SELECT     ENTER  CONFIRM     ESC  MENU",
			GetScreenHeight() - 60, kHintFontSize, kDimGreen);
	}

	// Gameplay HUD
	constexpr int kMaxLives = 3;
	constexpr int kHudTextY = 20;
	constexpr int kHudMarginX = 30;
	constexpr int kHudDividerY = 60;
	constexpr int kGroundY = 676;

	// Pixel heart used for the lives counter ('#' = pixel).
	constexpr std::array<const char*, 7> kHeartSprite{
		".##.##.",
		"#######",
		"#######",
		"#######",
		".#####.",
		"..###..",
		"...#...",
	};
	constexpr int kHeartSpriteWidth = 7;
	constexpr int kHeartPixelSize = 4;
	constexpr int kHeartWidth = kHeartSpriteWidth * kHeartPixelSize;
	constexpr int kHeartHeight = static_cast<int>(kHeartSprite.size()) * kHeartPixelSize;
	constexpr int kHeartGap = 10;

	// Draws a sprite given as rows of text ('#' = pixel), each pixel as a pixelSize square.
	void DrawPixelSprite(std::span<const char* const> rows, int x, int y, int pixelSize, Color color)
	{
		for (size_t row = 0; row < rows.size(); ++row)
		{
			for (int column = 0; rows[row][column] != '\0'; ++column)
			{
				if (rows[row][column] == '#')
				{
					DrawRectangle(x + column * pixelSize, y + static_cast<int>(row) * pixelSize,
						pixelSize, pixelSize, color);
				}
			}
		}
	}

	void DrawHeartIcon(int x, int y, Color color)
	{
		DrawPixelSprite(kHeartSprite, x, y, kHeartPixelSize, color);
	}

	// Textures can only be created after the window exists, so they are loaded in GameUI::Initialize.
	constexpr float kHitboxSize = 40.0f;
	constexpr float kUfoBoxSize = 50.0f;
	constexpr float kBossBoxSize = 150.0f; 

	constexpr const char* kPlayerShipTexturePath = "Assets/Sprites/player_ship.png";
	Texture2D g_playerShipTexture{};

	constexpr const char* kUfoTexturePath = "Assets/Sprites/ufo.png";
	Texture2D g_ufoShipTexture{};

	struct EnemySprite
	{
		const char* path{ nullptr };   // nullptr: no sprite, the fallback square is drawn
		float boxSize{};      // the sprite is scaled to this width
		Color fallbackColor;
		Texture2D texture;
	};

	std::array<EnemySprite, 4> g_enemySprites{ {

		{ "Assets/Sprites/enemy_normal.png",  kHitboxSize,  GREEN,  {} },
		{ "Assets/Sprites/enemy_armored.png", kHitboxSize,  BLUE,   {} },
		{ "Assets/Sprites/enemy_shooter.png", kHitboxSize,  ORANGE, {} },
		{ "Assets/Sprites/boss.png",          kBossBoxSize, RED,    {} },
	} };

	Texture2D LoadPixelTexture(const char* path)
	{
		const Texture2D texture = LoadTexture(path);
		if (texture.id != 0)
		{
			SetTextureFilter(texture, TEXTURE_FILTER_POINT);
		}
		return texture;
	}

	void DrawTextureInBox(const Texture2D& texture, float x, float y, float boxSize, bool sitOnBottom)
	{
		const float width = static_cast<float>(texture.width);
		const float height = static_cast<float>(texture.height);
		const float drawHeight = boxSize * height / width;
		const float offsetY = sitOnBottom ? boxSize - drawHeight : (boxSize - drawHeight) / 2.0f;

		DrawTexturePro(texture,
			Rectangle{ 0.0f, 0.0f, width, height },
			Rectangle{ x, y + offsetY, boxSize, drawHeight },
			Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
	}

	void DrawFallbackSquare(float x, float y, float boxSize, Color color)
	{
		DrawRectangle(static_cast<int>(x), static_cast<int>(y),
			static_cast<int>(boxSize), static_cast<int>(boxSize), color);
	}

	// position is the top-left of the hitbox.
	void DrawPlayerShip(float x, float y)
	{
		if (g_playerShipTexture.id == 0)
		{
			DrawFallbackSquare(x, y, kHitboxSize, kPrimaryGreen);
			return;
		}
		DrawTextureInBox(g_playerShipTexture, x, y, kHitboxSize, true);
	}

	void DrawUFO(float x, float y)
	{
		if (g_ufoShipTexture.id == 0)
		{
			DrawFallbackSquare(x, y, kUfoBoxSize, MAGENTA);
			return;
		}
		DrawTextureInBox(g_ufoShipTexture, x, y, kUfoBoxSize, false);
	}

	void DrawEnemyShip(const GameLogic::EnemyState& enemy)
	{
		const size_t index = static_cast<size_t>(enemy.type);

		const EnemySprite& sprite = g_enemySprites[index];
		if (sprite.texture.id == 0)
		{
			DrawFallbackSquare(enemy.position.x, enemy.position.y, sprite.boxSize, sprite.fallbackColor);
			return;
		}
		DrawTextureInBox(sprite.texture, enemy.position.x, enemy.position.y, sprite.boxSize, false);
	}

	void DrawHUD(uint32_t score, const char* levelLabel, int livesLeft)
	{
		const int screenWidth = GetScreenWidth();

		DrawText("SCORE", kHudMarginX, kHudTextY, kHintFontSize, kDimGreen);
		DrawText(TextFormat("%06u", static_cast<unsigned>(score)),
			kHudMarginX + MeasureText("SCORE", kHintFontSize) + 14, kHudTextY, kHintFontSize, kPrimaryGreen);

		DrawTextCentered(TextFormat("LEVEL %s", levelLabel), kHudTextY, kHintFontSize, kPrimaryGreen);

		constexpr int livesWidth = kMaxLives * kHeartWidth + (kMaxLives - 1) * kHeartGap;
		const int firstHeartX = screenWidth - kHudMarginX - livesWidth;
		const int heartY = kHudTextY + (kHintFontSize - kHeartHeight) / 2;

		const int labelWidth = MeasureText("LIVES", kHintFontSize);
		DrawText("LIVES", firstHeartX - labelWidth - 16, kHudTextY, kHintFontSize, kDimGreen);

		for (int i = 0; i < kMaxLives; ++i)
		{
			DrawHeartIcon(firstHeartX + i * (kHeartWidth + kHeartGap), heartY, i < livesLeft ? kPrimaryGreen : kDimGreen);
		}

		DrawRectangle(0, kHudDividerY, screenWidth, 2, kDimGreen);
		DrawRectangle(0, kGroundY, screenWidth, 4, kPrimaryGreen);
	}
}

GameUI::GameUI(std::unique_ptr<GameLogic::IGameService> gameService)
	: m_gameService{ std::move(gameService)}
{
	m_gameService->Subscribe(this);
}

GameUI::~GameUI()
{
	m_gameService->Unsubscribe(this);

	if (m_isInitialized && IsWindowReady())
	{
		UnloadTexture(g_playerShipTexture);
		g_playerShipTexture = Texture2D{};

		for (EnemySprite& sprite : g_enemySprites)
		{
			UnloadTexture(sprite.texture);
			sprite.texture = Texture2D{};
		}

		CloseWindow();
	}
}

void GameUI::Initialize()
{
	if (m_isInitialized) return;

	InitWindow(kScreenWidth, kScreenHeight, kWindowTitle);
	SetExitKey(KEY_NULL);
	SetTargetFPS(kTargetFps);

	g_playerShipTexture = LoadPixelTexture(kPlayerShipTexturePath);
	g_ufoShipTexture = LoadPixelTexture(kUfoTexturePath);

	for (EnemySprite& sprite : g_enemySprites)
	{
		if (sprite.path)
		{
			sprite.texture = LoadPixelTexture(sprite.path);
		}
	}

	m_starField.Initialize(kScreenWidth, kScreenHeight, kStarCount);
	m_currentScreen = ScreenType::MAIN_MENU;
	m_selectedMenuItem = 0;
	m_shouldExit = false;
	m_isInitialized = true;
}

void GameUI::Run()
{
	while (!m_shouldExit && !WindowShouldClose())
	{
		const float deltaTime = GetFrameTime();

		HandleInput();
		m_starField.Update(deltaTime);

		if (m_currentScreen == ScreenType::GAMEPLAY)
		{
			m_gameService->Update(deltaTime);
		}

		Render();
	}
}

void GameUI::OnGameEvent(const GameLogic::GameEvent& event)
{
	switch (event.type)
	{
	case GameLogic::GameEventType::LEVEL_COMPLETED:     m_currentScreen = ScreenType::VICTORY;       break;
	case GameLogic::GameEventType::GAME_OVER:           m_currentScreen = ScreenType::GAME_OVER;     break;
	default:                                                                                         break;
	}
}

void GameUI::ShowVerdict(ScreenType verdictScreen)
{
	m_selectedVerdictOption = 0;
	m_currentScreen = verdictScreen;
}

void GameUI::StartSelectedGame()
{
	m_gameService->StartLevel();
	m_currentScreen = ScreenType::GAMEPLAY;
}

// ---------------------------------------------------------------- Input

void GameUI::HandleInput()
{
	switch (m_currentScreen)
	{
	case ScreenType::MAIN_MENU:             HandleMainMenuInput(); break;
	case ScreenType::LEVEL_SELECTION:       HandleLevelSelectionInput(); break;
	case ScreenType::GAMEPLAY:              HandleGameplayInput(); break;
	case ScreenType::PAUSED:                HandlePauseInput(); break;
	case ScreenType::GAME_OVER:
	case ScreenType::VICTORY:               HandleVerdictInput(); break;
	}
}

void GameUI::HandleMainMenuInput()
{
	if (IsUpPressed())
	{
		m_selectedMenuItem = StepSelection(m_selectedMenuItem, -1, kMainMenuItems.size());
	}
	if (IsDownPressed())
	{
		m_selectedMenuItem = StepSelection(m_selectedMenuItem, +1, kMainMenuItems.size());
	}

	if (IsBackPressed())
	{
		m_shouldExit = true;
		return;
	}

	if (IsConfirmPressed())
	{
		switch (kMainMenuItems[static_cast<size_t>(m_selectedMenuItem)])
		{
		case MainMenuItem::PLAY:
		{
			m_currentScreen = ScreenType::LEVEL_SELECTION;
			m_gameService->GetLevel() = 0;
			break;
		}
		case MainMenuItem::EXIT: m_shouldExit = true; break;
		}
	}
}

void GameUI::HandleLevelSelectionInput()
{
	if (IsLeftPressed())
	{
		m_gameService->GetLevel() = StepSelection(m_gameService->GetLevel(), -1, kLevelLabels.size());
	}
	if (IsRightPressed())
	{
		m_gameService->GetLevel() = StepSelection(m_gameService->GetLevel(), 1, kLevelLabels.size());
	}

	if (IsBackPressed())
	{
		m_currentScreen = ScreenType::MAIN_MENU;
		return;
	}

	if (IsConfirmPressed())
	{
		StartSelectedGame();
	}
}

void GameUI::HandleVerdictInput()
{
	const auto items = GetVerdictItems(m_currentScreen == ScreenType::VICTORY, m_gameService->GetLevel());

	if (IsUpPressed()) m_selectedVerdictOption = StepSelection(m_selectedVerdictOption, -1, items.size());
	if (IsDownPressed()) m_selectedVerdictOption = StepSelection(m_selectedVerdictOption, 1, items.size());

	if (IsConfirmPressed())
	{
		switch (items[static_cast<size_t>(m_selectedVerdictOption)])
		{
		case VerdictMenuItem::NEXT_LEVEL:	++m_gameService->GetLevel(); StartSelectedGame();     break;
		case VerdictMenuItem::RETRY:        StartSelectedGame();								 break;
		case VerdictMenuItem::MAIN_MENU:    m_currentScreen = ScreenType::MAIN_MENU;			 break;
		}
	}
}

void GameUI::HandleGameplayInput()
{
	// Update() only runs on the GAMEPLAY screen, so switching screens freezes the game.
	if (IsBackPressed())
	{
		m_selectedPauseOption = 0;
		m_currentScreen = ScreenType::PAUSED;
		return;
	}

	if (IsLeftHold())
	{
		m_gameService->MovePlayer(GameLogic::Direction::LEFT);
	}
	if (IsRightHold())
	{
		m_gameService->MovePlayer(GameLogic::Direction::RIGHT);
	}

	if (IsSpacePressed())
	{
		m_gameService->Shoot();
	}
}

void GameUI::HandlePauseInput()
{
	if (IsUpPressed()) m_selectedPauseOption = StepSelection(m_selectedPauseOption, -1, kPauseMenuItems.size());
	if (IsDownPressed()) m_selectedPauseOption = StepSelection(m_selectedPauseOption, 1, kPauseMenuItems.size());

	// Esc toggles back into the game
	if (IsBackPressed())
	{
		m_currentScreen = ScreenType::GAMEPLAY;
		return;
	}

	if (IsConfirmPressed())
	{
		switch (kPauseMenuItems[m_selectedPauseOption])
		{
		case PauseMenuItem::RESUME:     m_currentScreen = ScreenType::GAMEPLAY;  break;
		case PauseMenuItem::RETRY:      StartSelectedGame();                     break;
		case PauseMenuItem::MAIN_MENU:
			m_gameService->Reset();
			m_currentScreen = ScreenType::MAIN_MENU;
			break;
		}
	}
}

// ---------------------------------------------------------------- rendering

void GameUI::Render()
{
	BeginDrawing();
	ClearBackground(kBackgroundColor);
	m_starField.Draw();

	switch (m_currentScreen)
	{
	case ScreenType::MAIN_MENU:         RenderMainMenu();         break;
	case ScreenType::LEVEL_SELECTION:   RenderLevelSelection();   break;
	case ScreenType::GAMEPLAY:          RenderGameplay();         break;
	case ScreenType::PAUSED:            RenderPause();            break;
	case ScreenType::GAME_OVER:         RenderGameOver();         break;
	case ScreenType::VICTORY:           RenderVictory();          break;
	}

	EndDrawing();
}

void GameUI::RenderMainMenu() const
{
	DrawScreenTitle("SPACE INVADERS", 150, kTitleFontSize);

	DrawMenuList(kMainMenuLabels, m_selectedMenuItem, 380);

	DrawTextCentered("UP/DOWN  SELECT     ENTER  CONFIRM     ESC  QUIT",
		GetScreenHeight() - 60, kHintFontSize, kDimGreen);
}

void GameUI::RenderLevelSelection() const
{
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();

	DrawScreenTitle("SELECT LEVEL", 150, kScreenTitleFontSize);

	// Three square boxes in a row; the selected one is filled green with a black label
	constexpr int boxSize = 160;
	constexpr int boxGap = 60;
	constexpr int boxY = 300;
	constexpr float borderThickness = 4.0f;
	constexpr int rowWidth = static_cast<int>(kLevelLabels.size()) * boxSize
		+ static_cast<int>((kLevelLabels.size() - 1)) * boxGap;
	const int firstBoxX = (screenWidth - rowWidth) / 2;

	for (size_t i = 0; i < kLevelLabels.size(); ++i)
	{
		const char* label = kLevelLabels[i];
		const int boxX = firstBoxX + static_cast<int>(i) * (boxSize + boxGap);
		const bool isSelected = (i == m_gameService->GetLevel());

		if (isSelected)
		{
			DrawRectangle(boxX, boxY, boxSize, boxSize, kPrimaryGreen);
		}
		else
		{
			const Rectangle box{ static_cast<float>(boxX), static_cast<float>(boxY),
								 static_cast<float>(boxSize), static_cast<float>(boxSize) };
			DrawRectangleLinesEx(box, borderThickness, kPrimaryGreen);
		}

		const int labelWidth = MeasureText(label, kLevelLabelFontSize);
		DrawText(label,
			boxX + (boxSize - labelWidth) / 2,
			boxY + (boxSize - kLevelLabelFontSize) / 2,
			kLevelLabelFontSize,
			isSelected ? kBackgroundColor : kPrimaryGreen);
	}

	DrawTextCentered("LEFT/RIGHT  SELECT     ENTER  START     ESC  BACK",
		screenHeight - 60, kHintFontSize, kDimGreen);
}

void GameUI::RenderGameplay()
{
	const int livesLeft = m_gameService->GetPlayerState().livesCount;
	DrawHUD(m_gameService->GetScore(), kLevelLabels[m_gameService->GetLevel()], livesLeft);

	const Vector2 playerPosition{ m_gameService->GetPlayerState().position.x, m_gameService->GetPlayerState().position.y };
	DrawPlayerShip(playerPosition.x, playerPosition.y);

	if (const auto& ufo = m_gameService->GetUFOState(); ufo.has_value())
	{
		DrawUFO(ufo.value().position.x, ufo.value().position.y);
	}

	for (const auto& enemy : m_gameService->GetEnemiesState())
	{
		DrawEnemyShip(enemy);
	}

	for (const auto& projectile : m_gameService->GetPlayerProjectilesState())
	{
		DrawRectangle(static_cast<int>(projectile.position.x), static_cast<int>(projectile.position.y), 2, 10, RED);
	}

	for (const auto& projectile : m_gameService->GetEnemyProjectilesState())
	{
		DrawRectangle(static_cast<int>(projectile.position.x), static_cast<int>(projectile.position.y), 2, 10, GREEN);
	}
}

void GameUI::RenderPause()
{
	// Frozen game stays visible behind a dimmed overlay
	RenderGameplay();
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 170 });

	DrawScreenTitle("PAUSED", 160, kTitleFontSize);
	DrawMenuList(kPauseMenuLabels, m_selectedPauseOption, 360);

	DrawTextCentered("UP/DOWN  SELECT     ENTER  CONFIRM     ESC  RESUME",
		GetScreenHeight() - 60, kHintFontSize, kDimGreen);
}

void GameUI::RenderGameOver() const
{
	const char* details = TextFormat("LEVEL %s", kLevelLabels[m_gameService->GetLevel()]);
	const auto labels = GetVerdictLabels(GetVerdictItems(false, m_gameService->GetLevel()));

	DrawVerdictScreen("GAME OVER", "THE INVADERS HAVE LANDED", details, m_gameService->GetScore(),
		labels, m_selectedVerdictOption);
}

void GameUI::RenderVictory() const
{
	const char* details = TextFormat("LEVEL %s", kLevelLabels[m_gameService->GetLevel()]);
	const auto labels = GetVerdictLabels(GetVerdictItems(true, m_gameService->GetLevel()));

	DrawVerdictScreen("VICTORY", "EARTH IS SAFE", details, m_gameService->GetScore(),
		labels, m_selectedVerdictOption);

}
