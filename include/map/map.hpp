#pragma once

#include "map/node.hpp"
#include "map/team.hpp"
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
