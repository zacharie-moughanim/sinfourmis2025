#include "argparse/argparse.hpp"
#include "map/map.hpp"
#include <iostream>

int main(int argc, char **argv) {
    argparse::ArgumentParser program("sinfourmis");
    program.add_description("The 2025 sinfourmis simulator");
    program.add_epilog("MIT Licence - Copyright (c) 2025 - sinfonie");
    program.add_argument("map").help("The map file to use").required().metavar("MAP");
    program.add_argument("-c", "--check")
        .help("Check the map file for errors and exit")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("-o", "--output")
        .help("The combat output file used to store the results")
        .metavar("OUTPUT");

    program.add_group("Simulation options:");
    program.add_argument("-d", "--duration")
        .help("The duration of the simulation in ticks")
        .default_value(1000)
        .nargs(1)
        .scan<'i', unsigned int>()
        .metavar("DURATION");
    program.add_argument("-s", "--seed")
        .help("The seed to use for the random number generator")
        .default_value(42)
        .nargs(1)
        .scan<'i', unsigned int>()
        .metavar("SEED");

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        std::cout << program;
        return 1;
    }

    Map map;
    if (!map.load(program.get<std::string>("map"))) {
        return 2;
    }
    if (program["check"] == true) {
        std::cout << "Map is valid" << std::endl;
        return 0;
    }

    // TODO: Run the simulation

    return 0;
}