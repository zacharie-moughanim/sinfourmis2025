#pragma once

#include "game/constants.hpp"
#include "game/team.hpp"
#include "sinfourmis.h"
#include <array>
#include <cstring>
#include <optional>
#include <vector>

class Node;

class Queen {
  public:
    Queen(Team *team, Node *node) : team(team), current_node(node) {}

    enum class Stat : uint32_t { LIFE, WATER, FOOD, ATTACK, STORED_ANT };
    enum class QueenStat : uint32_t { STORED_ANTS, PRODUCED_ANTS, UPGRADE_DURATION, ANTS_SENDING };

    /**
     * @brief Signal the queen that a game turn passed
     *
     */
    void game_turn();

    /**
     * @brief Check if the queen can perform an actions
     *
     * @return true if the queen can perform an action
     */
    bool can_perform_action() const;

    /**
     * @brief Run the upgrade of the given ant stat
     *
     * @param type the stat to upgrade
     * @return true if the upgrade can be performed
     */
    bool upgrade(Stat type);

    /**
     * @brief Run the upgrade for the given queen stat
     *
     * @param type the stat to upgrade
     * @return true if the upgrade can be performed
     */
    bool upgrade_queen(QueenStat type);

    uint32_t get_stat(Stat type) const;
    uint32_t get_queen_stat(QueenStat type) const;
    reine_etat as_reine_etat() const;

    fourmi_etat default_fourmi_etat() const;

    /**
     * @brief create a new ant in the queen storage
     *
     * @return true if the ant has been created
     */
    bool create_ant();

    /**
     * @brief push the given ant to the queen storage
     *
     * @param ant the ant state tu push
     * @return true if the ant has been successfully pushed
     */
    bool push_ant(fourmi_etat ant);

    /**
     * @brief pop an ant state from the queen storage
     *
     * @return std::optional<fourmi_etat> the state popped or `std::nullopt` if here is no ants to
     * pop
     */
    std::optional<fourmi_etat> pop_ant();

    unsigned int get_victory_points() const {
        return team->get_food();
    }

    unsigned int get_team_id() const {
        return team->get_id();
    }

    Node *get_current_node() const {
        return current_node;
    }

    std::vector<fourmi_etat> &get_states() {
        return ants_memory;
    }

    void set_result(int32_t result) {
        this->result = result;
    }

  private:
    Team *team;
    Node *current_node = nullptr;
    unsigned int waiting_upgrade = 0;
    int32_t result = 0;

    std::array<uint32_t, 5> stats{DEFAULT_MAX_LIFE, DEFAULT_MAX_WATER, DEFAULT_MAX_FOOD,
                                  DEFAULT_ATTACK, QUEEN_DEFAULT_MAX_STORED_ANT};

    std::array<uint32_t, 4> queen_stats{QUEEN_DEFAULT_MAX_STORED_ANT, QUEEN_DEFAULT_PRODUCED_ANTS,
                                        QUEEN_DEFAULT_UPGRADE_DURATION, QUEEN_DEFAULT_SENT_ANTS};

    const static std::array<uint32_t, 5> upgrade_costs;
    const static std::array<uint32_t, 4> queen_upgrade_costs;

    std::vector<fourmi_etat> ants_memory;
};