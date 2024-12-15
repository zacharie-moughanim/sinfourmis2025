#pragma once

#include "nlohmann/json.hpp"
#include "sinfourmis.h"

using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(salle_type, {
                                             {VIDE, "vide"},
                                             {EAU, "eau"},
                                             {NOURRITURE, "nourriture"},
                                         })
