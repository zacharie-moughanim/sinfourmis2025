#pragma once

#include "game/constants.hpp"
#include "map/node.hpp"
#include <cmath>

class Node;

/**
 * @brief Represents an edge between two nodes.
 *
 */
class Edge {
  public:
    Edge() = default;
    Edge(Node *n1, Node *n2);

    /**
     * @brief Get the other side node of the edge.
     *
     * @param node the current node
     * @return Node* the other node
     */
    Node *get_other_node(Node *node) const;

    bool can_be_crossed() const;

    void attack(unsigned int damages);
    void construct();

    float get_length() const {
        return length;
    }

    Node *get_node1() const {
        return node1;
    }
    Node *get_node2() const {
        return node2;
    }

  private:
    Node *node1 = nullptr;
    Node *node2 = nullptr;
    float length = 0;
    unsigned int life = EDGE_LIFE;
};