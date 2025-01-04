#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct AntGroupData {
	unsigned int team;
	unsigned int qt;
	float progress = -1;

	friend void to_json(json &j, const AntGroupData &data) {
		j = json{
			{"team", data.team},
			{"qt", data.qt},
		};
		if (data.progress != -1) {
			j["progress"] = data.progress;
		}
	}
};