#pragma once

#include "game/constants.hpp"
#include "sinfourmis.h"
#include <vector>

class Node;

class Queen {
  public:
    Queen(unsigned int team_id, Node *node) : team_id(team_id), current_node(node) {}

    enum class Stat : unsigned int { LIFE, WATER, FOOD, ATTACK, STORED_ANT };
	enum class QueenStat: unsigned int {STORED_ANTS, PRODUCED_ANTS, UPGRADE_DURATION, ANTS_SENDING};

    void game_turn();
    bool can_perform_action() const;
    bool upgrade(Stat type);
	bool upgrade_queen(QueenStat type);

    unsigned int get_stat(Stat type) const;

    unsigned int get_victory_points() const {
        return victory_points;
    }

    unsigned int get_team_id() const {
        return team_id;
    }

    Node *get_current_node() const {
        return current_node;
    }

	std::vector<fourmi_etat> get_states() const {
		return ants_memory;
	}

	void set_result(int32_t result) {
		this->result = result;
	}

  private:
    unsigned int team_id = 0;
    Node *current_node = nullptr;
    unsigned int waiting_upgrade = 0;
    unsigned int victory_points = 0;
	int32_t result = 0;

    unsigned int stats[5] = {DEFAULT_MAX_LIFE, DEFAULT_MAX_WATER, DEFAULT_MAX_FOOD, DEFAULT_ATTACK,
                             QUEEN_DEFAULT_MAX_STORED_ANT};

    const static unsigned int upgrade_costs[5];
	const static unsigned int queen_upgrade_costs[4];

	unsigned int upgrade_duration = QUEEN_DEFAULT_UPGRADE_DURATION;

    std::vector<fourmi_etat> ants_memory;
	size_t max_ants = QUEEN_DEFAULT_MAX_STORED_ANT;
	unsigned int sent_ants = QUEEN_DEFAULT_SENT_ANTS;
	unsigned int produced_ants = QUEEN_DEFAULT_PRODUCED_ANTS;
};