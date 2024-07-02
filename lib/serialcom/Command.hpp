#pragma once

#include <cstddef>
#include <unordered_map>
#include <optional>
#include <string>

namespace serialcom
{
    const size_t MAX_COMMMAND_TYPE_SIZE = 16;
    const size_t MAX_ARGUMENT_SIZE = 512;
    const size_t MAX_COMMAND_ARGUMENTS_COUNT = 5;
    constexpr size_t INPUT_MAX_SIZE = MAX_COMMMAND_TYPE_SIZE + MAX_COMMAND_ARGUMENTS_COUNT * MAX_ARGUMENT_SIZE;

    typedef void (*CommandCaller)(bool program_mode, char *args[]);

    struct CommandMeta
    {
        std::string name;
        bool isNested;
        CommandCaller exec;
    };

    struct Command
    {
        Command(char (&input)[INPUT_MAX_SIZE]);

        char *arguments[MAX_COMMAND_ARGUMENTS_COUNT + 1];

        int argoffset; // The length of the command name in the arguments array

        CommandCaller exec; // Command to be executed
    };
}