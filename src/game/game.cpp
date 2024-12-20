#include "game/game.hpp"

void Game::setMap(const Map &map) {
    this->map = map;
}

void Game::addInterface(Interface *interface) {
    if (interfaces.size() + 1 > map.get_team_count()) {
        throw std::runtime_error("Too many interfaces");
    }

    interfaces.emplace(map.get_team(current_team).get_id(), interface);
}

Game &Game::getInstance() {
    static Game instance;

    return instance;
}

void Game::fourmi_action(Ant &ant) {
	ant.water_action();
	if (!ant.alive()) {
		return;
	}
	// TODO: ant displacement
	auto etat = ant.as_fourmi_etat();
	auto room = ant.get_current_node()->as_salle();
	auto result = interfaces[ant.get_team_id()]->fourmi_activation(&etat, &room);
	switch (result.action) {
		case FOURMI_PASSE:
			ant.set_result(0);
			break;
		case fourmi_action::DEPLACEMENT:
			if (ant.get_current_node()->degree() < (uint32_t)result.arg) {
				ant.set_result(-1);
			} else {
				// TODO
			}
			break;
		case fourmi_action::DEPOSE_PHEROMONE:
			if (result.arg >> 8 != 0) {
				std::cout << "Warning: the pheromone is too big, it will be truncated" << std::endl;
				std::cout << "Perhaps it is a bug of your interface but else nice try cheater ;)" << std::endl;
			}
			ant.get_current_node()->set_pheromone(result.arg);
			break;
		case fourmi_action::RAMASSE_NOURRITURE:
		    if (ant.get_current_node()->get_type() != salle_type::NOURRITURE) {
				ant.set_result(-1);
			} else {
				// TODO
			}
			break;
		case fourmi_action::ATTAQUE:
			// TODO
			break;
		case fourmi_action::COMMENCE_CONSTRUCTION:
			// TODO
			break;
		case fourmi_action::TERMINE_CONSTRUCTION:
			// TODO
			break;
		default:
			throw std::runtime_error("Invalid action");
			break;
	}
}

void Game::run() {
    if (interfaces.size() != map.get_team_count()) {
        throw std::runtime_error("Not enough interfaces");
    }

    bool game_ended = false;
    std::vector<Ant> ants;

    while (!game_ended) {
        // === Ants turn ===
        ants.erase(
            std::remove_if(ants.begin(), ants.end(), [](auto &ant) { return !ant.alive(); }));
        for (auto &ant : ants) {
			fourmi_action(ant);
        }

        // === Queen turn ===
		// TODO
    }
}