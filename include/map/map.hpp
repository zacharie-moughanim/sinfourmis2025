#pragma once

#include "game/team.hpp"
#include "map/node.hpp"
#include "pair_hash.hpp"
#include "salle_parser.hpp"
#include "sinfourmis.h"
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief Represents the game map
 *
 */
class Map {
  public:
    Map() = default;

    /**
     * @brief Load the map from a file
     *
     * @param filename the file to load the map from
     * @return true if the map was loaded successfully
     * @return false if the map could not be loaded because of an error or a malformed data type
     */
    bool load(const std::string_view &filename);

    /**
     * @brief Function used for debugging purposes to print the map to a .dot file
     *
     * @param filename the file to write the map to
     */
    void to_dot(const std::string_view &filename) const;

    /**
     * @brief Regenerate the food on the map
     *
     */
    void regen_food();

    size_t get_node_count() const {
        return nodes.size();
    }

    size_t get_team_count() const {
        return teams.size();
    }

    const Team &get_team(unsigned int id) const {
        return teams.at(id);
    }

  private:
    std::vector<Team> teams;
    std::unordered_map<unsigned int, Node> nodes;
};
