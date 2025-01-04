#include "map/node.hpp"
#include <iostream>

void Node::add_edge(Node &other, unsigned int life) {
    auto edge = std::make_shared<Edge>(this, &other, life);
    edges.push_back(edge);
    other.edges.push_back(edge);
}

void Node::remove_edge(const std::shared_ptr<Edge> &edge) {
	std::erase(edges, edge);
}

salle Node::as_salle() const {
    return {
		.type = type,
		.pheromone = pheromone,
		.degre = (int32_t)edges.size()
	};
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

unsigned int Node::get_id_to(const Node *node) const {
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
