#pragma once

#include "game/ant.hpp"
#include "game/queen.hpp"
#include "interfaces/interface.hpp"
#include "map/map.hpp"
#include <algorithm>
#include <iostream>
#include <random>

/**
 * @brief Game object. It is a singleton that holds the game state and runs the game
 *
 */
class Game {
  public:
    static Game &getInstance();
    ~Game();

    void set_map(const Map &map);
    void add_interface(Interface* interface);

    void run(unsigned int duration, unsigned int seed);

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

  private:
    Game() = default;
    /**
     * @brief Perform the ant action and update game state accordingly
     *
     * @param ant he ant to run
     */
    void fourmi_action(Ant &ant);

    /**
     * @brief Perform the queen action and update game state accordingly
     *
     * @param queen the queen to run
     * @param ants the vector of ants in the game
     */
    void queen_action(Queen &queen, std::vector<Ant> &ants);

    std::mt19937_64 gen;
    Map map;
    unsigned int current_team = 0;
    std::map<unsigned int, Interface *> interfaces;
};