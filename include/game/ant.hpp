#pragma once

#include "game/constants.hpp"
#include "game/queen.hpp"
#include "map/node.hpp"
#include "sinfourmis.h"
#include <functional>

enum class AntActionState {
    NONE,
    MOVING,
    DIGGING,
};

class Ant {
  public:
    Ant(Node *node, Queen *queen);
    Ant(const Ant &&ant);
    Ant &operator=(const Ant &&ant);
    ~Ant();

    Node *get_current_node() const;
    void set_current_node(Node *node);

    void apply_damages(uint8_t damages);
    void water_action();

    bool alive();

    void move_along(Edge *edge);
    void displace();

    void begin_digging(Edge *edge);
    void dig();
    void stop_digging();

    unsigned int gather_food();

    fourmi_etat as_fourmi_etat() const {
        return etat;
    }

    unsigned int get_team_id() const {
        return queen->get_team_id();
    }

    unsigned int get_attack() const {
        return queen->get_attack();
    }

    void set_result(int32_t result) {
        this->etat.result = result;
    }

    AntActionState get_action_state() const {
        return action_state;
    }

  private:
    Node *current_Node = nullptr;
    Queen *queen = nullptr;
    fourmi_etat etat;

    AntActionState action_state = AntActionState::NONE;
    float displacement = 0;
    Edge *current_edge = nullptr;
};