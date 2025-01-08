#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct NeighborData {
    unsigned int to;
    float life;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NeighborData, to, life)
};
