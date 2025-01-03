#include "argparse/argparse.hpp"
#include "game/game.hpp"
#include "map/map.hpp"
#include "interfaces/dummy.hpp"
#include <iostream>
#include <random>

int main(int argc, char **argv) {
    argparse::ArgumentParser program("sinfourmis");
    program.add_description("The 2025 sinfourmis simulator");
    program.add_epilog("MIT Licence - Copyright (c) 2025 - sinfonie");
    program.add_argument("map").help("The map file to use").required().metavar("MAP");
    auto &group = program.add_mutually_exclusive_group(true);

    std::random_device rd;

    group.add_argument("-c", "--check")
        .help("Check the map file for errors and exit")
        .default_value(false)
        .implicit_value(true);

    group.add_argument("-t", "--team").help("Team files to use").nargs(1).append().metavar("TEAM");

    program.add_argument("-o", "--output")
        .help("The combat output file used to store the results")
        .metavar("OUTPUT");

    program.add_group("Simulation options:");
    program.add_argument("-d", "--duration")
        .help("The duration of the simulation in turns")
        .default_value(1000)
        .nargs(1)
        .scan<'i', int>()
		.action([](const std::string &value) {
			int duration = std::stoi(value);
			if (duration <= 0) {
				throw std::runtime_error("Duration must be a positive integer");
			}
			return duration;
		})
        .metavar("DURATION");
    program.add_argument("-s", "--seed")
        .help("The seed to use for the random number generator, default to a random value")
        .default_value((int)rd())
		.nargs(1)
        .scan<'i', int>()
		.action([](const std::string &value) {
			return std::stoi(value);
		})
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

    auto teams_opt = program.present<std::vector<std::string>>("team");
    if (!teams_opt.has_value()) {
        std::cerr << "No team files provided" << std::endl;
        return 1;
    }
    const std::vector<std::string> &teams = teams_opt.value();
    if (teams.size() != map.get_team_count()) {
        std::cerr << "Invalid number of teams : " << teams.size() << " instead of "
                  << map.get_team_count() << std::endl;
        return 1;
    }

    Game &game = Game::getInstance();
    game.set_map(std::move(map));
	int team_id = 0;
    for (const std::string &team : teams) {
        if (team == "dummy") {
			game.add_interface(team_id, new Dummy());
		} else {
			std::cerr << "Unknown team file : " << team << std::endl;
			return 1;
		}
		team_id++;
    }

	int duration = program.get<int>("duration");
	int seed = program.get<int>("seed");

    game.run(duration, seed);

    return 0;
}