#pragma once

#include "animation/animation.hpp"
#include "debugger/debugger.hpp"
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

    void set_map(Map &&map);
    void add_interface(unsigned int team, Interface *interface);

    /**
     * @brief Run the game for a given duration and seed
     *
     * @param duration the game duration in ticks
     * @param seed The game seed
     * @param flush if true, the animation will be flushed at each turn
     * @param debug if true, the game will run step by step to debug
     * @param path the path to the animation file
     */
    void run(unsigned int duration, unsigned int seed, bool flush, bool debug,
             std::filesystem::path &&path);

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

  private:
    Game() = default;
    /**
     * @brief Perform the ant action and update game state accordingly
     *
     * @param ant he ant to run
     */
    void fourmi_action(Ant *ant);

    /**
     * @brief Perform the queen action and update game state accordingly
     *
     * @param queen the queen to run
     * @param ants the vector of ants in the game
     */
    void queen_action(Queen *queen, std::vector<std::unique_ptr<Ant>> &ants);

    std::mt19937_64 gen;
    Map map;
    std::map<unsigned int, Interface *> interfaces;
};