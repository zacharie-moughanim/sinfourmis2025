#pragma once

#include "game/ant.hpp"
#include "interfaces/interface.hpp"
#include "map/map.hpp"
#include <iostream>
#include <vector>

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
    void run();

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

  private:
    Game() = default;

    Map map;
    unsigned int current_team = 0;
    std::map<unsigned int, Interface *> interfaces;
};