#include "Command.hpp"
#include "Commands/InfoCommand.hpp"
#include <iostream>

namespace serialcom
{
    Command::Command(char (&input)[INPUT_MAX_SIZE])
    {
        // detect the command type knowing that the maximum size that the command type can have is MAX_COMMMAND_TYPE_SIZE
        // char command_type[MAX_COMMMAND_TYPE_SIZE];

        // size_t command_size = MAX_COMMMAND_TYPE_SIZE;

        // for (size_t i = 0; i < MAX_COMMMAND_TYPE_SIZE; i++) {
        //     if (input[i] == ' ' || input[i] == '\0') {
        //         command_type[i] = '\0';
        //         command_size = i;
        //         break;
        //     }
        //     command_type[i] = input[i];
        // }

        // // match the command_type with a loop with the CommandType enum but don't forget that the input is a 16 char array

        // this->type = CommandType::unknown;
        // for (auto const& [key, val] : command_types_raw_strings) {
        //     if (val.size() == 0) {
        //         continue;
        //     }
        //     if (MAX_COMMMAND_TYPE_SIZE < val.size()) {
        //         SerialManager::sharedInstance->log("WARNING: COMMAND TYPE " + val + " HAS A GREATER SIZE THAN THE MAXIMUM COMMAND TYPE SIZE");
        //         continue;
        //     }

        //     bool broken = false;

        //     for(size_t i = 0; i < val.size(); i++) {
        //         if (command_type[i] != val[i]) {
        //             broken = true;
        //             break;
        //         }
        //     }
        //     if (!broken)
        //     {
        //         this->type = key;
        //         break;
        //     }
        // }

        // if (type == CommandType::unknown) {
        //     SerialManager::sharedInstance->log("ERROR: UNKONWN COMMAND TYPE");
        //     return;
        // }

        size_t current_index = 0;
        size_t argument_index = 0;
        for (argument_index = 0; argument_index < MAX_COMMAND_ARGUMENTS_COUNT; argument_index++)
        {
            // skip spaces
            while (input[current_index] == ' ' && current_index < INPUT_MAX_SIZE - 1)
            {
                current_index++;
            }

            // check if the input has ended
            if (input[current_index] == '\0')
            {
                break;
            }

            bool isUsingQuotes = false;

            if (input[current_index] == '"')
            {
                isUsingQuotes = true;
                current_index++;
            }

            // Add the argument to the argument list by using an offseted pointer
            this->arguments[argument_index] = input + current_index;

            bool isEscaping = false;
            size_t argument_char_index = 0;
            for (size_t i = current_index; i < current_index + MAX_ARGUMENT_SIZE; i++)
            {
                if (input[i] == '\0')
                {
                    current_index = i;
                    break;
                }

                else if (input[i] == '\\')
                {
                    if (isEscaping)
                    {
                        // escaping backslash
                        isEscaping = false;
                        input[current_index + argument_char_index] = '\\';
                        argument_char_index++;
                    }
                    else
                    {
                        // start escaping
                        isEscaping = true;
                    }
                }
                else if (input[i] == ' ')
                {
                    if (isUsingQuotes)
                    {
                        // space and is using quotes
                        input[current_index + argument_char_index] = ' ';
                        argument_char_index++;
                        continue;
                    }

                    // not using quotes => need to escape space characters
                    if (isEscaping)
                    {
                        // escaping space
                        isEscaping = false;
                        input[current_index + argument_char_index] = ' ';
                        argument_char_index++;
                        continue;
                    }
                    else
                    {
                        // end of argument because of space
                        input[current_index + argument_char_index] = '\0';
                        current_index = i + 1;
                        break;
                    }
                }
                else if (input[i] == '"')
                {
                    if (isUsingQuotes)
                    {
                        if (isEscaping)
                        {
                            // escaping quote
                            isEscaping = false;
                            input[current_index + argument_char_index] = '"';
                            argument_char_index++;
                        }
                        else
                        {
                            // end of argument because of quote
                            input[current_index + argument_char_index] = '\0';
                            current_index = i + 1;
                            break;
                        }
                    }
                    else
                    {
                        if (isEscaping)
                        {
                            // escaping quote
                            isEscaping = false;
                            input[current_index + argument_char_index] = '"';
                            argument_char_index++;
                        }
                        else
                        {
                            SerialManager::sharedInstance->log("ERROR: UNEXPECTED QUOTE CHARACTER AT POSITION " + std::to_string(i));
                            return;
                        }
                    }
                }
                else
                {
                    if (isEscaping)
                    {
                        // escaping character
                        isEscaping = false;
                        input[current_index + argument_char_index] = '\\';
                        argument_char_index++;
                    }
                    input[current_index + argument_char_index] = input[i];
                    argument_char_index++;
                }
            }
        }

        if (argument_index == 0)
        {
            return;
        }

        // Last array element
        this->arguments[argument_index] = nullptr;
    }

    static CommandMeta command_tree[]{
        {"info", true, nullptr},
        {"info hostname", false, nullptr},
        {"info mac", true, nullptr},
        {"info mac ble", false, nullptr},
        {"info mac wifi", false, nullptr},
        {"info version", false, nullptr},
        {"info hardware", false, nullptr},

        {"echo", false, nullptr},

        {"apps", true, nullptr},
        {"apps install", false, nullptr},
        {"apps remove", false, nullptr},
        {"apps launch", false, nullptr},
        {"apps show", false, nullptr},

        {"files", true, nullptr},
        {"files ls", false, nullptr},
        {"files cd", false, nullptr},
        {"files pwd", false, nullptr},
        {"files touch", false, nullptr},
        {"files mkdir", false, nullptr},
        {"files rm", false, nullptr},
        {"files cp", false, nullptr},
        {"files mv", false, nullptr},
        {"files upload", false, nullptr},
        {"files download", false, nullptr},

        {"elevate", false, nullptr}, // La 2e partie est un argument

        {"lte", true, nullptr},
        {"lte at", false, nullptr},
        {"lte imei", false, nullptr},
        {"lte imsi", false, nullptr},

    };
}
