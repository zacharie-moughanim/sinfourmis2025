#pragma once

#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

/**
 * @brief Represents a team in the game
 *
 */
class Team {
  public:
    Team() = default;
    Team(int id, const std::string_view &name, const std::string_view &color) : id(id), name(name) {}

	void add_food(unsigned int amount) {
		std::cout << "Team " << id << " gain " << amount << " food " << score << std::endl;
		food += amount;
		score += amount;
	}

	bool try_remove_food(unsigned int amount) {
		if (food < amount) {
			return false;
		}
		food -= amount;
		return true;
	}

    std::string get_name() const {
        return name;
    }
    unsigned int get_id() const {
        return id;
    }

	unsigned int get_food() const {
		return food;
	}

	unsigned int get_score() const {
		return score;
	}

    // for json serialization / deserialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Team, id, name, color)

  private:
    unsigned int id = -1;
    std::string name = "";
	std::string color = "#000000";
	unsigned int food = 0;
	unsigned int score = 0;
};
