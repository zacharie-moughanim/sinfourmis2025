#pragma once

#include "map/edge.hpp"
#include "map/node.hpp"
#include "game/ant.hpp"
#include <unordered_map>

std::map<std::tuple<unsigned int, unsigned int, float>, unsigned int> edge_groups(const Edge &edge);
std::unordered_map<unsigned int, unsigned int> node_groups(const Node &node);
