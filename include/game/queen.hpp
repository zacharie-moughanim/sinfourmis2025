#pragma once

#include "game/constants.hpp"
#include <vector>

class Node;

class Queen {
  public:
    Queen(unsigned int team_id, Node *node) : team_id(team_id), current_node(node) {}

    enum class Stat : unsigned int { LIFE, WATER, FOOD, ATTACK, STORED_ANT };

    void game_turn();
    bool can_perform_action() const;
    bool upgrade(Stat type);
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

  private:
    unsigned int team_id = 0;
    Node *current_node = nullptr;
    unsigned int waiting_upgrade = 0;
    unsigned int victory_points = 0;

    unsigned int stats[5] = {DEFAULT_MAX_LIFE, DEFAULT_MAX_WATER, DEFAULT_MAX_FOOD, DEFAULT_ATTACK,
                             DEFAULT_MAX_STORED_ANT};

    const static unsigned int upgrade_costs[5];

    std::vector<char[256]> ants_memory;
};