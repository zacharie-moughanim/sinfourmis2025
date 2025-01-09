#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct AntGroupData {
    unsigned int team;
    unsigned int qt;
    float progress = -1;
    float progress_anim = -1;

    AntGroupData(unsigned int team, unsigned int qt, float progress = -1, float progress_anim = -1)
        : team(team), qt(qt), progress(progress), progress_anim(progress_anim) {}

    friend void to_json(json &j, const AntGroupData &data) {
        j = json{
            {"team", data.team},
            {"qt", data.qt},
        };
        if (data.progress != -1) {
            j["progress"] = data.progress;
        }
        if (data.progress_anim != -1) {
            j["anim"] = json{{"progress", data.progress_anim}};
        }
    }
};