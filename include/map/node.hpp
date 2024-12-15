#pragma once

#include "nlohmann/json.hpp"
#include "salle_parser.hpp"
#include "sinfourmis.h"

using json = nlohmann::json;

class Node {
  public:
    Node() = default;
	explicit Node(salle &&s) : salle_repr(std::move(s)) {}

	salle &as_salle() { return salle_repr; }

  private:
    salle salle_repr;
};