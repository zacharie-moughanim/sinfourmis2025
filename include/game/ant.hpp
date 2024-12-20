#pragma once

#include "map/node.hpp"
#include "sinfourmis.h"
#include <functional>

class Ant {
  public:
    Ant(unsigned int team_id, Node *node, uint8_t max_life, uint32_t max_water);
    Ant(const Ant &&ant);
    Ant &operator=(const Ant &&ant);
	~Ant();

    Node *get_current_node() const;
    void set_current_node(Node *node);

    void apply_damages(uint8_t damages);
	void water_action();

    bool alive();

    fourmi_etat as_fourmi_etat() const {
		return etat;
	}

    unsigned int get_team_id() const {
        return team_id;
    }

	void set_result(int32_t result) {
		this->etat.result = result;
	}

  private:
    unsigned int team_id = 0;
    Node *current_Node = nullptr;
	fourmi_etat etat;
	uint8_t max_life = 0;
	uint32_t max_water = 0;
};