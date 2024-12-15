#pragma once

#include "sinfourmis.h"
#include "salle_parser.hpp"
#include "map/team.hpp"
#include "map/node.hpp"
#include <string_view>
#include <vector>

class Map {
  public:
    Map() = default;

    bool load(const std::string_view &filename);

  private:
    std::vector<Team> teams;
    std::vector<Node> nodes;
};
