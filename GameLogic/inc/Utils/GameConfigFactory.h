#pragma once

#include "Structs/GameConfig.h"

#include "Utils/EnemyUtils.h"
	
#include "nlohmann/json.hpp"

#include <fstream>
#include <format>
#include <filesystem>
#include <iostream>

using json = nlohmann::json;

namespace GameLogic
{
	namespace
	{
		constexpr std::string_view LEVELS_DIR = "Assets/Levels";
		constexpr std::array<std::string_view, 3> LEVEL_FILES
		{
			"level1.json", "level2.json", "level3.json"
		};

		std::array<LevelConfig, 3> LoadLevels()
		{
			std::array<LevelConfig, 3> levels{};

			for (size_t i = 0; i < levels.size(); ++i)
			{
				const std::filesystem::path path = std::filesystem::path{ LEVELS_DIR } / LEVEL_FILES[i];
				std::ifstream levelFile(path);

				std::cout << "hereee\n";
				if (!levelFile.is_open())
				{
					throw std::runtime_error(std::format("Could not open {} file", LEVEL_FILES[i]));
				}

				std::cout << "hereee\n";
				json data;
				try
				{
					data = json::parse(levelFile);
				}
				catch(json::parse_error& e)
				{
					throw std::runtime_error(std::format("Parse error: {}", e.what()));
				}

				levels[i].hasBoss = data["hasBoss"];

				for (const auto& enemy : data["enemies"])
				{
					EnemyConfig config;

					config.position.x = enemy["position"]["x"];
					config.position.y = enemy["position"]["y"];

					const std::string type = enemy["type"];

					if (type == "NORMAL")		config.type = EnemyType::NORMAL;
					else if (type == "ARMORED")	config.type = EnemyType::ARMORED;
					else if (type == "SHOOTER")	config.type = EnemyType::SHOOTER;
					else if (type == "BOSS")	config.type = EnemyType::BOSS;
					else						config.type = EnemyType::NORMAL;

					config.health = Utils::GetEnemyHealth(config.type);
					levels[i].enemies.emplace_back(config);
				}

				levelFile.close();
			}

			return levels;
		}
	}

	GameConfig CreateDefaultConfig();
}