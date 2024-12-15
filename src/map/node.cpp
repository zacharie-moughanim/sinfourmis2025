#include "map/node.hpp"

Node::~Node() {
    for (const auto &edge : edges) {
        if (edge->get_node1() == this) {
            edge->get_node2()->remove_edge(edge);
        } else {
            edge->get_node1()->remove_edge(edge);
        }
    }
}

void Node::add_edge(Node &other) {
    auto edge = std::make_shared<Edge>(this, &other);
    edges.push_back(edge);
    other.edges.push_back(edge);
}

void Node::remove_edge(const std::shared_ptr<Edge> &edge) {
    auto it =
        std::find_if(edges.begin(), edges.end(), [&edge](const auto &e) { return e == edge; });
    if (it != edges.end()) {
        edges.erase(it);
    }
}

salle Node::as_salle(unsigned int team_id) const {
    salle salle;
    salle.degre = edges.size();
    salle.type = type;
    if (auto it = pheromones.find(team_id); it != pheromones.end()) {
        salle.pheromone = it->second;
    } else {
        salle.pheromone = 0;
    }
    return salle;
}