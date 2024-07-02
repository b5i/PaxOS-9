#pragma once

#include <cstddef>
#include <unordered_map>
#include <optional>

namespace serialcom
{
    const size_t MAX_COMMMAND_TYPE_SIZE = 16;
    const size_t MAX_ARGUMENT_SIZE = 512;
    const size_t MAX_COMMAND_ARGUMENTS_COUNT = 5;
    constexpr size_t INPUT_MAX_SIZE = MAX_COMMMAND_TYPE_SIZE + MAX_COMMAND_ARGUMENTS_COUNT * MAX_ARGUMENT_SIZE;

    struct CommandMeta
    {
        std::string name;
        bool isNested;
        void (*exec)(bool program_mode, char *args[]);
    };
    struct Command
    {
        Command(char (&input)[INPUT_MAX_SIZE]);

        char *arguments[MAX_COMMAND_ARGUMENTS_COUNT + 1];
    };
}