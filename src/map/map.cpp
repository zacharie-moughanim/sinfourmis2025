#include "map/map.hpp"
#include <fstream>
#include <iostream>

bool Map::load(const std::string_view &filename) {
    std::ifstream file(filename.data());
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    if (file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "File is empty: " << filename << std::endl;
        return false;
    }
    json data;
    try {
        data = json::parse(file);
    } catch (json::parse_error &e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        return false;
    }

    auto json_nodes = data.find("nodes");
    if (json_nodes == data.end()) {
        std::cerr << "Failed to find nodes in JSON" << std::endl;
        return false;
    }

    std::unordered_set<std::pair<unsigned int, unsigned int>> edges;
    for (const auto &node : *json_nodes) {
        try {
            auto node_obj = node.get<Node>();
            auto res = nodes.try_emplace(node_obj.get_id(), node_obj);
            if (!res.second) {
                std::cerr << "Duplicate node: " << node_obj.get_id() << std::endl;
                return false;
            }
            auto neighbors_it = node.find("neighbors");
            if (neighbors_it == node.end()) {
                continue;
            }
            std::vector<unsigned int> neighbors = neighbors_it->get<std::vector<unsigned int>>();
            for (const auto &neighbor : neighbors) {
                edges.insert({node_obj.get_id(), neighbor});
            }
        } catch (json::exception &e) {
            std::cerr << "Failed to parse node: " << e.what() << std::endl;
            return false;
        }
    }
    for (const auto &edge : edges) {
        auto [node1, node2] = edge;
        auto node1_it = nodes.find(node1);
        if (node1_it == nodes.end()) {
            std::cerr << "Node not found: " << node1 << std::endl;
            return false;
        }
        auto node2_it = nodes.find(node2);
        if (node2_it == nodes.end()) {
            std::cerr << "Node not found: " << node2 << std::endl;
            return false;
        }
        nodes[node1].add_edge(nodes[node2]);
    }

    auto json_teams = data.find("teams");
    if (json_teams == data.end()) {
        std::cerr << "Failed to find teams in JSON" << std::endl;
        return false;
    }
    for (const auto &team : *json_teams) {
        try {
            teams.emplace_back(team.get<Team>());
        } catch (json::exception &e) {
            std::cerr << "Failed to parse team: " << e.what() << std::endl;
            return false;
        }
    }

    to_dot("graph.dot");

    return true;
}

void Map::to_dot(const std::string_view &filename) const {
    std::ofstream file(filename.data());
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file << "graph {" << std::endl;
    for (const auto &node : nodes) {
        file << "  " << node.first << std::endl;
        for (const auto &edge : node.second.get_edges()) {
            if (edge->get_node1()->get_id() == node.first) {
                file << "  " << node.first << " -- " << edge->get_node2()->get_id() << std::endl;
            }
        }
    }
    file << "}" << std::endl;
    file.close();
}

void Map::regen_food() {
	for (auto &node : nodes) {
		node.second.regen_food();
	}
}