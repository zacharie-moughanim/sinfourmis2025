#pragma once

#include "game/ant.hpp"
#include "game/queen.hpp"
#include "map/map.hpp"
#include "utils/groups.hpp"
#include <variant>
#include <optional>

class Debugger {
public:
	explicit Debugger(bool debug) : m_debug(debug) {};

	void debug(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants, const std::vector<std::unique_ptr<Queen>> &queens);

	bool exit() const {
		return m_exit;
	}
private:
	// Example commands:
	// - step 10
	// - step
	// - exit
	// - help
	// - display node 1
	// - display edge 1 2
	// - display ant 1
	// - display queen 1

	struct DebugCommand {
        enum class Type { STEP, EXIT, HELP, DISPLAY, LIST, LISTIN, INVALID };

        DebugCommand() : type(Type::INVALID) {}
		DebugCommand(Type t) : type(t) {}
		DebugCommand(int a) : type(Type::STEP), arg(a) {}
		DebugCommand(const std::string &a) : type(Type::LIST), arg(std::make_tuple(a, -1, -1)) {}
		DebugCommand(const std::string &a, int b, int c = -1) : type(Type::DISPLAY), arg(std::make_tuple(a, b, c)) {}

		Type type;
		std::optional<std::variant<int, std::tuple<std::string, int, int>>> arg;
	};

    DebugCommand parse_command(const std::string &command) const;

    const Debugger::DebugCommand parse_id_command(std::istringstream &iss,
                                                   const std::string &command) const;

    void print_error(const std::string &command, int column, const std::string &message) const;
    void display_element(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                         const std::vector<std::unique_ptr<Queen>> &queens, const std::string &type,
                         int id1, int id2) const;
	void list_elements(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
					   const std::vector<std::unique_ptr<Queen>> &queens, const std::string &type) const;

	void list_ants_in(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants, const std::string &type, int id1, int id2) const;
	
    void display_help() const;

    bool m_debug;
	bool m_exit = false;
	int skip = 0;
};
