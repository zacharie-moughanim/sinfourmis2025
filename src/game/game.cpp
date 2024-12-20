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
    if (ant.get_action_state() == AntActionState::MOVING) {
        ant.displace();
        return;
    }
    if (ant.get_action_state() == AntActionState::DIGGING) {
        ant.dig();
        return;
    }
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
            } else if (ant.get_action_state() != AntActionState::NONE) {
                ant.set_result(-1);
            } else if (ant.get_current_node()->get_edge(result.arg)->can_be_crossed()) {
                ant.set_result(-2);
            } else {
                auto edge = ant.get_current_node()->get_edge(result.arg);
                auto curr_node = ant.get_current_node();
                ant.move_along(edge);
                ant.set_result(edge->get_other_node(curr_node)->get_id_to(curr_node));
            }
            break;
        case fourmi_action::DEPOSE_PHEROMONE:
            if (result.arg >> 8 != 0) {
                std::cout << "Warning: the pheromone is too big, it will be truncated" << std::endl;
                std::cout << "Perhaps it is a bug of your interface but else, nice try cheater ;)"
                          << std::endl;
            }
            ant.get_current_node()->set_pheromone(result.arg & 0xFF);
            break;
        case fourmi_action::RAMASSE_NOURRITURE:
            if (ant.get_current_node()->get_type() != salle_type::NOURRITURE ||
                ant.get_action_state() != AntActionState::NONE) {
                ant.set_result(-1);
            } else {
                ant.set_result(ant.gather_food());
            }
            break;
        case fourmi_action::ATTAQUE:
            {
                if (ant.get_action_state() != AntActionState::NONE) {
                    ant.set_result(-1);
                    break;
                }
                if (result.arg >> 8 != 0) {
                    std::cout << "Warning: the attack is too big, it will be truncated"
                              << std::endl;
                    std::cout
                        << "Perhaps it is a bug of your interface but else, nice try cheater ;)"
                        << std::endl;
                }
                unsigned int team_id_attacked = result.arg & 0xFF;
                if (ant.get_team_id() == team_id_attacked) {
                    std::cout << "Warning: self-attack on team" << ant.get_team_id() << std::endl;
                }
                int32_t result = 0;
                for (auto &ant : ant.get_current_node()->get_ants()) {
                    if (ant->get_team_id() == team_id_attacked) {
                        result++;
                        ant->apply_damages(ant->get_attack());
                    }
                }
                ant.set_result(result);
            }
            break;
        case fourmi_action::COMMENCE_CONSTRUCTION:
            if (ant.get_action_state() != AntActionState::NONE) {
                ant.set_result(-1);
                break;
            }
            if (ant.get_current_node()->degree() < (uint32_t)result.arg) {
                ant.set_result(-1);
                break;
            }
            ant.begin_digging(ant.get_current_node()->get_edge(result.arg));
            ant.set_result(0);
            break;
        case fourmi_action::TERMINE_CONSTRUCTION:
            if (ant.get_action_state() != AntActionState::DIGGING) {
                ant.set_result(-1);
                break;
            }
            ant.stop_digging();
            ant.set_result(0);
            break;
        case fourmi_action::ATTAQUE_TUNNEL:
            if (ant.get_action_state() != AntActionState::NONE) {
                ant.set_result(-1);
                break;
            }
            if (ant.get_current_node()->degree() < (uint32_t)result.arg) {
                ant.set_result(-1);
                break;
            }
            ant.set_result(ant.get_current_node()->get_edge(result.arg)->attack(ant.get_attack()));
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
        map.regen_food();

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