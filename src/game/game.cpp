#include "game/game.hpp"

void Game::set_map(Map &&map) {
    this->map = std::move(map);
}

void Game::add_interface(unsigned int team, Interface *interface) {
    if (interfaces.size() + 1 > map.get_team_count()) {
        throw std::runtime_error("Too many interfaces");
    }

    interfaces.emplace(map.get_team(team).get_id(), interface);
}

Game &Game::getInstance() {
    static Game instance;

    return instance;
}

Game::~Game() {
	for (auto [_, interface]: interfaces) {
		delete interface;
	}
}

void Game::fourmi_action(Ant *ant) {
    ant->consume_water();
    if (!ant->alive()) {
        return;
    }
    if (ant->get_action_state() == AntActionState::MOVING) {
        ant->displace();
        return;
    }
    if (ant->get_action_state() == AntActionState::DIGGING) {
        ant->dig();
		// we can still perform other actions while digging
    }
    auto &etat = ant->as_fourmi_etat();
    auto room = ant->get_current_node()->as_salle();
    auto ant_result = interfaces[ant->get_team_id()]->fourmi_activation(&etat, &room);
	if (ant_result.depose_pheromone) {
        ant->get_current_node()->set_pheromone(ant_result.pheromone);
    }
    switch (ant_result.action) {
        case FOURMI_PASSE:
            ant->set_result(0);
            break;
        case fourmi_action::DEPLACEMENT:
            if (ant->get_current_node()->degree() < (uint32_t)ant_result.arg) {
                ant->set_result(-1);
            } else if (ant->get_action_state() != AntActionState::NONE) {
                ant->set_result(-1);
            } else if (!ant->get_current_node()->get_edge(ant_result.arg)->can_be_crossed()) {
                ant->set_result(-2);
            } else {
                auto edge = ant->get_current_node()->get_edge(ant_result.arg);
                auto curr_node = ant->get_current_node();
                ant->move_along(edge);
                ant->set_result(edge->get_other_node(curr_node)->get_id_to(curr_node));
            }
            break;
        case fourmi_action::RAMASSE_NOURRITURE:
            if (ant->get_current_node()->get_type() != salle_type::NOURRITURE ||
                ant->get_action_state() != AntActionState::NONE) {
                ant->set_result(-1);
            } else {
                ant->set_result(ant->gather_food());
            }
            break;
        case fourmi_action::ATTAQUE:
            {
                if (ant->get_action_state() != AntActionState::NONE) {
                    ant->set_result(-1);
                    break;
                }
                if (ant_result.arg >> 8 != 0) {
                    std::cout << "Warning: the attack is too big, it will be truncated"
                              << std::endl;
                    std::cout
                        << "Perhaps it is a bug of your interface but else, nice try cheater ;)"
                        << std::endl;
                }
                unsigned int team_id_attacked = ant_result.arg & 0xFF;
                if (ant->get_team_id() == team_id_attacked) {
                    std::cout << "Warning: self-attack on team" << ant->get_team_id() << std::endl;
                }
                int32_t result = 0;
                for (auto &node_ant : ant->get_current_node()->get_ants()) {
                    if (node_ant->get_team_id() == team_id_attacked) {
                        result++;
                        node_ant->apply_damages((uint8_t)node_ant->get_attack());
                    }
                }
                ant->set_result(result);
            }
            break;
        case fourmi_action::COMMENCE_CONSTRUCTION:
            if (ant->get_action_state() != AntActionState::NONE) {
                ant->set_result(-1);
                break;
            }
            if (ant->get_current_node()->degree() < (uint32_t)ant_result.arg) {
                ant->set_result(-1);
                break;
            }
            ant->begin_digging(ant->get_current_node()->get_edge(ant_result.arg));
            ant->set_result(0);
            break;
        case fourmi_action::TERMINE_CONSTRUCTION:
            if (ant->get_action_state() != AntActionState::DIGGING) {
                ant->set_result(-1);
                break;
            }
            ant->stop_digging();
            ant->set_result(0);
            break;
        case fourmi_action::ATTAQUE_TUNNEL:
            if (ant->get_action_state() != AntActionState::NONE) {
                ant->set_result(-1);
                break;
            }
            if (ant->get_current_node()->degree() < (uint32_t)ant_result.arg) {
                ant->set_result(-1);
                break;
            }
            ant->set_result(ant->get_current_node()->get_edge(ant_result.arg)->attack(ant->get_attack()));
            break;
        default:
            throw std::runtime_error("Invalid action");
            break;
    }
}

void Game::queen_action(Queen *queen, std::vector<std::unique_ptr<Ant>> &ants) {
    queen->game_turn();
    if (!queen->can_perform_action()) {
        return;
    }
    auto &memories = queen->get_states();
    auto etat = queen->as_reine_etat();
    auto salle = queen->get_current_node()->as_salle();
    auto result = interfaces[queen->get_team_id()]->reine_activation(memories.data(),
                                                                    memories.size(), &etat, &salle);
    switch (result.action) {
        case reine_action::REINE_PASSE:
            break;
        case reine_action::AMELIORE_DEGATS:
            queen->set_result(!queen->upgrade(Queen::Stat::ATTACK));
            break;
        case reine_action::AMELIORE_VIE:
            queen->set_result(!queen->upgrade(Queen::Stat::LIFE));
            break;
        case reine_action::AMELIORE_EAU:
            queen->set_result(!queen->upgrade(Queen::Stat::WATER));
            break;
        case reine_action::AMELIORE_RAMASSAGE:
            queen->set_result(!queen->upgrade(Queen::Stat::FOOD));
            break;
        case reine_action::AMELIORE_VITESSE_AMELIORATION:
            queen->set_result(!queen->upgrade_queen(Queen::QueenStat::UPGRADE_DURATION));
            break;
        case reine_action::AMELIORE_STOCKAGE:
            queen->set_result(!queen->upgrade_queen(Queen::QueenStat::STORED_ANTS));
            break;
        case reine_action::AMELIORE_ENVOI:
            queen->set_result(!queen->upgrade_queen(Queen::QueenStat::ANTS_SENDING));
            break;
        case reine_action::AMELIORE_PRODUCTION:
            queen->set_result(!queen->upgrade_queen(Queen::QueenStat::PRODUCED_ANTS));
            break;
        case reine_action::CREER_FOURMI:
            {
                unsigned int created = 0;
                for (int i = 0; i < result.arg && queen->create_ant(); i++) {
                    created++;
                }
                queen->set_result(created);
            }
            break;
        case reine_action::ENVOYER_FOURMI:
            {
                unsigned int sent = 0;
                for (int i = 0; i < std::min(result.arg, (int32_t)queen->get_queen_stat(
                                                             Queen::QueenStat::ANTS_SENDING));
                     i++) {
                    auto ant_state = queen->pop_ant();
                    if (ant_state.has_value()) {
                        sent++;
                        ants.emplace_back(std::make_unique<Ant>(queen, std::move(ant_state.value())));
                    } else {
                        break;
                    }
                }
                queen->set_result(sent);
            }
            break;
        case reine_action::RECUPERER_FOURMI:
            {
                uint32_t gathered = 0;
                Node *node = queen->get_current_node();
                auto node_ants = node->get_ants();
				if (result.arg < 0) {
					queen->set_result(-1);
					break;
				}
				auto max_gathered = std::min((uint32_t)result.arg, queen->get_queen_stat(Queen::QueenStat::STORED_ANTS));
                for (auto it = node_ants.begin();
                     it != node_ants.end() && gathered < max_gathered;
                     it++) {
                    if ((*it)->get_team_id() != queen->get_team_id() || !(*it)->alive()) {
                        continue;
                    }
                    if (queen->push_ant((*it)->as_fourmi_etat())) {
                        gathered++;
                        (*it)->kill(); // Kill the ant, it will be removed at the end of the turn
                    }
                }
                queen->set_result(gathered);
            }
            break;
        default:
            throw std::runtime_error("Invalid action");
            break;
    }
}

void Game::run(unsigned int duration, unsigned int seed, bool flush, std::filesystem::path &&path) {
    if (interfaces.size() != map.get_team_count()) {
		std::cerr << interfaces.size() << " " << map.get_team_count() << std::endl;
        throw std::runtime_error("Not enough interfaces");
    }

    std::cout << "Running game with seed " << seed << std::endl;

	Animation animation(&map, path);

    gen.seed(seed);

    std::vector<std::unique_ptr<Queen>> queens;
    for (auto &team : map.get_teams()) {
        queens.emplace_back(std::make_unique<Queen>(&team, map.get_starting_node(team.get_id())));
    }
    std::vector<std::unique_ptr<Ant>> ants;

    while (animation.game_turn() < duration) {
		animation.start_frame();
        map.regen_food();

        // === Ants turn ===
		if (!ants.empty()) {
			std::erase_if(ants, [](auto &ant) { return !ant->alive(); });
			std::ranges::shuffle(ants, gen);
		}
        for (auto &ant : ants) {
            fourmi_action(ant.get());
        }

        // === Queen turn ===
		std::ranges::shuffle(queens, gen);
        for (auto &queen : queens) {
            queen_action(queen.get(), ants);
        }
		animation.end_frame();
		if (flush) {
			animation.flush();
		}
    }
	for (auto &team: map.get_teams()) {
		std::cout << "Team " << team.get_id() << " score: " << team.get_score() << std::endl;
	}
	animation.flush();
}
