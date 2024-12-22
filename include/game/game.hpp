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
    ~Game() = default;

    void setMap(const Map &map);
    void addInterface(Interface *interface);

    void fourmi_action(Ant &ant);
    void run(unsigned int duration, unsigned int seed);

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

  private:
    Game() = default;

    std::mt19937_64 gen;
    Map map;
    unsigned int current_team = 0;
    std::map<unsigned int, Interface *> interfaces;
};