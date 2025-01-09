#pragma once

#include "game/ant.hpp"
#include "game/queen.hpp"
#include "map/map.hpp"
#include "utils/groups.hpp"
#include <optional>
#include <variant>

class Debugger {
  public:
    explicit Debugger(bool debug) : m_debug(debug) {};

    void debug(unsigned int turn, const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
               const std::vector<std::unique_ptr<Queen>> &queens);

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
        enum class Type { STEP, GOTO, EXIT, HELP, DISPLAY, LIST, LISTIN, INVALID };

        DebugCommand(Type t, const std::string &a, int b, int c)
            : type(t), arg(std::make_tuple(a, b, c)) {}
        DebugCommand(Type t, int a) : type(t), arg(a) {}
        DebugCommand(Type t, const std::string &a) : type(t), arg(a) {}

        static DebugCommand step(int n) {
            return DebugCommand(Type::STEP, n);
        }
        static DebugCommand exit() {
            return DebugCommand(Type::EXIT, "");
        }
        static DebugCommand help() {
            return DebugCommand(Type::HELP, "");
        }
        static DebugCommand display(const std::string &type, int id1, int id2) {
            return DebugCommand(Type::DISPLAY, type, id1, id2);
        }
        static DebugCommand list(const std::string &type) {
            return DebugCommand(Type::LIST, type);
        }
        static DebugCommand listin(const std::string &type, int id1, int id2) {
            return DebugCommand(Type::LISTIN, type, id1, id2);
        }
        static DebugCommand invalid() {
            return DebugCommand(Type::INVALID, "");
        }

        static DebugCommand goto_step(int n) {
            return DebugCommand(Type::GOTO, n);
        }

        int get_int_arg() const {
            return std::get<int>(arg);
        }

        std::string get_string_arg() const {
            return std::get<std::string>(arg);
        }

        std::tuple<std::string, int, int> get_tuple_arg() const {
            return std::get<std::tuple<std::string, int, int>>(arg);
        }

        Type type;
        std::variant<int, std::string, std::tuple<std::string, int, int>> arg;
    };

    DebugCommand parse_command(const std::string &command) const;

    const Debugger::DebugCommand parse_id_command(std::istringstream &iss,
                                                  const std::string &command) const;

    void print_error(const std::string &command, int column, const std::string &message) const;
    void display_element(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                         const std::vector<std::unique_ptr<Queen>> &queens, const std::string &type,
                         int id1, int id2) const;
    void list_elements(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                       const std::vector<std::unique_ptr<Queen>> &queens,
                       const std::string &type) const;

    void list_ants_in(const Map &map, const std::vector<std::unique_ptr<Ant>> &ants,
                      const std::string &type, int id1, int id2) const;

    void display_help() const;

    bool m_debug;
    bool m_exit = false;
    int skip = 0;
};
