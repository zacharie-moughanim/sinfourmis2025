#pragma once

#include "nlohmann/json.hpp"
#include "salle_parser.h"
#include "sinfourmis.h"

using json = nlohmann::json;

class Node {
  public:
    Node() : id(-1), x(0), y(0), type(VIDE) {}
    Node(int id, float x, float y, salle_type type) : id(id), x(x), y(y), type(type) {}

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Node, id, x, y, type)

  private:
    unsigned int id;
    float x;
    float y;
    salle_type type;
};