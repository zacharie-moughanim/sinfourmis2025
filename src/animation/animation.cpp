#include "animation/animation.hpp"

// <start node id, team id, progress>
std::map<std::tuple<unsigned int, unsigned int, float>, unsigned int>
edge_groups(const Edge &edge) {
    std::map<std::tuple<unsigned int, unsigned int, float>, unsigned int> groups;
    for (auto ant : edge.get_ants()) {
        auto key = std::make_tuple(ant->get_current_node()->get_id(), ant->get_team_id(),
                                   ant->get_progress());
        auto it = groups.find(key);
        if (it == groups.end()) {
            groups[key] = 1;
        } else {
            groups[key]++;
        }
    }
    return groups;
}

void to_json(json &j, const Edge &edge) {
    j = json{
        {"ids", {edge.get_node1()->get_id(), edge.get_node2()->get_id()}},
        {"life_ratio", (float)edge.get_life() / (float)EDGE_LIFE},
    };
    auto groups = edge_groups(edge);
    for (auto [key, qt] : groups) {
        float progress = std::get<2>(key);
        float progress_anim = progress + EDGE_CROSS_SPEED;
        if (std::get<0>(key) == edge.get_node1()->get_id()) {
            progress /= edge.get_length();
            progress_anim /= edge.get_length();
        } else {
            progress = 1. - (progress / edge.get_length());
            progress_anim = 1. - (progress_anim / edge.get_length());
        }
        progress = std::max(0.f, std::min(1.f, progress));
        progress_anim = std::max(0.f, std::min(1.f, progress_anim));
        j["groups"].push_back(AntGroupData{std::get<1>(key), qt, progress});
        j["groups"].back()["anim"] = json{{"progress", progress_anim}};
    }
}

void Animation::write_edges_departure_groups(const Node &node) {
    int i = 0;
    for (auto edge : node.get_edges()) {
        if (edge->get_node1()->get_id() == node.get_id()) {
            std::map<unsigned int, unsigned int> departures;
            for (auto ant : node.get_ants()) {
                if (ant->get_action_state() == AntActionState::MOVING && ant->get_progress() == 0) {
                    auto it = departures.find(ant->get_team_id());
                    if (it == departures.end()) {
                        departures[ant->get_team_id()] = 1;
                    } else {
                        departures[ant->get_team_id()]++;
                    }
                }
            }
			if (!data["edges"][i].contains("groups") && !departures.empty()) {
				data["edges"][i]["groups"] = json::array();
			}
            for (auto [team, qt] : departures) {
                data["edges"][i]["groups"].push_back(AntGroupData{team, qt, 0});
                data["edges"][i]["groups"].back()["anim"] = json{{"progress", EDGE_CROSS_SPEED}};
            }
        }
    }
}

std::unordered_map<unsigned int, unsigned int> node_groups_map(const Node &node) {
    std::unordered_map<unsigned int, unsigned int> groups;
    for (auto ant : node.get_ants()) {
        auto it = groups.find(ant->get_team_id());
        if (it == groups.end()) {
            groups[ant->get_team_id()] = 1;
        } else {
            groups[ant->get_team_id()]++;
        }
    }
    return groups;
}

json Animation::node_groups(const Node &node) {
    auto groups = node_groups_map(node);
    auto res = json::array();
    for (auto [team, qt] : groups) {
        res.push_back(AntGroupData{team, qt});
    }
    return res;
}

json Animation::write_groups_animation(const Node &node, json &groups) {
    auto groups_map = node_groups_map(node);
    auto res = json::array();
    for (auto group : groups) {
        auto it = groups_map.find(group["team"]);
        if (it == groups_map.end()) {
            res.push_back(AntGroupData{group["team"], 0});
        } else {
            if (it->second != group["qt"]) {
                res.push_back(AntGroupData{group["team"], group["qt"]});
            }
            groups_map.erase(it);
        }
    }
    for (auto [team, qt] : groups_map) {
        groups.push_back(AntGroupData{team, 0});
        res.push_back(AntGroupData{team, qt});
    }
    return res;
}

void Animation::start_frame() {
    turn++;
    if (started) {
        throw std::runtime_error("Start / end frame must be called in pairs");
    }
    started = true;
    data = json::object();

    // save teams
    auto teams = json::array();
    for (auto team : map->get_teams()) {
        teams.push_back(team);
        teams.back()["score"] = team.get_food();
    }
    data["teams"] = teams;

    // save nodes and edges
    auto nodes = json::array();
    auto edges = json::array();
	unsigned int max_food = 0;

    for (auto [_ , node] : map->get_nodes()) {
		if (node.get_type() == salle_type::NOURRITURE) {
			max_food = std::max(max_food, node.get_max_food());
		}
        nodes.push_back(node);
        nodes.back()["ants"] = node_groups(node);
        for (auto edge : node.get_edges()) {
            if (edge->get_node1()->get_id() == node.get_id()) {
                edges.push_back(*edge);
            }
        }
    }
	data["max_food"] = max_food;
    data["nodes"] = nodes;
    data["edges"] = edges;
}

void Animation::end_frame() {
    if (!started) {
        throw std::runtime_error("Start / end frame must be called in pairs");
    }
    started = false;
    file.open(path / (std::to_string(turn) + ".json"));
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << path / (std::to_string(turn) + ".json") << std::endl;
        exit(1);
    }

    // teams animation
    auto teams = map->get_teams();
    for (uint i = 0; i < teams.size(); i++) {
        data["teams"][i]["next"] = json::object();
        if (data["teams"][i]["score"] != teams[i].get_food()) {
            data["teams"][i]["next"]["score"] = teams[i].get_food();
        }
    }

    // nodes animation
    for (auto &node_json : data["nodes"]) {
        node_json["anim"] = json::object();
        auto node = map->get_node(node_json["id"]);
        if (node.get_type() == salle_type::NOURRITURE && node_json["food"] != node.get_food()) {
            node_json["anim"]["food"] = node.get_food();
        }
        node_json["anim"]["ants"] = write_groups_animation(node, node_json["ants"]);
        write_edges_departure_groups(node);
    }

    // write data to file
    file << data.dump(4);
    file.close();
}
