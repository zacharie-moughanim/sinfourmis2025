#pragma once

class Node;

/**
 * @brief Represents an edge between two nodes.
 *
 */
class Edge {
  public:
    Edge() = default;
    Edge(Node *n1, Node *n2) : node1(n1), node2(n2) {}

    Node *get_node1() const {
        return node1;
    }
    Node *get_node2() const {
        return node2;
    }

  private:
    Node *node1 = nullptr;
    Node *node2 = nullptr;
};