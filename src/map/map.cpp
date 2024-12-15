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
    for (const auto &node : *json_nodes) {
        try {
            nodes.emplace_back(node.get<Node>());
        } catch (json::exception &e) {
            std::cerr << "Failed to parse node: " << e.what() << std::endl;
            return false;
        }
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

    return true;
}