#include "Logger.h"

#include <chrono>

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>

namespace Logger 
{
    namespace
    {
        std::string getCurrentTime()  // Get current system time as a string
        {
            const auto currentTime = std::chrono::system_clock::now();
            return fmt::format("{:%T}", fmt::localtime(std::chrono::system_clock::to_time_t(currentTime)));
        }
    }

    void printDebug(std::string_view message)
    {
        fmt::print(stderr, fg(fmt::color::light_golden_rod_yellow), "[DEBUG] ({}) - {}\n", Logger::getCurrentTime(), message);
    }

    void printInfo(std::string_view message)
    {
        fmt::print(stdout, fg(fmt::color::powder_blue), "[INFO] ({}) - {}\n", Logger::getCurrentTime(), message);
    }

    void printWarning(std::string_view message)
    {
        fmt::print(stderr, fg(fmt::color::orange), "[WARNING] ({}) - {}\n", Logger::getCurrentTime(), message);
    }

    void printError(std::string_view message)
    {
        fmt::print(stderr, fg(fmt::color::red), "[ERROR] ({}) - {}\n", Logger::getCurrentTime(), message);
    }
}