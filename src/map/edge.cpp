#include "map/edge.hpp"

Edge::Edge(Node *n1, Node *n2) : node1(n1), node2(n2) {
    auto [x1, y1] = n1->get_position();
    auto [x2, y2] = n2->get_position();
    length = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Node *Edge::get_other_node(const Node *node) const {
    if (node == node1) {
        return node2;
    }
    if (node == node2) {
        return node1;
    }
    return nullptr;
}

bool Edge::can_be_crossed() const {
    return life > EDGE_MIN_LIFE;
}

unsigned int Edge::attack(unsigned int damages) {
    if (life > damages) {
        life -= damages;
    } else {
        life = 0;
    }
    return life;
}

void Edge::dig() {
    if (life < EDGE_LIFE) {
        life++;
    }
}