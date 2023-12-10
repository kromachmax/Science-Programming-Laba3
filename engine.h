#ifndef ENGINE_H
#define ENGINE_H

#include "wrapper.h"

class Engine {
public:
    template <typename FT>
    void register_command(Wrapper<FT>* wrapper, std::string command) {
        if (commands.find(command) != commands.end()) {
            throw std::runtime_error("Command " + command + " is already exist!!!");
        }
        commands[command] = *wrapper;
    }

    int execute(const std::string& command, const std::unordered_map<std::string, int>& args) {
        auto command_iterator = commands.find(command);

        if (command_iterator == commands.end()) {
            throw std::runtime_error("Command not found!!!");
        }

        auto func = command_iterator->second;

        return func(std::move(args));
    }

private:
    std::unordered_map<std::string, std::function<int(std::unordered_map<std::string, int>)>> commands;
};

#endif // ENGINE_H
