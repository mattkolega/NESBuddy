#include "Logger.h"

#include <fmt/color.h>
#include <fmt/core.h>

namespace Logger 
{
    void printDebug(std::string_view message)
    {
        fmt::print(stderr, fg(fmt::color::light_golden_rod_yellow), "[DEBUG]: {}\n", message);
    }

    void printInfo(std::string_view message)
    {
        fmt::print(stdout, fg(fmt::color::powder_blue), "[INFO]: {}\n", message);
    }

    void printWarning(std::string_view message)
    {
        fmt::print(stderr, fg(fmt::color::orange), "[WARNING]: {}\n", message);
    }

    void printError(std::string_view message)
    {
        fmt::print(stderr, fg(fmt::color::red), "[ERROR]: {}\n", message);
    }
}