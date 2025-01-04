#pragma once

#include "map/map.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>

using json = nlohmann::json;

class Animation {
public:
	Animation(const Map *map, std::filesystem::path path) : map(map), path(path) {}

	void start_frame();
	void end_frame();

	unsigned int game_turn() const { return turn; }

private:
	unsigned int turn = 0;

	const Map *map;
	std::filesystem::path path;
	std::filesystem::path current_path;

	std::ofstream file;
};