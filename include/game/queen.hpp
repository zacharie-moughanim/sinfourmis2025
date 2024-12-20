#pragma once

#include "game/constants.hpp"
#include <vector>

class Node;

class Queen {
  public:
    Queen(unsigned int team_id, Node *node) : team_id(team_id), current_node(node) {}

    unsigned int get_team_id() const {
        return team_id;
    }

    Node *get_current_node() const {
        return current_node;
    }

    unsigned int get_max_life() const {
        return max_life;
    }

    unsigned int get_max_water() const {
        return max_water;
    }

    unsigned int get_max_food() const {
        return max_food;
    }

    unsigned int get_attack() const {
        return attack;
    }

    unsigned int get_max_stored_ant() const {
        return max_stored_ant;
    }

  private:
    unsigned int team_id = 0;
    Node *current_node = nullptr;

    unsigned int max_life = DEFAULT_MAX_LIFE;
    unsigned int max_water = DEFAULT_MAX_WATER;
    unsigned int max_food = DEFAULT_MAX_FOOD;
    unsigned int attack = DEFAULT_ATTACK;
    unsigned int max_stored_ant = DEFAULT_MAX_STORED_ANT;
};