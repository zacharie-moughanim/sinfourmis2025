#include "game/queen.hpp"

const std::array<unsigned int, 5> Queen::upgrade_costs = {QUEEN_UPGRADE_LIFE_COST, QUEEN_UPGRADE_WATER_COST,
                                              QUEEN_UPGRADE_FOOD_COST, QUEEN_UPGRADE_ATTACK_COST,
                                              QUEEN_UPGRADE_NB_ANT_COST};

const std::array<unsigned int, 4> Queen::queen_upgrade_costs = {QUEEN_UPGRADE_NB_ANT_COST, QUEEN_UPGRADE_PRODUCED_ANTS_COST,
													QUEEN_REDUCE_UPGRADE_TIME_COST, QUEEN_UPGRADE_NB_ANT_SENDING_COST};

void Queen::game_turn() {
    if (waiting_upgrade > 0) {
        waiting_upgrade--;
    }
}

bool Queen::can_perform_action() const {
    return waiting_upgrade == 0;
}

bool Queen::upgrade(Stat type) {
    if (waiting_upgrade > 0) {
        return false;
    }
    if (food < upgrade_costs[(unsigned int)type]) {
        return false;
    }
    stats[(unsigned int)type]++;
    food -= upgrade_costs[(unsigned int)type];
    waiting_upgrade = get_queen_stat(QueenStat::UPGRADE_DURATION);
    return true;
}

bool Queen::upgrade_queen(QueenStat type) {
    if (waiting_upgrade > 0) {
        return false;
    }
    if (food < queen_upgrade_costs[(unsigned int)type]) {
		return false;
	}
	switch (type) {
		case QueenStat::UPGRADE_DURATION: {
			unsigned int &upgrade_duration = queen_stats[(unsigned int)type];
			if (upgrade_duration > 1) {
				upgrade_duration--;
			}
		}
			break;
		default:
			queen_stats[(unsigned int)type]++;
			break;
	}
	waiting_upgrade = get_queen_stat(QueenStat::UPGRADE_DURATION);
    return true;
}

unsigned int Queen::get_stat(Stat type) const {
    return stats[(unsigned int)type];
}

unsigned int Queen::get_queen_stat(QueenStat type) const {
    return queen_stats[(unsigned int)type];
}
fourmi_etat Queen::default_fourmi_etat() const {
    fourmi_etat etat;
	etat.result = -1;

	etat.vie = std::max((unsigned int)UINT8_MAX, get_stat(Stat::LIFE));
	
	etat.eau = get_stat(Stat::WATER);
	etat.nouriture = 0;
	memset(etat.memoire, 0, sizeof(etat.memoire));
	return etat;
}
bool Queen::create_ant() {
    if (ants_memory.size() >= get_queen_stat(QueenStat::STORED_ANTS)) {
		return false;
	}
	ants_memory.push_back(default_fourmi_etat());
	return true;
}

bool Queen::push_ant(fourmi_etat ant) {
    if (ants_memory.size() >= get_queen_stat(QueenStat::STORED_ANTS)) {
		return false;
	}
	this->food += ant.nouriture;
	ants_memory.push_back(ant);
	return true;
}

std::optional<fourmi_etat> Queen::pop_ant() {
    if (ants_memory.empty()) {
		return std::nullopt;
	}
	fourmi_etat ant = ants_memory.back();
	ants_memory.pop_back();
    ant.nouriture = 0;
    ant.eau = get_stat(Stat::WATER);
    ant.vie = std::max((unsigned int)UINT8_MAX, get_stat(Stat::LIFE));
    return ant;
}
