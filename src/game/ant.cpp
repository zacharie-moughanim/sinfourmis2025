#include "game/ant.hpp"

Ant::Ant(unsigned int team_id, Node *node, uint8_t max_life, uint32_t max_water)
    : team_id(team_id), current_Node(node), max_life(max_life),
      max_water(max_water) {
		etat.result = -1;
		etat.vie = max_life;
		etat.eau = max_water;
		etat.nouriture = 0;
		if (current_Node != nullptr) {
			node->add_ant(this);
		}
	  }

Ant::Ant(const Ant &&ant)
    : team_id(ant.team_id), current_Node(ant.current_Node)
	 {
		etat = ant.etat;
	 }

Ant &Ant::operator=(const Ant &&ant) {
	team_id = ant.team_id;
    current_Node = ant.current_Node;
    etat = ant.etat;
    return *this;
}

Ant::~Ant() {
	if (current_Node != nullptr) {
		current_Node->remove_ant(this);
	}
}

Node *Ant::get_current_node() const {
    return current_Node;
}

void Ant::set_current_node(Node *node) {
    current_Node = node;
}

void Ant::apply_damages(uint8_t damages) {
    if (damages > etat.vie)  {
        etat.vie = 0;
    } else {
        etat.vie -= damages;
    }
}

void Ant::water_action() {
    if (current_Node != nullptr && current_Node->get_type() == salle_type::EAU) {
        etat.eau = max_water;
    } else if (etat.eau > 0) {
		etat.eau--;
	}
	if (etat.eau == 0) {
		etat.vie = 0;
	}
}

bool Ant::alive() {
    return etat.vie != 0;
}