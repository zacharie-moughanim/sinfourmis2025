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

salle Node::as_salle() const {
    salle salle;
    salle.degre = edges.size();
    salle.type = type;
    salle.pheromone = pheromone;
    return salle;
}

void Node::add_ant(Ant *ant) {
    ants.insert(ant);
}

void Node::remove_ant(Ant *ant) {
    ants.erase(ant);
}

Edge *Node::get_edge(unsigned int id) const {
    return edges[id].get();
}

unsigned int Node::get_id_to(Node *node) const {
    for (unsigned int i = 0; i < edges.size(); i++) {
        if (edges[i]->get_other_node(this) == node) {
            return i;
        }
    }
    return -1;
}

unsigned int Node::gather_food(unsigned int max_food) {
    assert(this->type == salle_type::NOURRITURE);
    unsigned int food = std::min(max_food, this->food);
    this->food -= food;
    return food;
}

void Node::regen_food() {
    if (this->type == salle_type::NOURRITURE) {
        this->food = std::min(this->food + NODE_FOOD_REGEN, NODE_MAX_FOOD);
    }
}
