#include "debugger/debugger.hpp"

std::ostream &operator<<(std::ostream &os, const Node &node) {
    os << "Node " << node.get_id() << " (" << node.get_x() << ", " << node.get_y() << ")\n";
    os << "Ants:\n";
    auto groups = node_groups(node);
    for (const auto &[team_id, count] : groups) {
        os << " - Team " << team_id << ": " << count << "\n";
    }
    os << "Type: " << node.get_type() << "\n";
    if (node.get_type() == salle_type::NOURRITURE) {
        os << "Food: " << node.get_food() << "\n";
        os << "Remaining: " << node.get_remaining() << "\n";
    }
    if (node.get_type() == salle_type::REINE) {
        os << "Queen: " << node.get_team() << "\n";
    }
    os << "Pheromones: " << node.get_pheromones() << "\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Edge &edge) {
    os << "Edge " << edge.get_node1()->get_id() << " -- " << edge.get_node2()->get_id() << "\n";
    os << "Ants:\n";
    auto groups = edge_groups(edge);
    for (const auto &[key, count] : groups) {
        const auto &[start, team, progress] = key;
        os << "- Group of " << team << ": ";
        os << "\tAdvancement:" << count << " (" << progress << "/" << edge.get_length() << ")\n";
        os << "Origin:" << start << "\n";
    }
    return os;
}

void Debugger::debug(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                     const std::vector<std::unique_ptr<Queen>> &queens) {
    if (!m_debug) {
        return;
    }
    if (skip > 0) {
        skip--;
        return;
    }
    std::string command;
	while (skip == 0 && !m_exit) {
		std::cout << "> ";
		std::getline(std::cin, command);
		auto cmd = parse_command(command);
		using enum DebugCommand::Type;
		switch (cmd.type) {
			case EXIT:
				m_exit = true;
				break;
			case HELP:
				display_help();
				break;
			case STEP:
				if (cmd.arg.has_value()) {
					skip = std::get<int>(cmd.arg.value()) + 1;
				} else {
					skip = 1;
				}
				break;
			case DISPLAY:
				{
					auto args = std::get<std::tuple<std::string, int, int>>(cmd.arg.value());
					display_element(map, ants, queens, std::get<0>(args), std::get<1>(args),
									std::get<2>(args));
				}
				break;
			case LIST:
				list_elements(map, ants, queens, std::get<0>(std::get<1>(cmd.arg.value())));
				break;
			case LISTIN:
				{
					auto args = std::get<std::tuple<std::string, int, int>>(cmd.arg.value());
					list_ants_in(map, ants, std::get<0>(args), std::get<1>(args), std::get<2>(args));
				}
				break;
			case INVALID:
				break;
		}
	}
	skip--;
}

Debugger::DebugCommand Debugger::parse_command(const std::string &command) const {
    using enum DebugCommand::Type;
    if (command == "exit") {
        return DebugCommand(EXIT);
    } else if (command == "help") {
        return DebugCommand(HELP);
    } else if (command == "step") {
        return DebugCommand(STEP);
    } else {
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;
        if (cmd == "step") {
            int steps;
            iss >> steps;
            if (iss.fail()) {
                print_error(command, iss.tellg(), "Invalid number of steps");
                return DebugCommand();
            }
            if (iss.tellg() != -1) {
                print_error(command, iss.tellg(), "Invalid command");
                return DebugCommand();
            }
            return DebugCommand(steps);
        } else if (cmd == "display" || cmd == "listin") {
            auto debug_command = parse_id_command(iss, command);
			if (debug_command.type != INVALID && cmd == "listin") {
				debug_command.type = LISTIN;
				auto type = std::get<0>(std::get<1>((debug_command.arg.value())));
				if (type != "node" && type != "edge") {
					print_error(command, 0, "Invalid listin type, expected node or edge");
					return DebugCommand();
				}
			}
			return debug_command;
        } else if (cmd == "list") {
			std::string type;
			auto cursor = iss.tellg();
			iss >> type;
			if (iss.fail()) {
                print_error(command, cursor,
                            "Invalid list type, expected nodes, edges, ants or queens");
                return DebugCommand();
			}
			if (iss.tellg() != -1) {
				print_error(command, 0, "Invalid command");
				return DebugCommand();
			}
			if (type != "nodes" && type != "edges" && type != "ants" && type != "queens") {
                print_error(command, cursor,
                            "Invalid list type, expected nodes, edges, ants or queens");
                return DebugCommand();
			}
			return DebugCommand(type);		
		} else {
            print_error(command, 0, "Invalid command");
        }
    }
    return DebugCommand();
}

const Debugger::DebugCommand Debugger::parse_id_command(std::istringstream &iss,
                                                         const std::string &command) const {
    std::string type;
    iss >> type;
    if (iss.fail()) {
        print_error(command, iss.tellg(),
                    "Invalid display type, expected node, edge, ant or queen");
        return DebugCommand();
    }
    if (type == "global") {
        if (iss.tellg() != -1) {
            print_error(command, iss.tellg(), "Invalid command");
            return DebugCommand();
        }
        return DebugCommand("global", -1);
    }
    if (type != "node" && type != "edge" && type != "ant" && type != "queen") {
        print_error(command, iss.tellg(),
                    "Invalid display type, expected node, edge, ant or queen");
        return DebugCommand();
    }
    int id = -1;
    iss >> id;
    if (iss.fail()) {
        print_error(command, iss.tellg(), "Invalid id");
        return DebugCommand();
    }
    int id2 = -1;
    if (type == "edge") {
        iss >> id2;
        if (iss.fail()) {
            print_error(command, iss.tellg(), "Invalid id");
            return DebugCommand();
        }
    }
    if (iss.tellg() != -1) {
        print_error(command, iss.tellg(), "Invalid command");
        return DebugCommand();
    }
    return DebugCommand(type, id, id2);
}
void Debugger::print_error(const std::string &command, int column,
                           const std::string &message) const {
    std::cerr << "Error: " << message << "\n";
    std::cerr << command << "\n";
	if (column >= 0) {
		for (int i = 0; i < column; i++) {
			std::cerr << " ";
		}
		std::cerr << "~";
		for (unsigned int i = 0; i < command.size() - column - 1; i++) {
			std::cerr << "^";
		}
	}
    std::cerr << std::endl;
}

void Debugger::display_element(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                               const std::vector<std::unique_ptr<Queen>> &queens,
                               const std::string &type, int id1, int id2) const {
    if (id1 < 0 && type != "global") {
        std::cerr << "Invalid id\n";
        return;
    }
    if (id2 < 0 && type == "edge") {
        std::cerr << "Invalid id\n";
        return;
    }
    unsigned int uid1 = static_cast<unsigned int>(id1);
    unsigned int uid2 = static_cast<unsigned int>(id2);

    if (type == "global") {
        std::cout << "== Global statistics ==\n";
        std::cout << "Nodes: " << map.get_node_count() << "\n";
        std::cout << "Teams: " << map.get_team_count() << "\n";
        std::cout << "Ants: " << ants.size() << "\n";
        for (const auto &team : map.get_teams()) {
            std::cout << "Team " << team.get_name() << " (" << team.get_id()
                      << ") score: " << team.get_score() << "\n";
        }
    } else if (type == "node") {
        const auto &nodes = map.get_nodes();
		const auto &node = nodes.find(uid1);
		if (node == nodes.end()) {
			std::cerr << "Node not found\n";
			return;
		}
        std::cout << node->second << std::endl;
    } else if (type == "edge") {
        auto edge = map.get_edge(uid1, uid2);
		if (edge == nullptr) {
			std::cerr << "Edge not found\n";
			return;
		}
        std::cout << *edge << std::endl;
    } else if (type == "ant") {
        const auto &ant =
            std::ranges::find_if(ants, [uid1](const auto &ant) { return ant->get_id() == uid1; });
		if (ant == ants.end()) {
			std::cerr << "Ant not found\n";
			return;
		}
        std::cout << **ant << std::endl;
    } else if (type == "queen") {
        const auto &queen = std::ranges::find_if(
            queens, [uid1](const auto &queen) { return queen->get_team_id() == uid1; });
		if (queen == queens.end()) {
			std::cerr << "Queen not found\n";
			return;
		}
        std::cout << **queen << std::endl;
    }
    std::cout << std::endl;
}

void Debugger::list_elements(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                             const std::vector<std::unique_ptr<Queen>> &queens,
                             const std::string &type) const {
    if (type == "nodes") {
        std::cout << "== Nodes ==\n";
        for (const auto &[_, node] : map.get_nodes()) {
            std::cout << "Node :" << node.get_id() << " (" << node.get_type() << ")\n";
        }
    } else if (type == "edges") {
        std::cout << "== Edges ==\n";
        for (const auto &[_, node] : map.get_nodes()) {
            for (const auto &edge : node.get_edges()) {
                if (edge->get_node1()->get_id() == node.get_id()) {
                    std::cout << "Edge :" << edge->get_node1()->get_id() << " -- "
                              << edge->get_node2()->get_id() << "\n";
                }
            }
        }
    } else if (type == "ants") {
        std::cout << "== Ants ==\n";
        for (const auto &ant : ants) {
            std::cout << "Ant (" << ant->get_team_id() << ") :" << ant->get_id() << "\n";
        }
    } else if (type == "queens") {
        std::cout << "== Queens ==\n";
        for (const auto &queen : queens) {
            std::cout << "Queen :" << queen->get_team_id() << "\n";
        }
    } else {
        std::cerr << "Invalid type\n";
    }
    std::cout << std::endl;
}
void Debugger::list_ants_in(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                            const std::string &type, int id1, int id2) const {
    if (id1 < 0) {
        std::cerr << "Invalid id\n";
        return;
    }
    unsigned int uid1 = static_cast<unsigned int>(id1);
    if (id2 < 0 && type == "edge") {
        std::cerr << "Invalid id\n";
        return;
    }
    unsigned int uid2 = static_cast<unsigned int>(id2);
    if (type == "node") {
        const auto &nodes = map.get_nodes();
		const auto &nodeit = nodes.find(uid1);
		if (nodeit == nodes.end()) {
			std::cerr << "Node not found\n";
			return;
		}
		const auto &node = nodeit->second;
        std::cout << "== Ants in node " << node.get_id() << " ==\n";
        for (const auto &ant : node.get_ants()) {
            std::cout << "Ant :" << ant->get_id() << " " << ant->get_team_id() << "\n";
        }
    } else if (type == "edge") {
        auto edge = map.get_edge(uid1, uid2);
		if (edge == nullptr) {
			std::cerr << "Edge not found\n";
			return;
		}
        std::cout << "== Ants in edge " << edge->get_node1()->get_id() << " -- "
                  << edge->get_node2()->get_id() << " ==\n";
        for (const auto &ant : edge->get_ants()) {
            std::cout << "Ant :" << ant->get_id() << " " << ant->get_team_id() << "\n";
        }
    } else {
        std::cerr << "Invalid type\n";
    }
    std::cout << std::endl;
}

void Debugger::display_help() const {
    std::cout << "Help :\n";
    std::cout << " - step [n] : execute n steps, default is 1\n";
    std::cout << " - exit : exit the debugger\n";
    std::cout << " - help : display this help\n";
    std::cout << " - display node <id> : display node with id\n";
    std::cout << " - display edge <id1> <id2> : display edge between nodes with id1 and id2\n";
    std::cout << " - display ant <id> : display ant with id\n";
    std::cout << " - display queen <id> : display queen with id\n";
    std::cout << " - display global : display global statistics\n";
    std::cout << " - list nodes : list all nodes\n";
    std::cout << " - list edges : list all edges\n";
    std::cout << " - list ants : list all ants\n";
    std::cout << " - list queens : list all queens\n";
    std::cout << " - listin node <id> : list all ants in node with id\n";
    std::cout
        << " - listin edge <id1> <id2> : list all ants in edge between nodes with id1 and id2\n";
    std::cout << std::endl;
}
