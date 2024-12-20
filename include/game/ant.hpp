#pragma once

#include "game/constants.hpp"
#include "map/node.hpp"
#include "sinfourmis.h"
#include <functional>

enum class AntActionState {
    NONE,
    MOVING,
    CONSTRUCTING,
};

class Ant {
  public:
    Ant(unsigned int team_id, Node *node, uint8_t max_life, uint32_t max_water);
    Ant(const Ant &&ant);
    Ant &operator=(const Ant &&ant);
    ~Ant();

    Node *get_current_node() const;
    void set_current_node(Node *node);

    void apply_damages(uint8_t damages);
    void water_action();

    bool alive();

    fourmi_etat as_fourmi_etat() const {
        return etat;
    }

    unsigned int get_team_id() const {
        return team_id;
    }

    void set_result(int32_t result) {
        this->etat.result = result;
    }

    AntActionState get_action_state() const {
        return action_state;
    }

    void move_along(Edge *edge);
    void displace();

    void begin_construction(Edge *edge);
    void construct();
    void stop_constructing();

  private:
    unsigned int team_id = 0;
    Node *current_Node = nullptr;
    fourmi_etat etat;
    uint8_t max_life = 0;
    uint32_t max_water = 0;

    AntActionState action_state = AntActionState::NONE;
    float displacement = 0;
    Edge *current_edge = nullptr;
};