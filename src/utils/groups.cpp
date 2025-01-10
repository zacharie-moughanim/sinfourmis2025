#include "utils/groups.hpp"

// <start node id, team id, progress>
std::map<std::tuple<unsigned int, unsigned int, float>, unsigned int>
edge_groups(const Edge &edge) {
    std::map<std::tuple<unsigned int, unsigned int, float>, unsigned int> groups;
    for (auto ant : edge.get_ants()) {
        auto key = std::make_tuple(ant->get_current_node()->get_id(), ant->get_team_id(),
                                   ant->get_progress());
        groups[key]++;
    }
    return groups;
}

std::unordered_map<unsigned int, unsigned int> node_groups(const Node &node) {
    std::unordered_map<unsigned int, unsigned int> groups;
    for (const auto &ant : node.get_ants()) {
        groups[ant.first] = ant.second.size();
    }
    return groups;
}