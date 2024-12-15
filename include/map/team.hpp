#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Team {
  public:
    Team() : id(-1), name("") {}
    Team(int id, std::string name, std::string color) : id(id), name(name) {}

    std::string getName() const {
        return name;
    }
    unsigned int getId() const {
        return id;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Team, id, name)

  private:
    unsigned int id;
    std::string name;
};
