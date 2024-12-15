#pragma once

#include "map/edge.hpp"
#include "nlohmann/json.hpp"
#include "salle_parser.hpp"
#include "sinfourmis.h"
#include <memory>

using json = nlohmann::json;

class Node {
  public:
    Node() = default;
    ~Node();

	// for json serialization / deserialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Node, id, type, x, y)

	/**
	 * @brief Convert the node to a salle that can be sent to the ants simulation
	 * 
	 * @param team_id the id of the team requesting the salle
	 * @return salle the salle representation of the node
	 */
    salle as_salle(unsigned int team_id) const;

	/**
	 * @brief Add an edge from this node to another node. It is undirected so the other node will also have an edge to this node.
	 * 
	 * @param other the node to add an edge to
	 */
    void add_edge(Node &other);

    unsigned int get_id() const {
        return id;
    }
    std::vector<std::shared_ptr<Edge>> get_edges() const {
        return edges;
    }
    std::vector<std::shared_ptr<Edge>> &get_edges() {
        return edges;
    }

  private:
    void remove_edge(const std::shared_ptr<Edge> &edge);

    unsigned int id = 0;
    salle_type type = salle_type::VIDE;
    float x = 0;
    float y = 0;

    std::unordered_map<unsigned int, uint8_t> pheromones;
    std::vector<std::shared_ptr<Edge>> edges;
};